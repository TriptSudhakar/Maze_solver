#include "dijkstra.hpp"

vector<int> find_neighbors(int cell,int size_x,int size_y,int* maze)
{
    vector<int> n;
    if(cell%size_y>0 && maze[cell-1]==1)
        n.push_back(cell-1);
    if(cell%size_y<size_y-1 && maze[cell+1]==1)
        n.push_back(cell+1);
    if(cell/size_y>0 && maze[cell-size_y]==1)
        n.push_back(cell-size_y);
    if(cell/size_y<size_x-1 && maze[cell+size_y]==1)
        n.push_back(cell+size_y);
    return n;
}

pair<pair<bool,int>,pair<int,int>> team_meeting(int* distances,int size_x,int size_y,vector<int> &stack,int end,int world_size,int world_rank,int shift)
{
    // MPI_Barrier(MPI_COMM_WORLD);
    bool done=(distances[end]!=size_x*size_y+2);
    // if(done){
    //     cout<<"Process "<<world_rank<<" found the path"<<endl;
    // }
    int size=stack.size()-shift;
    MPI_Allreduce(MPI_IN_PLACE,&done,1,MPI_C_BOOL,MPI_LOR,MPI_COMM_WORLD);
    if(done){
        return {{true,0},{-1,-1}};
    }

    int sizes[world_size];
    int shift_change=0;
    int value=-1;
    int distance=-1;

    MPI_Allgather(&size,1,MPI_INT,sizes,1,MPI_INT,MPI_COMM_WORLD);
    // cout<<sizes[0]<<" "<<sizes[1]<<" "<<sizes[2]<<" "<<sizes[3]<<endl;
    int min_size=size,max_size=size;
    for(int i=0;i<world_size;i++){
        if(sizes[i]<min_size){
            min_size=sizes[i];
        }
        if(sizes[i]>max_size){
            max_size=sizes[i];
        }
    }
    if(min_size>0||max_size==1){
        return {{false,0},{-1,-1}};
    }
    MPI_Allreduce(MPI_IN_PLACE,distances,size_x*size_y,MPI_INT,MPI_MIN,MPI_COMM_WORLD);
    if(size==0){
        //receive stack from any other process
        int max_size=0;
        for(int i=0;i<world_size;i++){
            if(sizes[i]>max_size){
                max_size=sizes[i];
            }
        }
        if(max_size>1){
            int producers=0;
            int my_rank=0;
            for(int i=0;i<world_rank;i++){
                if(sizes[i]==0){
                    my_rank++;
                }
            }
            for(int i=0;i<world_size;i++){
                if(sizes[i]>1){
                    producers++;
                }
            }
            if(producers>my_rank){
                MPI_Status status;
                // cout<<"Porcess "<<world_rank<<" is waiting"<<endl;
                MPI_Recv(&value,1,MPI_INT,MPI_ANY_SOURCE,0,MPI_COMM_WORLD,&status);
                // MPI_Recv(&distance,1,MPI_INT,status.MPI_SOURCE,0,MPI_COMM_WORLD,&status);
                // distances[value]=distance;
                // cout<<"Porcess "<<world_rank<<" received "<<value<<" with distance "<<distance<<" from "<<status.MPI_SOURCE<<endl;
                stack.push_back(value);
            }
        }
    }
    else if(size>1){
        // cout<<"Process "<<world_rank<<" can produce"<<endl;
        int my_rank=0;
        for(int i=0;i<world_rank;i++){
            if(sizes[i]>1){
                my_rank++;
            }
        }
        int my_receiver=-1;
        for(int i=0;i<world_size;i++){
            if(sizes[i]==0){
                if(my_rank==0){
                    my_receiver=i;
                    break;
                }
                my_rank--;
            }
        }
        // cout<<"Process "<<world_rank<<" my_rank: "<<my_rank<<" my_receiver: "<<my_receiver<<endl;
        if(my_receiver!=-1){
            //send front element of stack to my_receiver
            value=stack[shift];
            shift_change=1;
            // cout<<"Porcess "<<world_rank<<" sending "<<value<<" to "<<my_receiver<<endl;
            MPI_Send(&value,1,MPI_INT,my_receiver,0,MPI_COMM_WORLD);
            distance=distances[value];
            // MPI_Send(&distance,1,MPI_INT,my_receiver,0,MPI_COMM_WORLD);
            // cout<<"Porcess "<<world_rank<<" sent "<<value<<" to "<<my_receiver<<endl;
            value=-1;
        }
            
    }
    // cout<<"Process "<<world_rank<<" leaving"<<endl;
    return {{false,shift_change},{value,distance}};
}

