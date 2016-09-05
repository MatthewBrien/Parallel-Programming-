// Suzaku program for matrix multiplication with low level routines SZ_matrixmult.c
//B. Wilkinson Dec 29, 2015

#define N 256
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "suzaku.h"  		// Suzaku routines

void print_results(char *prompt, double a[N][N]) {  // available to print out arrays for checking, not currently used below
	int i, j;
	printf ("\n\n%s\n", prompt);
	for (i = 0; i< N; i++) {
		for (j = 0; j < N; j++) {
			printf(" %.2lf", a[i][j]);
		}
		printf ("\n");
	}
	printf ("\n\n");
}

int main(int argc, char *argv[])	{

	int i, j, k, error = 0; 	// All variables declared here are in every process
	double A[N][N], B[N][N], C[N][N], D[N][N], sum;
	
	double time1, time2; 		// for timing
	int P; 				// P, number of processes
	int blksz;  			// used to define blocksize in matrix multiplication

	SZ_Init(P);			// this initializes MPI environment
					// just master process after this
	if (N % P != 0) {
		printf("Error -- N/P must be an integer\n");
	} 

	for (i = 0; i < N; i++) {   	// set some initial values for A and B
		for (j = 0; j < N; j++) {
			A[i][j] = j*1;
			B[i][j] = i*j+2;
		}
	}

	for (i = 0; i < N; i++) {   	// sequential matrix multiplication
		for (j = 0; j < N; j++)   {
			sum = 0;
			for (k=0; k < N; k++) {
				sum += A[i][k]*B[k][j]; 
			}      
			D[i][j] = sum;
		}
	}

	time1 = SZ_Wtime(); 	// record  time stamp

	SZ_Parallel_begin

	blksz = N/P;

	double A1[blksz][N]; 		// used in slaves to hold scattered a
	double C1[blksz][N]; 		// used in slaves to hold their result

	SZ_Scatter(A,A1);  	// Scatter A array into A1

	SZ_Broadcast(B);  	// broadcast B array

	for(i = 0 ; i < blksz; i++) {
		for(j = 0 ; j < N ; j++) {
			sum = 0;
			for(k = 0 ; k < N ; k++) {
				sum += A1[i][k] * B[k][j];
			}
			C1[i][j] = sum;
		}
	}

	SZ_Gather(C1,C);	// gather results

	SZ_Parallel_end; 	// end of parallel, back to just master, note a barrier here

	time2 = SZ_Wtime(); 	// record  time stamp

	int error = 0;		// check sequential and parallel versions same answers, within rounding
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++)   {
			if ( (C[i][j] - D[i][j] > 0.001) || (D[i][j] - C[i][j] > 0.001)) error = -1;
		}
	}
	if (error == -1) printf("ERROR, sequential and parallel code give different answers.\n");
	else printf("Sequential and parallel code give same answers.\n");

	printf("elapsed_time = %f (seconds)\n", time2 - time1);  // print out execution time

	SZ_Finalize(); 
	return 0;
}
