// VectorAdd.cu
#include <stdio.h>
#include <cuda.h>
#include <stdlib.h>
#define N 100                                      // size of matricies
#define B 1                                      // blocks in the grid
#define T 100                                         // threads in a block

__global__ void multiply (int *a,int *b, int *c) {

      int index = threadIdx.y + blockDim *threadIdx.x;
      int row = threadIdx.x;
      int col = threadIdx.y;
        //int tidy = blockIdy.y *
        if(row < N  && col < N) {
          int i;
          double product = 0;
          for(i = 0; i< N; i++){
            product += a[row+i]*b[col*i];
          }
          c[index] = product;
        }
}
int main(void) {
    int a[N][N],b[N][N],c[N][N], d[N][N];
    int *dev_a, *dev_b, *dev_c;
    int matrix = N*N;  //user will enter a single dimension, but, it's like a square yo.
    double product;
    dim3 gridBlock(B,B,0); //2d structure of for the grid setup



    cudaMalloc((void**)&dev_a,matrix * sizeof(int));
    cudaMalloc((void**)&dev_b,matrix * sizeof(int));
    cudaMalloc((void**)&dev_c,matrix * sizeof(int));

    //initialize the matrices
    int i, ii, iii;
    for (i=0;i<N;i++) {
      for(ii = 0; ii < N; ii++){
        a[i][ii] = i+ii;
        b[i][ii] = i+(2*ii);
      }
    }

    //copy values to gpu
    cudaMemcpy(dev_a, a , N*sizeof(int),cudaMemcpyHostToDevice);
    cudaMemcpy(dev_b, b , N*sizeof(int),cudaMemcpyHostToDevice);

    //call kernel
    multiply<<<B,gridBlock>>>(dev_a,dev_b,dev_c);

    cudaMemcpy(c,dev_c,N*sizeof(int),cudaMemcpyDeviceToHost);

    //calculate sequentiallly
    for(i=0;i<N;i++){
      for(ii=0;ii<N;ii++){
        product = 0;
        for(iii=0;iii<N;iii++){
          product += a[i][iii] * b[iii][ii];

        }
        //compare sequential and parallel computations
        d[i][ii] = product;
        if(d[i][ii] != c[i][ii])
        {
          printf("Error at location %d, %d,   %f vs %f \n", i, ii, d[i][ii], c[i][ii]);
        }
      }
    }

    cudaFree(dev_a);
    cudaFree(dev_b);
    cudaFree(dev_c);
  return 0;
}
