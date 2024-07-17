#ifndef DFS_HPP
#define DFS_HPP
#include <iostream>
#include <cstdlib>
#include <random>
#include <vector>
#include <cmath>
#include <ctime>
#include <algorithm>
#include <mpi.h>

using namespace std;

vector<int> find_neighbors_dfs(int cell,int size_x,int size_y,int* maze);
pair<pair<bool,int>,pair<int,int>> team_meeting_dfs(int* parents,int size_x,int size_y,vector<int> &stack,int end,int world_size,int world_rank,int shift);
void dfs_solver(int size_x,int size_y,int *maze,int start,int end,int world_size,int world_rank);
#endif