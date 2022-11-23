#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

class MatrixGraph
{
public:
    void initialize(unsigned int n)
    {
        v = std::vector<std::vector<int>>(n, std::vector<int> (n, 0));
        size = n;
    }
    void add_edge(unsigned int row, unsigned int col)
    {
        if (row == col)
        {
            std::cout << "Row == Col - you probably don't wanna do that" << std::endl;
            return;
        }
        if (v[row][col] != 0)
        {
            std::cout << "Edge already Exists";
            return;
        }
        v[row][col] = 1;
        v[col][row] = 1;
        edge_count++;
    }
    std::vector<int> get_neighbours(unsigned int node)
    {
        std::vector<int> res;
        if (node > size)
        {
            std::cout << "This node does not exist" << std::endl;
            return res;
        }

        for (int i = 0; i < size; i++)
        {
            if (v[node][i])
            {
                res.push_back(i);
            }
        }
        return res;
    }
    unsigned int get_size()
    {
        return size;
    }
    void get_graph_from_instance_file(char* file_name)
    {
        FILE * f = freopen(file_name, "r", stdin);
        FILE * f_temp = freopen("temp_file.txt", "w", stdout);
        std:: cin >> size;
        initialize(size);

        std::string line;
        unsigned int lines = 0;
        while (std::getline(std::cin, line))
        {
            if (lines) // From 1 cuz for some reason 1st line in temp file is newline
            {
                std::cout << line << std::endl;
            }
            lines++;
        }
        fclose(f);
        fclose(f_temp);
        FILE * f_temp_read = freopen("temp_file.txt", "r", stdin);
        for (int i = 0; i < lines; i++) 
        {
            unsigned int x, y;
            std::cin >> x >> y;
            add_edge(--x,--y); // Not indexed from 0
        }
        fclose(f_temp_read);
    }
    void print_graph_to_file()
    {
        FILE * f = freopen("cpp_graph.txt", "w", stdout);
        for (int i = 0; i< size; i++)
        {
            for (int j = 0; j< size; j++)
            {
                std::cout << v[i][j] << " ";
            }
            std::cout << std::endl;
        }
        fclose(f);
    }

private:
    std::vector<std::vector<int>> v;
    unsigned int edge_count = 0;
    unsigned int size = 0;
};

int main()
{
    MatrixGraph G;
    // G.initialize(20);
    // G.add_edge(0,1);
    G.get_graph_from_instance_file("mycie14.txt");
    G.print_graph_to_file();
    return 0;
}