#ifndef MAZESOLVER_HPP
#define MAZESOLVER_HPP
#include "dfs.hpp"
#include "dijkstra.hpp"

void mazesolver(int size_x, int size_y, int* maze, int start, int end, int world_size, int world_rank, int option);
#endif