void dijkstra_solver(int size_x, int size_y, int* maze, int start, int end, int world_size, int world_rank) 
{

    // int size_x=64;
    // int size_y=64;
    // int maze[size_x*size_y];
    // int end;
    int freq=3;
    vector<int> stack;
    int distances[size_x*size_y];
    for(int i=0;i<size_x*size_y;i++)
    {
        distances[i]=size_x*size_y+1;
    }

    // //initialize MPI
    // MPI_Init(NULL,NULL);
    // int world_size;
    // MPI_Comm_size(MPI_COMM_WORLD,&world_size);
    // int world_rank;
    // MPI_Comm_rank(MPI_COMM_WORLD,&world_rank);

    if(world_rank==0){
        // ifstream input("input.txt");
        // if(!input.is_open())
        // {
        //     cout<<"File not found"<<endl;
        //     return 0;
        // }
        // int start=-1;
        // for(int i=0;i<size_x*size_y;i++)
        // {
        //     char c;
        //     input>>c;
        //     if(c=='*')
        //     {
        //         maze[i]=0;
        //     }
        //     else if(c=='_' || c=='S' || c=='E')
        //     {
        //         maze[i]=1;
        //         if(c=='S')
        //             start=i;
        //         else if(c=='E')
        //             end=i;
        //     }
        //     else
        //     {
        //         i--;
        //     }
        // }
        // input.close();

        //send maze to all processes
        MPI_Bcast(maze,size_x*size_y,MPI_INT,0,MPI_COMM_WORLD);
        //send end to all processes
        MPI_Bcast(&end,1,MPI_INT,0,MPI_COMM_WORLD);
        distances[start]=0;
        stack.push_back(start);
        distances[end]=size_x*size_y+2;
        int current;
        vector<int> neighbors;
        int i=1;
        bool done=false;
        pair<pair<bool,int>,pair<int,int> > result;
        int shift=0;
        while(!done)
        {
            if(i%freq==0){
                result=team_meeting(distances,size_x,size_y,stack,end,world_size,world_rank,shift);
                if(result.first.first)
                    break;
                shift=shift+result.first.second;
                // if(result.second.first!=-1){
                //     // cout<<"Porcess "<<world_rank<<" received "<<result.second.first<<" with distance "<<result.second.second<<endl;
                //     distances[result.second.first]=result.second.second;
                // }
            }
            if(stack.size()-shift>0){
                // cout<<"Stack: "<<stack.size()-shift<<endl;
                int s_size=stack.size();
                for(;shift<s_size;shift++)
                {    
                    current=stack[shift];
                    if(current==end){
                        i++;
                        continue;
                    }
                    neighbors=find_neighbors(current,size_x,size_y,maze);
                    for(int i=0;i<neighbors.size();i++)
                    {
                        if(distances[neighbors[i]]<=distances[current]-1)
                            continue;
                        stack.push_back(neighbors[i]);
                        distances[neighbors[i]]=distances[current]+1;
                    }
                }
                // for(int i=shift;i<stack.size();i++)
                // {
                //     cout<<stack[i]<<" ";
                // }
                // cout<<endl;
            }
            i++;
        }
        MPI_Allreduce(MPI_IN_PLACE,distances,size_x*size_y,MPI_INT,MPI_MIN,MPI_COMM_WORLD);
        if(distances[end]==-2)
        {
            cout<<"No path found"<<endl;
        }
        else
        {
            // cout<<"Path found"<<endl;
            vector<int> path;
            current=end;
            while(distances[current]!=0)
            {
                // cout<<current<<endl;
                path.push_back(current);
                neighbors=find_neighbors(current,size_x,size_y,maze);
                for(int i=0;i<neighbors.size();i++)
                {
                    if(distances[neighbors[i]]<distances[current])
                    {
                        current=neighbors[i];
                        break;
                    }
                }
            }
            // ofstream output("output.txt");
            for (int i = 0; i < size_x; ++i) {
                for (int j = 0; j < size_y; ++j) {
                    if (i*size_y+j == start)
                        cout<< "S";
                    else if (i*size_y+j == end)
                        cout<< "E";
                    else if (find(path.begin(),path.end(),i*size_y+j)!=path.end())
                        cout<< "P";
                    else if (maze[i * size_y + j] == 1)
                        cout<< " ";
                    else
                        cout<< "*";
                }
                cout<< std::endl;
            }
            // output.close();
        }
    }
    else{
        //receive maze from process 0
        MPI_Bcast(maze,size_x*size_y,MPI_INT,0,MPI_COMM_WORLD);
        //receive end from process 0
        MPI_Bcast(&end,1,MPI_INT,0,MPI_COMM_WORLD);

        distances[end]=size_x*size_y+2;
        int current;
        vector<int> neighbors;
        int i=1;
        bool done=false;
        pair<pair<bool,int>,pair<int,int> > result;
        int shift=0;
        while(!done)
        {
            if(i%freq==0){
                result=team_meeting(distances,size_x,size_y,stack,end,world_size,world_rank,shift);
                if(result.first.first)
                    break;
                shift=shift+result.first.second;
                // if(result.second.first!=-1){
                //     // cout<<"Process "<<world_rank<<" received "<<result.second.first<<" with distance "<<result.second.second<<endl;
                //     distances[result.second.first]=result.second.second;
                // }
            }
            if(stack.size()-shift>0){
                int s_size=stack.size();
                for(;shift<s_size;shift++){
                    current=stack[shift];
                    if(current==end){
                        i++;
                        continue;
                    }
                    neighbors=find_neighbors(current,size_x,size_y,maze);
                    for(int i=0;i<neighbors.size();i++)
                    {
                        if(distances[neighbors[i]]<=distances[current]-1)
                            continue;
                        stack.push_back(neighbors[i]);
                        distances[neighbors[i]]=distances[current]+1;
                    }
                }
            }
            i++;
        }
        MPI_Allreduce(MPI_IN_PLACE,distances,size_x*size_y,MPI_INT,MPI_MIN,MPI_COMM_WORLD);
    }
    MPI_Barrier(MPI_COMM_WORLD);

    // MPI_Finalize();

    // return 0;
}