#include "generator/mazegenerator.hpp"
#include "solver/mazesolver.hpp"

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);
    int comm_sz;
    int rank;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    string generator_option, solver_option;
    for (int i = 1; i < argc; ++i) {
        std::string arg(argv[i]);
        if (arg == "-g") {
            if (i + 1 < argc) {
                generator_option = argv[i + 1];
                i++; // Skip the option value
            } else {
                std::cerr << "Error: Missing value for -g option" << std::endl;
                MPI_Abort(MPI_COMM_WORLD, 1);
            }
        } else if (arg == "-s") {
            if (i + 1 < argc) {
                solver_option = argv[i + 1];
                i++; // Skip the option value
            } else {
                std::cerr << "Error: Missing value for -s option" << std::endl;
                MPI_Abort(MPI_COMM_WORLD, 1);
            }
        }
    }

    int n = 64;
    int* grid = (int*) malloc(n*n*sizeof(int));
    if(generator_option == "kruskal") kruskal_generator(n, grid, comm_sz, rank);
    else bfs_generator(n, grid, comm_sz, rank);
    MPI_Barrier(MPI_COMM_WORLD);

    if(solver_option == "dfs") mazesolver(n, n, grid, n-1, n*(n-1), comm_sz, rank, 0);
    else mazesolver(n, n, grid, n-1, n*(n-1), comm_sz, rank, 1);
    MPI_Finalize();
}