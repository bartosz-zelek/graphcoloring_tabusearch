#include "MatrixGraph.h"
#include "tabu_search.h"
#include "omp.h"

#include <cstdlib>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <string>
#include <iostream>
std::vector<std::vector<int>> propose_solution(MatrixGraph G, int k)
{
    std::vector<std::vector<int>> r(k);
    for (int i = 0; i < k - 1; i++)
    {
        r[i].push_back(i);
    }
    for (int i = k - 1; i < G.get_size(); i++)
    {
        r[k - 1].push_back(i);
    }
    return r;
}

void print_sol_to_file(std::vector<std::vector<int>> &sol, std::ostream &f)
{
    for (int i = 0; i < sol.size(); i++)
    {
        f << i << ": ";
        for (auto v : sol.at(i))
        {
            f << v << " ";
        }
        f << "\n";
    }
}

std::vector<std::vector<int>> greedy_coloring(MatrixGraph &G)
{
    auto g_size = G.get_size();
    std::vector<std::vector<int>> colours(g_size);
    std::vector<unsigned int> nodes_degrees(g_size);
    std::vector<int> nodes(g_size);
    std::iota(nodes.begin(), nodes.end(), 0);
    for (int i = 0; i < g_size; i++)
    {
        nodes_degrees[i] = G.get_neighbours(i).size();
    }
    std::sort(nodes_degrees.begin(), nodes_degrees.end(), std::greater<int>());
    // set nodes in descending order of degrees
    std::sort(nodes.begin(), nodes.end(), [&nodes_degrees](int a, int b)
              { return nodes_degrees[a] > nodes_degrees[b]; });
    int greedy_coloring_count = 0;
    for (int i : nodes)
    {
        for (int j = 0; j < g_size; j++)
        {
            bool cant_assign_colour = false;
            if (colours[j].empty()) // No node has that colour
            {
                greedy_coloring_count++; // Increasing size of result vector
                cant_assign_colour = false;
                colours[j].push_back(i);
                break;
            }
            else // There are nodes with the same colour so we check if we can assign j-th colour
            {
                for (int k = 0; k < colours[j].size(); k++)
                {
                    if (G.edge_exists(i, colours[j][k]))
                    {
                        cant_assign_colour = true;
                        break;
                    }
                }
                if (!cant_assign_colour)
                {
                    colours[j].push_back(i); // We can assign j-th colour
                    cant_assign_colour = false;
                }
            }
            if (cant_assign_colour)
            {
                continue;
            }
            else
            {
                break;
            }
        }
    }
    return std::vector<std::vector<int>>(colours.begin(), colours.begin() + greedy_coloring_count);
}

int main(int argc, char *argv[])
{
    std::string file_path = "../instances/mycie14.txt";
    int number_of_neighbours = 50;
    int number_of_iterations = 500000;
    if (argc == 2)
    {
        file_path = argv[1];
    }
    else if (argc == 4)
    {
        file_path = argv[1];
        number_of_neighbours = std::atoi(argv[2]);
        number_of_iterations = std::atoi(argv[3]);
    }

    MatrixGraph G = MatrixGraph::get_graph_from_instance_file(file_path, false);

    std::string f_out;
    bool double_dot_found = false;
    for (int i = 0; i < file_path.length() - 1; i++)
    {
        if (file_path[i] != '.')
        {
            f_out += file_path[i];
            double_dot_found = false;
        }
        else if (file_path[i + 1] == '.')
        {
            f_out += file_path[i];
            double_dot_found = true;
        }
        else
        {
            if (double_dot_found)
            {
                f_out += file_path[i];
                continue;
            }
            break;
        }
    }
    f_out += "_out.txt";
    std::ofstream f(f_out);

    std::vector<std::vector<int>> r;
    std::vector<std::vector<int>> sol = greedy_coloring(G);
    std::vector<std::vector<int>> best_shared = sol;

    print_sol_to_file(sol, f);
    f.close();

    bool omp_brake = false;

    std::vector<std::vector<std::vector<int>>> best_solutions(omp_get_max_threads());

    int greedy_count;
// auto start = std::chrono::high_resolution_clock::now();
#pragma omp parallel default(none) firstprivate(omp_brake, G, number_of_neighbours, number_of_iterations, sol) private(greedy_count, r) shared(best_solutions)
    for (greedy_count = sol.size() - 1; greedy_count > 1; greedy_count--)
    {
        if (omp_brake)
        {
            continue;
        }

        auto temp = sol;
        auto min = std::min_element(temp.begin(), temp.end(), [](const auto &a, const auto &b)
                                    { return a.size() < b.size(); }); // Find the smallest colour class
        std::iter_swap(min, temp.end() - 1);                          // smallest colour class goes at the end of temp
        for (int i = 0; i < temp[temp.size() - 1].size(); i++)        // distribute colors from smallest class to the rest of the classes
        {
            temp[i % (temp.size() - 1)].push_back(temp[temp.size() - 1][i]);
        }
        temp.pop_back(); // delete last class
        r = tabu_search(G, greedy_count, temp, 7, number_of_neighbours, number_of_iterations);
        if (!r.empty())
        {
            sol = r;
            best_solutions[omp_get_thread_num()] = sol;
            printf("[TID: %d] %d\n", omp_get_thread_num(), greedy_count);
        }
        else
        {
            omp_brake = true;
        }
    }

    std::vector<std::vector<int>> best = best_solutions[0];
    for (auto sol : best_solutions)
    {
        if (sol.size() < best.size())
        {
            best = sol;
        }
    }
    printf("Best solution size: %ld\n.", best.size());
    f.open(f_out);
    print_sol_to_file(best, f);
    f.close();

    return 0;
}