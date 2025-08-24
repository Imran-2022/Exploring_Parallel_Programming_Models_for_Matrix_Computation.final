// MPI collective operations using appropriate patterns such as MPI_Bcast, MPI_Scatter, MPI_Gather, MPI_Reduce etc.

#include <mpi.h>
#include <iostream>
#include <vector>
#include <cstdlib>

int main(int argc, char* argv[]) {

    MPI_Init(&argc, &argv);

    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int N = 4;  
    if (world_rank == 0 && argc > 1) {
        N = std::atoi(argv[1]);
    }

    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int rows_per_proc = N / world_size;  
    int elems_per_proc = rows_per_proc * N;

    std::vector<int> A, B, C;  
    if (world_rank == 0) {
        A.resize(N * N, 1);   
        B.resize(N * N, 2);   
        C.resize(N * N);
    }

    std::vector<int> localA(elems_per_proc);
    std::vector<int> localB(elems_per_proc);
    std::vector<int> localC(elems_per_proc);

    MPI_Scatter(A.data(), elems_per_proc, MPI_INT,
        localA.data(), elems_per_proc, MPI_INT,
        0, MPI_COMM_WORLD);

    MPI_Scatter(B.data(), elems_per_proc, MPI_INT,
        localB.data(), elems_per_proc, MPI_INT,
        0, MPI_COMM_WORLD);

    for (int i = 0; i < elems_per_proc; i++) {
        localC[i] = localA[i] + localB[i];
    }

    MPI_Gather(localC.data(), elems_per_proc, MPI_INT,
        C.data(), elems_per_proc, MPI_INT,
        0, MPI_COMM_WORLD);

    int local_sum = 0;
    for (int val : localC) local_sum += val;

    int global_sum = 0;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (world_rank == 0) {
        std::cout << "Matrix size: " << N << "x" << N << "\n";
        std::cout << "First row of result matrix C: ";
        for (int j = 0; j < N; j++) std::cout << C[j] << " ";
        std::cout << "\nGlobal sum of all elements in C = " << global_sum << "\n";
    }

    MPI_Finalize();
    return 0;
}
