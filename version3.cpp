// Point to point bidirectional communication between processes using send_msg and recv_msg(or equivalent MPI primitives like MPI_Send and MPI_Recv)

#include <iostream>
#include <vector>
#include <chrono>
#include <mpi.h>

using namespace std;
using namespace std::chrono;

#define MATRIX_SIZE 500

int main(int argc, char* argv[]) {
    int num_processes, rank;
    
    MPI_Init(&argc, &argv);
    
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    int rows_per_process = MATRIX_SIZE / num_processes;
    int chunk_size = rows_per_process * MATRIX_SIZE;
    
    high_resolution_clock::time_point start_time;
    if (rank == 0) {
        start_time = high_resolution_clock::now();
    }
    
    int *A = new int[MATRIX_SIZE * MATRIX_SIZE];
    int *B = new int[MATRIX_SIZE * MATRIX_SIZE];
    int *C = new int[MATRIX_SIZE * MATRIX_SIZE];
    
    int *chunk_A = new int[chunk_size];
    int *chunk_B = new int[chunk_size];
    int *chunk_C = new int[chunk_size];
    
    if (rank == 0) {

        for (int i = 0; i < MATRIX_SIZE * MATRIX_SIZE; ++i) {
            A[i] = 1;
            B[i] = 2;
        }
        
        for (int i = 1; i < num_processes; ++i) {
            int start_row = i * rows_per_process;
            int start_index = start_row * MATRIX_SIZE;
            
            MPI_Send(&A[start_index], chunk_size, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&B[start_index], chunk_size, MPI_INT, i, 1, MPI_COMM_WORLD);
        }
        
        for (int i = 0; i < rows_per_process * MATRIX_SIZE; ++i) {
            C[i] = A[i] + B[i];
        }
        
        for (int i = 1; i < num_processes; ++i) {
            int start_row = i * rows_per_process;
            int start_index = start_row * MATRIX_SIZE;
            
            MPI_Recv(&C[start_index], chunk_size, MPI_INT, i, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        
        auto end_time = high_resolution_clock::now();
        duration<double> duration = end_time - start_time;
        
        cout << "Matrix C (Result of A + B) - MPI Version" << endl;
        cout << "Time taken: " << duration.count() << " seconds" << endl;
        cout << "C[0][0] = " << C[0] << endl;
        cout << "C[MATRIX_SIZE-1][MATRIX_SIZE-1] = " << C[MATRIX_SIZE*MATRIX_SIZE - 1] << endl;
        
    } else {

        MPI_Recv(chunk_A, chunk_size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(chunk_B, chunk_size, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        for (int i = 0; i < chunk_size; ++i) {
            chunk_C[i] = chunk_A[i] + chunk_B[i];
        }

        MPI_Send(chunk_C, chunk_size, MPI_INT, 0, 2, MPI_COMM_WORLD);
    }
    
    delete[] A;
    delete[] B;
    delete[] C;
    delete[] chunk_A;
    delete[] chunk_B;
    delete[] chunk_C;
    
    MPI_Finalize();
    
    return 0;
}
