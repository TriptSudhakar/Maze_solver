#include "bfs.hpp"
// define a function to create a 64*64 maze using BFS algorithm

// return neighbors of a cell
vector<int> find_neighbors(int cell,int size_x,int size_y)
{
    vector<int> n;
    if(cell%size_y>1)
        n.push_back(cell-2);
    if(cell%size_y<size_y-2)
        n.push_back(cell+2);
    if(cell/size_y>1)
        n.push_back(cell-size_y*2);
    if(cell/size_y<size_x-2)
        n.push_back(cell+size_y*2);
    return n;
}

bool check_neighbors(int cell1,int cell2,int size_x,int size_y)
{
    if(abs(cell1-cell2)==2 || abs(cell1-cell2)==size_y*2)
        return true;
    else
        return false;
}

void bfs_helper(int* maze,int size_x,int size_y,int root)
{
    vector<int> queue,new_q;
    queue.push_back(root);
    maze[root]=1;
    vector<int> neighbors,possible_neighbors,temp_neighbors;
    while(queue.size()>0)
    {
        new_q.clear();
        int size=queue.size();
        std::random_device rd;
        std::mt19937 g(rd());
        shuffle(queue.begin(),queue.end(),g);
        for(int i=0;i<size;i++)
        {
            int current=queue[i];
            neighbors=find_neighbors(current,size_x,size_y);
            possible_neighbors.clear();
            for(int i=0;i<neighbors.size();i++)
            {
                if(maze[neighbors[i]]==0)
                {
                    new_q.push_back(neighbors[i]);
                    maze[neighbors[i]]=1;
                    maze[(current+neighbors[i])/2]=1;
                }
            }
        }
        queue.swap(new_q);
    }
}

// function to create a maze using BFS algorithm and return the maze as pointer to array

void bfs(int size_x,int size_y,int end,int correct_edge,int* maze)
{

    for(int i=0;i<size_x*size_y;i++)
    {
        maze[i]=0;
    }

    srand(time(0));
    int r_x=2*(rand()%((int)(size_x/2)));
    int r_y=2*(rand()%((int)(size_y/2)));
    int r=r_x*size_y+r_y;

    bfs_helper(maze,size_x,size_y,r);

    if(end!=-1){
        if(end%size_y==size_y-1 && maze[end]==0)
        {
            maze[end]=1;
            int curr=end;
            while(maze[curr-1]!=1 && curr<size_x*size_y)
            {
                curr+=size_y;
                maze[curr]=1;
            }
            if(curr>=size_x*size_y)
            {
                curr=end;
                while(maze[curr-1]!=1 && curr>size_y)
                {
                    curr-=size_y;
                    maze[curr]=1;
                }
            }       
        }

        if(end/size_y==size_x-1 && maze[end]==0)
        {
            maze[end]=1;
            int curr=end;
            while(maze[curr-size_y]!=1 && curr<size_x*size_y)
            {
                curr++;
                maze[curr]=1;
            }
            if(curr>=size_x*size_y)
            {
                curr=end;
                while(maze[curr-size_y]!=1 && curr%size_y>0)
                {
                    curr--;
                    maze[curr]=1;
                }
            }       
        }
    }

    if((correct_edge==1||correct_edge==3) && size_y%2==0)
    {
        int curr=size_y-1;
        while(curr<size_x*size_y)
        {
            int count=0;
            if(curr%size_y>0&&maze[curr-1]==1)
                count++;
            if(curr%size_y<size_y-1&&maze[curr+1]==1)
                count++;
            if(curr/size_y>0&&maze[curr-size_y]==1)
                count++;
            if(curr/size_y<size_x-1&&maze[curr+size_y]==1)
                count++;
            if(count==1)
            {
                r=rand()%2;
                if(r==0)
                {
                    maze[curr]=1;
                }
            }
            curr+=size_y;
        }
    }

    if((correct_edge==2||correct_edge==3) && size_x%2==0)
    {
        int curr=size_x*size_y-size_y;
        while(curr<size_x*size_y)
        {
            int count=0;
            if(curr%size_y>0&&maze[curr-1]==1)
                count++;
            if(curr%size_y<size_y-1&&maze[curr+1]==1)
                count++;
            if(curr/size_y>0&&maze[curr-size_y]==1)
                count++;
            if(curr/size_y<size_x-1&&maze[curr+size_y]==1)
                count++;
            if(count==1)
            {
                r=rand()%2;
                if(r==0)
                {
                    maze[curr]=1;
                }
            }
            curr++;
        }
    }
                
}

// int main()
// {
//     int size = 8;
//     int maze[size*size];
//     bfs(size,size,size*size-1,3,maze);

//     for (int i = 0; i < size; ++i) {
//         for (int j = 0; j < size; ++j) {
//             if (i == 0 && j == 0)
//                 cout << "S";
//             else if (i == size - 1 && j == size - 1)
//                 cout << "E";
//             else if (maze[i * size + j] == 1)
//                 cout << "_";
//             else
//                 cout << "*";
//         }
//         cout << endl;
//     }

//     return 0;
// }