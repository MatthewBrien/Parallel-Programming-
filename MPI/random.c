#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <omp.h>
//testing the less than random nature of pseudorandom number generation
int main (int argc, char *argv[]){
//*printf("time %f\n", MPI_Wtime());
  //printf("time %f\n", MPI_Wtime());

  int i,r,j=0;

  printf("seeding with 2215\n");
  srand (2215);
  for(i = 0; i<5; i++){
      r = rand();
      printf("%d\n",r );
      }
printf("Reseed with 2237\n");
      srand (2237);
       for(i = 0; i<5; i++){
           r = rand();
           printf("%d\n",r );
           }
/*
    printf("Reseed\n");
      srand (1);
      for(i = 0; i<5; i++){
          r = rand();
          printf("%d\n",r );
          }

printf("seeding with time\n");
  for(j=0; j<3;j++){
    printf("reseed, new time__________________________\n");
    double time1 = omp_get_wtime();
    printf("time %f, \n", time1);
    time1 *= 100000;
    int time2 = (int)time1 % 1000;
    printf("new number %d\n", time2);
    srand((int) time1);
    for(i = 0; i<10; i++){
      r = rand();
      printf("%d\n",r );
      }
  }
  */

  return(0);
}
