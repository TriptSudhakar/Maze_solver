#include "mazegenerator.hpp"

void kruskal_generator(int n, int* grid, int comm_sz, int rank)
{   
    int p = sqrt(comm_sz);
    int gridDim = n/p;

    if(rank == 0)
    {
        int* arr;
        int* final;
        arr = (int*) malloc(gridDim*gridDim*sizeof(int));
        for(int j = 0; j < gridDim*gridDim; j++) arr[j] = 0;

        kruskal(gridDim, gridDim, arr);
        // for(int i = 1; i < comm_sz; i++)
        // {
        //     MPI_Recv(arr[i], gridDim*gridDim, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        // }

        final= (int*) malloc(comm_sz*gridDim*gridDim*sizeof(int));
        MPI_Gather(arr, gridDim*gridDim, MPI_INT, final, gridDim*gridDim, MPI_INT, 0, MPI_COMM_WORLD);     


        
        
        for(int i = 0; i < comm_sz; i++)
        {
            int x = i/p;
            int y = i%p;
            for(int j = 0; j < gridDim; j++)
            {
                for(int k = 0; k < gridDim; k++)
                {
                    grid[(x*gridDim+j)*n + y*gridDim+k] = final[i*gridDim*gridDim+j*gridDim+k];
                }
            }
        }

        vector<pair<int,int>> edges;
        int x = p, y = p;
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

        for(auto &edge : tree)
        {
            auto a = edge.first, b = edge.second;
            int c = max(a, b);
            int d = min(a, b);
            srand(time(0));
            int r = 2*(rand()%(gridDim/2));

            if(c/x == d/x) grid[((c/x)*gridDim+r)*n + (c%x)*gridDim-1] = 1;
            else grid[((c/x)*gridDim-1)*n + (c%x)*gridDim+r] = 1;
        }

        grid[n-1] = 1;
        grid[(n-1)*n] = 1;

        int curr=n-1;
        while(curr<n*n)
        {
            int count=0;
            if(curr%n>0&&grid[curr-1]==1)
                count++;
            if(curr%n<n-1&&grid[curr+1]==1)
                count++;
            if(curr/n>0&&grid[curr-n]==1)
                count++;
            if(curr/n<n-1&&grid[curr+n]==1)
                count++;
            if(count==1)
            {
                int r=rand()%2;
                if(r==0)
                {
                    grid[curr]=1;
                }
            }
            curr+=n;
        }
        curr=n*n-n;
        while(curr<n*n)
        {
            int count=0;
            if(curr%n>0&&grid[curr-1]==1)
                count++;
            if(curr%n<n-1&&grid[curr+1]==1)
                count++;
            if(curr/n>0&&grid[curr-n]==1)
                count++;
            if(curr/n<n-1&&grid[curr+n]==1)
                count++;
            if(count==1)
            {
                int r=rand()%2;
                if(r==0)
                {
                    grid[curr]=1;
                }
            }
            curr++;
        }
    }
    else
    {
        int* arr = (int*) malloc(gridDim*gridDim*sizeof(int));
        for(int i = 0; i < gridDim*gridDim; i++) arr[i] = 0;
        kruskal(gridDim, gridDim, arr);
        // MPI_Send(arr, gridDim*gridDim, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Gather(arr, gridDim*gridDim, MPI_INT, NULL, 0, MPI_INT, 0, MPI_COMM_WORLD);
    }
}

