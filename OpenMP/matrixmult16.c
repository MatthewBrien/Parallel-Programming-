	#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#define N 1028
#define CHUNKSIZE 10

int main(int argc, char *argv) {
	int numThreads = 16;
	omp_set_num_threads(numThreads);//set number of threads here
	printf("Executing with %d Threads \n", numThreads);

	int i, j, k, x;
	double sum;
	double start, end, avgTime = 0;			// used for timing
	static double A[N][N], B[N][N], C[N][N], D[N][N];	

	// set some initial values for A and B
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++)     {
			A[i][j] = j*1;
			B[i][j] = i*j+2;
		}
	}


	// sequential matrix multiplication
	start = omp_get_wtime(); 	//start time measurement
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++)   {
			sum = 0;
			for (k=0; k < N; k++) {
				sum += A[i][k]*B[k][j]; 
			}      
			C[i][j] = sum;
		}
	}
 	end = omp_get_wtime();	//end time measurement
	printf("Time of sequential computation: %f seconds\n", end-start);

for(int time = 0; time < 10; time++){
	start = omp_get_wtime();

	// parallel matrix multiplication here result in array D[N][N]

		
		for (i = 0; i < N; i++) {
		#pragma omp parallel shared(A, B, D) private(k, sum) 
		{
		#pragma omp for schedule(static, CHUNKSIZE)
			for (j = 0; j < N; j++)   {
				sum = 0;
				for (k=0; k < N; k++) {
					sum += A[i][k]*B[k][j]; 
				}      
				D[i][j] = sum;
			}
		}
	}
	end = omp_get_wtime();
	avgTime += end - start;
}
	printf("Average Time of parallel computation: %f seconds\n", avgTime/10);

	// check sequential and parallel versions give same answers, within rounding
	int error = 0;
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++)   {
			if ((C[i][j] - D[i][j] > 0.001) || (D[i][j] - C[i][j] > 0.001))
				error = -1;

		}
	}
	if (error == -1) printf("ERROR, sequential and parallel versions give different answers\n");

	return(0);
}

