#include <stdio.h>
#include <cuda.h>
#include <stdlib.h>
#define N 10  // size of arrays
#define B 1 //number of blocks
__global__ void multiply (int *a, int *b, int *c) {
							//this will only work for a single block...
									   int col = blockIdx.x*blockDim.x+threadIdx.x;
	                   int row =blockIdx.y*blockDim.y+threadIdx.y;
									int tid = row*blockDim.x + col;

									double product = 0;
									int i = 0;
									for(i=0;i<N;i++){
										product += a[row+i] * b[col*i];
									}

									c[tid] = product;
}

int main (int argc, char **argv ) {
 int i,ii;
 int size = N * N *sizeof(int); // total size of arrays
 int  a[N][N], b[N][N], c[N][N], *devA, *devB, *devC; //TODO::declare a,b,c will malloc...
 int gridsize = (int) ceil((double) N/16);
for (i = 0; i < N; i++){                  // put some numbers into array
 for (ii= 0; ii < N; ii++) {
    	a[i][ii] = ii + N * i;
			b[i][ii] = ii + N * i;
      }
    }

 printf("Initial values of A");
     for (i = 0; i < N; i++) {
      printf("\n");
      for (ii= 0; ii < N; ii++) {
        printf("%3d  ",a[i][ii]);
       }
      }

	 printf("\nInitial values of B");
	     for (i = 0; i < N; i++) {
	      printf("\n");
	      for (ii= 0; ii < N; ii++) {
	        printf("%3d  ",B[i][ii]);
	       }
			 }

  printf("\nN = %d, grid size = %d\n",N,gridsize);
     dim3 block (1,1);
     dim3 grid (10, 10);

		 cudaMalloc( (void**)&devA, size );
     cudaMalloc( (void**)&devB, size );
		 cudaMalloc( (void**)&devC, size );

     cudaMemcpy( devA, a, size, cudaMemcpyHostToDevice);
		 cudaMemcpu( devB, b, size, cudaMemcpyHostToDevice);
     multiply<<<grid, block>>>(devA, devB, devC);
     cudaMemcpy( c, devC, size, cudaMemcpyDeviceToHost);

 printf("\n");
 cudaFree( devA);
 cudaFree( devB);
 printf("\nfinal values of C");
		 for (i = 0; i < N; i++) {
			printf("\n");
			for (ii= 0; ii < N; ii++) {
				printf("%3d  ",C[i][ii]);
			 }
		 }
 return (0);
}
