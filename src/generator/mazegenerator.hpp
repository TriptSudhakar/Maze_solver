#ifndef MAZEGENERATOR_HPP
#define MAZEGENERATOR_HPP
#include "kruskal.hpp"
#include "bfs.hpp"
#include <mpi.h>

void kruskal_generator(int n, int* grid, int comm_sz, int rank);
void bfs_generator(int n, int* grid, int comm_sz, int rank);
#endif