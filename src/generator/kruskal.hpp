#ifndef KRUSKAL_HPP
#define KRUSKAL_HPP
#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <algorithm>
using namespace std;

class DSU
{
    int *parent;
    int *rank;
    public:
    DSU(int n);
    int find(int i);
    void unite(int x,int y);
};

void kruskal(int n, int m, int* grid);
#endif