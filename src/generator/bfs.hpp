#ifndef BFS_HPP
#define BFS_HPP
#include <iostream>
#include <cstdlib>
#include <random>
#include <vector>
#include <cmath>
#include <ctime>
#include <algorithm>

using namespace std;

vector<int> find_neighbors(int cell,int size_x,int size_y);
bool check_neighbors(int cell1,int cell2,int size_x,int size_y);
void bfs_helper(int* maze,int size_x,int size_y,int root);
void bfs(int size_x,int size_y,int end,int correct_edge,int* maze);
#endif