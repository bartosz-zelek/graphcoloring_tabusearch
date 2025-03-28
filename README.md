### **Graph Coloring with Tabu Search**

**Description:**
A graph coloring algorithm using the Tabu Search metaheuristic implemented with OpenMP for parallel execution.

**Repository:**
[graphcoloring_tabusearch](https://github.com/bartox7777/graphcoloring_tabusearch/tree/main/CPP)

#### **Algorithm Overview**

- Starts with greedy coloring.
- Iteratively attempts to reduce the number of colors by resolving conflicts using Tabu Search.
- Key steps:
    - Randomly select a conflicting vertex and assign it a new color not in the Tabu list.
    - Check if the new coloring reduces conflicts or is valid.
    - Update the solution and Tabu list accordingly.


#### **Pseudocode**

```python
solution = greedy_coloring(GRAPH)
# Parallel processing across threads
for i in solution.size():
    temp = solution
    min = smallest_color_class(solution)
    redistribute_nodes_and_delete_color(temp, min)
    new_coloring = tabusearch(temp)
    if coloring_valid(temp):
        best_solutions[TID] = temp

best_solution = minimal_colors(best_solutions)
```


#### **Example Execution**

- Initial greedy coloring: 4 colors.
- Tabu Search reduces this to a valid coloring with fewer colors (e.g., 3).


#### **Performance on Graph Instances**

| Graph Instance | Time (s) | Best Solution Size |
| :-- | :-- | :-- |
| GC1000 | 50.64 | 151 |
| le450_5a | 83.12 | 5 |
| GC500 | 14.65 | 76 |

Graph instances used:

- [GC1000](https://github.com/bartox7777/graphcoloring_tabusearch/blob/main/instances/gc1000.txt)
- [le450_5a](https://github.com/bartox7777/graphcoloring_tabusearch/blob/main/instances/le450_5a.txt)
- [GC500](https://github.com/bartox7777/graphcoloring_tabusearch/blob/main/instances/gc500.txt)
