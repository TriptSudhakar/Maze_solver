#include "kruskal.hpp"

// DSU data structure
// Path compression + Union by rank
DSU::DSU(int n)
{
    parent = new int[n];
    rank = new int[n];
    for(int i=0;i<n;i++)
    {
        parent[i] = -1;
        rank[i] = 1;
    }
}
// Find function
int DSU::find(int i)
{
    // base case
    if(parent[i]==-1)
    {
        return i;
    }

    return parent[i] = find(parent[i]);
}
// Unite (union)
void DSU::unite(int x,int y)
{
    int s1 = find(x);
    int s2 = find(y);

    if(s1!=s2)
    {
        // Union by rank optimization
        if(rank[s1]<rank[s2])
        {
            parent[s1] = s2;
            rank[s2] += rank[s1];
        }

        else
        {
            parent[s2] = s1;
            rank[s1] += rank[s2];
        }
    }
}

void kruskal(int n, int m, int* grid)
{
    vector<pair<int,int>> edges;
    int x = n/2, y = m/2;
    for(int i = 0; i < x; i++)
    {
        for(int j = 0; j < y; j++)
        {
            if(i > 0) edges.push_back({i*y+j, (i-1)*y+j});
            if(i < x-1) edges.push_back({(i+1)*y+j, i*y+j});
            if(j > 0) edges.push_back({i*y+j, i*y+j-1});
            if(j < y-1) edges.push_back({i*y+j+1, i*y+j});
        }
    }

    std::random_device rd;
    std::mt19937 rng(rd());
    shuffle(edges.begin(), edges.end(), rng);

    DSU vertices(x*y);
    vector<pair<int,int>> tree;
    for(auto &edge : edges)
    {
        auto a = edge.first, b = edge.second;
        if(vertices.find(a) != vertices.find(b))
        {
            vertices.unite(a, b);
            tree.push_back({a, b});
        }
    }

    for(int i = 0; i <n; i++)
    {
        for(int j = 0; j < m; j++)
        {
            if(i%2 == 0 && j%2 == 0) grid[i*m+j] = 1;
            else grid[i*m+j] = 0;
        }
    }

    for(auto &edge : tree)
    {
        auto a = edge.first, b = edge.second;
        int c = max(a, b);
        int d = min(a, b);
        if(c/x == d/x)
        {
            int row = 2*(d/x);
            int col = 2*(d%x)+1;
            grid[row*m+col] = 1;
        }
        else
        {
            int row = 2*(d/x)+1;
            int col = 2*(d%x);
            grid[row*m+col] = 1;
        }
    }
}

// int main()
// {
//     int n = 16;
//     int* grid = (int*) malloc(n*n*sizeof(int)); 
//     kruskal(n, n, grid);
    
//     for(int i = 0; i < n; i++)
//     {
//         for(int j = 0; j < n; j++)
//         {
//             if(i == 0 && j == n-1) cout<<"S";
//             else if(i == n-1 && j == 0) cout<<"E";
//             else if(grid[i*n+j] == 0) cout<<"*";
//             else if(grid[i*n+j] == 1) cout<<"_";
//         }
//         cout << endl;
//     }
//     return 0;
// }