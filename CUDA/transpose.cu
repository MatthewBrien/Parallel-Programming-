#include <stdio.h>
#include <cuda.h>
#include <stdlib.h>
#define N 17  // size of arrays
__global__ void transpose (int *a, int *b) {
                   int col = blockIdx.x*blockDim.x+threadIdx.x;
                   int row =blockIdx.y*blockDim.y+threadIdx.y;
                   int index1 = col + row * N;
                   int index2 = row + col * N;
                   if ( col < N && row < N) b[index1]= a[index2];
}
int main (int argc, char **argv ) {
 int i,j;
 int size = N * N *sizeof( int);
 int  a[N][N], *devA, *devB;
 int gridsize = (int) ceil((double) N/16);
for (i = 0; i < N; i++){                  // put some numbers into array
 for (j= 0; j < N; j++) {
    a[i][j] = j + N * i;
      }
    }

 printf("Initial values");
     for (i = 0; i < N; i++) {
      printf("\n");
      for (j= 0; j < N; j++) {
        printf("%3d  ",a[i][j]);
       }
      }

  printf("\nN = %d, grid size = %d\n",N,gridsize);
     dim3 block (16,16);
     dim3 grid (gridsize, gridsize);
     cudaMalloc( (void**)&devA, size );
     cudaMalloc( (void**)&devB, size );
     cudaMemcpy( devA, a, size, cudaMemcpyHostToDevice);
     transpose<<<grid, block>>>(devA, devB);
     cudaMemcpy( a, devB, size, cudaMemcpyDeviceToHost);
         printf("Results");
        for (i = 0; i < N; i++) {
          printf("\n");
          for (j= 0; j < N; j++) {
            printf("%3d  ",a[i][j]);
          }
        }
 printf("\n");
 cudaFree( devA);
 cudaFree( devB);
 return (0);
}
