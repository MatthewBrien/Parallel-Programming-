#define N 16 //Currently the only way to change N is to redefine and recompile.
#include <stdio.h>
#include <time.h>
#include "mpi.h"
#include <stdlib.h>


//function takes an array and length, and adds a second dimension, only works for square arrays.
void create_array(double** array){
	int i;
	for(i =0; i<N;i++){
		array[i]=(double*) malloc(N*8);
	//	printf("i is %d  at %p\n", i, array[i]);
	}
//	printf("\n");
}

void print_array(double array[N][N]){
	int i, j;
	for(i=0;i<N;i++){
		for( j=0; j<N;j++){
			printf("%d ", (int) array[i][j]);
		}
		printf("\n");
	}
}



int main(int argc, char *argv[])	{
	int i, j, k, error, P, rank, size, sum, blksz = 0;
	static double A[N][N], B[N][N], C[N][N], D[N][N];  // arrays declared statically so as not on the stack
  static double a[N][N], b[N][N], c[N][N], d[N][N];



	double time1, time2, time3, time4; 		// for timing
	char machine_name[256];
	MPI_Status status; 		// MPI variables

	MPI_Init(&argc, &argv);  		// Start MPI
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &P);



	int Z = N%P; //number of rows remaining
  blksz = (int) N/P; //size of each block


if (rank == 0) {
	printf("Block size is %d\n",(N/P));
	printf("Remaining rows = %d\n", Z);
	printf("Remainder after scatter is %d elements\n", (blksz*Z));
		printf ("N = %d P = %d\n",N, P);
		for (i = 0; i < N; i++) {   	// set some initial values for A and B
			for (j = 0; j < N; j++) {
				A[i][j] = j*1;
				B[i][j] = i*j+2;
			}
		}
			time3=MPI_Wtime(); //get time before parallel section starts
	}

	//block size is the number of rows. It is  the size of the array, divided by p
	MPI_Scatter(A, blksz*N, MPI_DOUBLE, a, blksz*N, MPI_DOUBLE, 0, MPI_COMM_WORLD);// Scatter input matrix A
	MPI_Bcast(B, N*N, MPI_DOUBLE, 0, MPI_COMM_WORLD); // Broadcast the input matrix B

	for(i =0 ; i < blksz; i++) {
		for(j = 0 ; j < N ; j++) {
			sum = 0;
			for(k = 0 ; k < N ; k++) {
				sum += a[i][k] * B[k][j];
			}
			c[i][j] = sum;
		}
	}

	MPI_Gather(c, blksz*N, MPI_DOUBLE, C, blksz*N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
//make rank 0 complete the Remainder

if(rank==0){
		for(i = N-Z ; i < N; i++) {
			for(j = 0 ; j < N ; j++) {
				sum = 0;
				for(k = 0 ; k < N ; k++) {
					sum += A[i][k] * B[k][j];
				}
				C[i][j] = sum;
			}
		}


}
  if(rank == 0){
    time4 = MPI_Wtime(); // record time 2



printf("Performing Sequential Calculation\n");
if(rank == 0){
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
		printf("Sequential time = %lf (seconds)\n", time2-time1);
		//print_array(D);

}
    int error = 0;
    for(i = 0; i<N;i++){
      for(j=0; j<N;j++){
        if ( (C[i][j] - D[i][j] > 0.001) || (D[i][j] - C[i][j] > 0.001)) error = -1;
      }
    }
    if(error == -1){
      printf("Checking the matrices when suddenly, STRANGER DANGER! \n");
			//print if there is an error, so that I can find where the heck it is
							print_array(D);
							printf("-------------------------------------\n");
							print_array(C);

    }
    else{
      printf("Parallel time = %lf (seconds)\n", time4-time3);
				printf("Speedup for %d cores was %lf\n", P, ((time2-time1)/(time4-time3)));

    }
  }

MPI_Finalize();
return(0);
}
