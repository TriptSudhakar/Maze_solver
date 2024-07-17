#ifndef DIJKSTRA_HPP
#define DIJKSTRA_HPP
#include <iostream>
#include <cstdlib>
#include <random>
#include <vector>
#include <cmath>
#include <ctime>
#include <algorithm>
#include <mpi.h>

using namespace std;

vector<int> find_neighbors(int cell,int size_x,int size_y,int* maze);
pair<pair<bool,int>,pair<int,int>> team_meeting(int* distances,int size_x,int size_y,vector<int> &stack,int end,int world_size,int world_rank,int shift);
void dijkstra_solver(int size_x, int size_y, int* maze, int start, int end, int world_size, int world_rank);
#endif