#include <stdio.h>
#include "mpi.h"
#include <stdlib.h>

int main (int argc, char*argv[]){
  int rank, i, P = 0;
  int array[25];
  MPI_Status status;
int z;





  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &P);
  if(rank==0){
    printf("Please enter a number:");
    scanf("%d",&z);

  }
  for(i=0; i< 25; i++){array[i]=0;}
  if(rank==0){
    for(i = 0; i<25; i++){
      array[i] = i;
    }
  }
  MPI_Scatter(array, 5, MPI_INT, array, 5, MPI_INT, 0, MPI_COMM_WORLD);// Scatter input matrix A

  if(rank ==0){
    for(i=5; i<25;i++){
      array[i]=0;
    }
    array[0]=1;
  }

  i=0;
  while(array[i]  !=0){
    i++;
  }
  printf("In process %d I counted %d filled elements\n", rank, i);
  MPI_Gather(array, 5, MPI_INT, array, 5, MPI_INT, 0, MPI_COMM_WORLD );
  if(rank == 0){
    for(i=0;i<25;i++){
      printf(" %d ", array[i]);
    }
    printf("\n");
  }
  MPI_Finalize();


  return(0);
}
