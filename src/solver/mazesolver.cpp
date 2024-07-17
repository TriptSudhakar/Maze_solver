#include "mazesolver.hpp"

void mazesolver(int size_x, int size_y, int* maze, int start, int end, int world_size, int world_rank, int option)
{
    if(option == 0)
    {
        dfs_solver(size_x, size_y, maze, start, end, world_size, world_rank);
    }
    else
    {
        dijkstra_solver(size_x, size_y, maze, start, end, world_size, world_rank);
    }
}