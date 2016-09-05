#define N 256
#include <stdio.h>
#include <time.h>
#include "mpi.h"


int main(int argc, char *argv[])	{
	int i, j, k, error, P, rank, size, blksz = 0;
	static double A[N][N], B[N][N], C[N][N], D[N][N], sum;  // arrays declared statically so as not on the stack
  static double a[N][N], b[N][N], c[N][N], d[N][N];



	double time1, time2, time3, time4; 		// for timing
	char machine_name[256];
	MPI_Status status; 		// MPI variables

	MPI_Init(&argc, &argv);  		// Start MPI
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &P);


	if ((rank == 0) && (N % P != 0)) {
		printf("Error -- N/P must be an integer\n"); // should really stop now
		MPI_Abort(MPI_COMM_WORLD,1);
	}

  blksz = N/P;

if (rank == 0) {
		printf ("N = %d P = %d\n",N, P);
		for (i = 0; i < N; i++) {   	// set some initial values for A and B
			for (j = 0; j < N; j++) {
				A[i][j] = j*1;
				B[i][j] = i*j+2;
			}
		}
			time1 = MPI_Wtime(); 	// record  time stamp
		for (i = 0; i < N; i++) {   	// sequential matrix multiplication
			for (j = 0; j < N; j++)   {
				sum = 0;
				for (k=0; k < N; k++) {
					sum += A[i][k]*B[k][j];
				}
				D[i][j] = sum;
			}
		}
		time2 = MPI_Wtime(); 	// record  time stamp
		  printf("Elapsed time = %lf (seconds)\n", time2-time1);
			time3=MPI_Wtime();
	}
	MPI_Scatter(A, blksz*N, MPI_DOUBLE, A, blksz*N, MPI_DOUBLE, 0, MPI_COMM_WORLD);// Scatter input matrix A
	MPI_Bcast(B, N*N, MPI_DOUBLE, 0, MPI_COMM_WORLD); // Broadcast the input matrix B

	for(i = 0 ; i < blksz; i++) {
		for(j = 0 ; j < N ; j++) {
			sum = 0;
			for(k = 0 ; k < N ; k++) {
				sum += A[i][k] * B[k][j];
			}
			C[i][j] = sum;
		}
	}

	MPI_Gather(C, blksz*N, MPI_DOUBLE, C, blksz*N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  if(rank == 0){
    time4 = MPI_Wtime(); // record time 2

    int error = 0;
    for(i = 0; i<N;i++){
      for(j=0; j<N;j++){
        if ( (C[i][j] - D[i][j] > 0.001) || (D[i][j] - C[i][j] > 0.001)) error = -1;
      }
    }
    if(error == -1){
      printf("Checking the matrices when suddenly, STRANGER DANGER! \n");
    }
    else{
      printf("Elapsed time = %lf (seconds)\n", time4-time3);
				printf("Speedup for %d cores was %lf\n", P, ((time2-time1)/(time4-time3)));

    }
  }

MPI_Finalize();
return(0);
}
