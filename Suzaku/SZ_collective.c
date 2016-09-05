// B. Wilkinson Dec 28, 2015 Testing SZ_Broadcast with different types
#include <stdio.h>
#include <string.h>
#include "suzaku.h"  			// Suzaku macros

int main(int argc, char *argv[]) {
	char message[20];
	int i,j,k, P, PID; 				//All variables declared here are in every process
	int x = 0;	
	int Y[10] = {0,0,0,0,0,0,0,0,0,0};
	double p=123;			
	double A[10] = {0,0,0,0,0,0,0,0,0,0};
	double B[2][3] = {{0,0,0},{0,0,0}};		// multidimensional can only be doubles
	strcpy(message, "------------");

	SZ_Init(P);					// initialize MPI message-passing environment, 
							// Initialize, only master does this until parallel section
	strcpy(message, "Hello world");
	x = 88;						
	for (i = 0; i < 10; i++) {
		Y[i] = i;
		A[i] = 9 - i;
	}
	p=123;
	k = 0;
	for (i = 0; i < 2; i++)
	   for (j = 0; j < 3; j++) 
		B[i][j] = k++;

	SZ_Parallel_begin				// parallel section, all processes do this
		PID = SZ_Get_process_num();  		// get process ID

		SZ_Broadcast(message);  		// broadcast a message
		if (PID == 1) printf("String, message\nReceived by process %d = %s\n",PID,message); // print it out one destination

		SZ_Broadcast(&x);  			// broadcast an int
		if (PID == 1) printf("Single integer, &x\nReceived by process %d = %d\n",PID,x); // print it out at one destination

		SZ_Broadcast(Y); 			// broadcast an array of doubles
		if (PID == 1) { 			// print it out at one destination
			printf("1-D Array of integers, Y\nReceived by process %d = ",PID);
			for (i = 0; i < 10; i++)
				printf("%2d ",Y[i]);
			printf("\n");
		}

		SZ_Broadcast(&p); 			// broadcast a double	
		if (PID == 1)  printf("Single double, &p\nReceived by process %d = %f\n",PID,p); 	// print it out at one destination

		SZ_Broadcast(A); 			// broadcast an array of doubles
		if (PID == 1) { 			// print it out at one destination
			printf("1-D Array of doubles, A\nReceived by process %d = ",PID);
			for (i = 0; i < 10; i++)
				printf("%2.2f ",A[i]);
			printf("\n");
		}

		SZ_Broadcast(B); 			// broadcast a 2-D array of doubles
		if (PID == 1) { 			// print it out at one destination
			printf("2-D array of doubles, B\nReceived by process %d\n",PID);
			for (i = 0; i < 2; i++) {
			     for (j = 0; j < 3; j++) 
				printf("%2.2f ",B[i][j]);
			     printf("\n");
			}
		}
	SZ_Parallel_end;				// end of parallel
	SZ_Finalize(); 
	return 0;
}

