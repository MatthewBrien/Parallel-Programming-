// B. Wilkinson Nov 14, 2015 Testing SZ_Point_to_point with different types
#include <stdio.h>
#include <string.h>
#include "suzaku.h"  			// Suzaku macros

int main(int argc, char *argv[]) {
	int i,j, P, PID; 				//All variables declared here are in every process
	int x = 88,y=99;					
	double a[10] = {0,1,2,3,4,5,6,7,8,9};
	double b[10] = {0,0,0,0,0,0,0,0,0,0};
	char a_message[20], b_message[20];
	strcpy(a_message, "Hello world");
	strcpy(b_message, "------------");
	double p=123, q=0;
	double xx[2][3] = {{0,1,2},{3,4,5}},yy[2][3] = {{0,1,2},{3,4,5}};// multidimensional can only be doubles
	
	SZ_Init(P);					// initialize MPI message-passing environment, 
							// sets P to be number of processes, not used here
	SZ_Parallel_begin				// parallel section, all processes do this
		PID = SZ_Get_process_num();  		// get process ID

		SZ_Point_to_point(0, 1, a_message, b_message);  // send a message from one process to another
		if (PID == 1) printf("Received by process %d = %s\n",PID,b_message); // print it out at destination

		SZ_Point_to_point(0, 1, &x, &y);  	// send an int from one process to another
		if (PID == 1) printf("Received by process %d = %d\n",PID,y); // print it out at destination

		SZ_Point_to_point(0, 1, a, b); 		// send an array of doubles from one process to another
		if (PID == 1) { 			// print it out at destination
			printf("Received by process %d = ",PID);
			for (i = 0; i < 10; i++)
				printf("%2.2f ",b[i]);
			printf("\n");
		}

		SZ_Point_to_point(0, 1, &p, &q); 		// send a double from one process to another	
		if (PID == 1)  printf("Received by process %d = %f\n",PID,q); 	// print it out at destination

		SZ_Point_to_point(0, 1, xx, yy); 	// send an 2-D array of doubles from one process to another
		if (PID == 1) { 			// print it out at destination
			printf("Received by process %d\n",PID);
			for (i = 0; i < 2; i++) {
			     for (j = 0; j < 3; j++) 
				printf("%2.2f ",yy[i][j]);
			     printf("\n");
			}
		}
	SZ_Parallel_end;				// end of parallel

	SZ_Finalize(); 

	return 0;
}

