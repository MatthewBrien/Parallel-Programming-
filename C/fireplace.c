	#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>



void printroom(double** room, int length){
  for(int i = 0; i<length; i+=(length/10)){
    for(int k =0; k<length;k+=(length/10)){
      printf("%.2f ", room[i][k]);

    }
  printf("\n");
  }
}
int myfloor(double n)
  {
    return ((int) n / 1);
  }
int myceil(double n)
  {
    if( ((int) n / 1) == n)
    return ((int) n);
    else
    return ((int) n + 1);
  }
void setupfireplace(double* wall, int length)
  {
      double percent = (((double) length)/100)*40;
      int start = myceil(percent);
      int end = length - start;
      for(int i = start; i < end; i++)
      {
        wall[i] = 100; //initialize the fireplace
      }
  }


int main(int args){
  int length;
  int itterations;
  double start, end = 0;
    //get user input, room size and iterations
  printf("Please enter a length for the room: ");
    scanf("%d", &length);
    printf("\nPlease enter the number of iterations: ");
    scanf("%d", &itterations);

  //Generate two array of array pointers. Each array is length long,
  //and each element will point to an array of doubles that is lenght long
  double** roomA;
  double** roomB;
  roomA = (double**) malloc(length*sizeof(double*));
  roomB = (double**) malloc(length*sizeof(double*));

  //generate sperately in hopes of getting some benefit from reference of locality
    for(int i = 0; i<length; i++){
      roomA[i] = (double*) malloc(length*8);
    }
    for(int i = 0; i<length; i++){
      roomB[i] = (double*) malloc(length*8);
    }
  //initialize the arrays.
  for(int i =0; i<length; i++){
    for(int j = 0; j < length; j++){
    roomA[i][j] = 20;
    roomB[i][j] = 20;
  }}

  //initialize the fireplace
  setupfireplace(roomA[0], length);
  setupfireplace(roomB[0], length);
printf("Initial Values: \n");
printroom(roomA, length);
double**tempRoom;

start = omp_get_wtime();
//computer the average Jacobi iterations
//setting max iterations initialy to 4
//if iterations is an even number, the last iterations has the fireplace
//if the iterations is an odd number, the second to last iterations has the fireplace.
for(int Z = 0; Z < itterations; Z++){
  for(int i = 1; i < length-1; i++){
    for(int j = 1; j<length-1; j++){
      roomB[i][j] = 0.25*( roomA[i-1][j]+roomA[i+1][j]+roomA[i][j-1]+roomA[i][j+1]);
    }
  }
  //swap the room's pointers
  tempRoom = roomA;
  roomA = roomB;
  roomB = tempRoom;
}
end = omp_get_wtime();
	printf("Time of sequential computation: %f seconds\n", end-start);

printf("Final Values: \n");
printroom(roomA, length);

  return (0);
}