void bfs_generator(int n, int* grid, int comm_sz, int rank)
{
    int p = sqrt(comm_sz);
    int gridDim = n/p;

    if(rank == 0)
    {
        int* arr;
        int *final;
        // for(int i = 0; i < comm_sz; i++)
        // {
        arr = (int*) malloc(gridDim*gridDim*sizeof(int));
        for(int j = 0; j < gridDim*gridDim; j++) arr[j] = 0;
        // }

        bfs(gridDim, gridDim, -1, 0, arr);
        // for(int i = 1; i < comm_sz; i++)
        // {
        //     MPI_Recv(arr[i], gridDim*gridDim, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        // }

        final= (int*) malloc(comm_sz*gridDim*gridDim*sizeof(int));
        MPI_Gather(arr, gridDim*gridDim, MPI_INT, final, gridDim*gridDim, MPI_INT, 0, MPI_COMM_WORLD);
        
        for(int i = 0; i < comm_sz; i++)
        {
            int x = i/p;
            int y = i%p;
            for(int j = 0; j < gridDim; j++)
            {
                for(int k = 0; k < gridDim; k++)
                {
                    grid[(x*gridDim+j)*n + y*gridDim+k] = final[i*gridDim*gridDim+j*gridDim+k];
                }
            }
        }

        vector<pair<int,int>> edges;
        int x = p, y = p;
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

        for(auto &edge : tree)
        {
            auto a = edge.first, b = edge.second;
            int c = max(a, b);
            int d = min(a, b);
            srand(time(0));
            int r = 2*(rand()%(gridDim/2));

            if(c/x == d/x) grid[((c/x)*gridDim+r)*n + (c%x)*gridDim-1] = 1;
            else grid[((c/x)*gridDim-1)*n + (c%x)*gridDim+r] = 1;
        }
        
        grid[n-1] = 1;

        int curr=n-1;
        while(curr<n*n)
        {
            int count=0;
            if(curr%n>0&&grid[curr-1]==1)
                count++;
            if(curr%n<n-1&&grid[curr+1]==1)
                count++;
            if(curr/n>0&&grid[curr-n]==1)
                count++;
            if(curr/n<n-1&&grid[curr+n]==1)
                count++;
            if(count==1)
            {
                int r=rand()%2;
                if(r==0)
                {
                    grid[curr]=1;
                }
            }
            curr+=n;
        }
        curr=n*n-n;
        while(curr<n*n)
        {
            int count=0;
            if(curr%n>0&&grid[curr-1]==1)
                count++;
            if(curr%n<n-1&&grid[curr+1]==1)
                count++;
            if(curr/n>0&&grid[curr-n]==1)
                count++;
            if(curr/n<n-1&&grid[curr+n]==1)
                count++;
            if(count==1)
            {
                int r=rand()%2;
                if(r==0)
                {
                    grid[curr]=1;
                }
            }
            curr++;
        }
    }
    else
    {
        int* arr = (int*) malloc(gridDim*gridDim*sizeof(int));
        for(int i = 0; i < gridDim*gridDim; i++) arr[i] = 0;
        if(rank == p*(p-1)) bfs(gridDim, gridDim, gridDim*(gridDim-1), 0, arr);
        else bfs(gridDim, gridDim, -1, 0, arr);
        // MPI_Send(arr, gridDim*gridDim, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Gather(arr, gridDim*gridDim, MPI_INT, NULL, 0, MPI_INT, 0, MPI_COMM_WORLD);
    }
}

// int main()
// {
//     int comm_sz;
//     int rank;
//     int n = 64;
//     int* grid = (int*) malloc(n*n*sizeof(int));
//     for(int i = 0; i < n*n; i++) grid[i] = 0;

//     MPI_Init(NULL, NULL);
//     MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
//     MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//     int p = sqrt(comm_sz);
//     // kruskal_generator(n, p, grid, rank);
//     bfs_generator(n, p, grid, rank);
//     MPI_Barrier(MPI_COMM_WORLD);

//     if(rank == 0)
//     {
//         for(int i = 0; i < n; i++)
//         {
//             for(int j = 0; j < n; j++)
//             {
//                 if(i == 0 && j+1 == n) cout<<"S";
//                 else if(i+1 == n && j == 0) cout<<"E";
//                 else if(grid[i*n+j] == 0) cout<<"*";
//                 else if(grid[i*n+j] == 1) cout<<"_";
//             }
//             cout << endl;
//         }
//     }
//     MPI_Finalize();
//     return 0;
// }