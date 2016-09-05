#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "X11Macros.h"
#include "suzaku.h"
#define N 6 //bodies
#define D 5 //dimensions (mass, x & y coor, x & y vel)
#define G 100 // gravitational constant rounded from 6.674×10−11 N⋅m2/kg2
#define X_RESN 1000
#define Y_RESN 1000
#define deltaT 0.05




void print_bodies(double A[N][D]){
  int i;
  printf("Body\tmass\t\tX\t\tY\t\txVel\t\tYvel\n");
  for(i=0; i<N; i++){
      printf("%d  \t%.4f  \t%.4f  \t%.4f  \t%.4f  \t%.4f\n", i, A[i][0], A[i][1], A[i][2], A[i][3], A[i][4]);
  }
}
void armagedon(double A[N][D], int a, int i){

  printf("Planet %d and %d were pretty close at (%f,%f) and (%f, %f) and then died\n", a, i, A[a][1], A[a][2], A[i][1], A[i][2]);
  A[a][0]=0; A[i][0]=0; A[a][1]=0;
  A[i][1]=0; A[a][2]=0; A[i][2]=0;

}

int main (int argc, char *argv[]){
  int P = N;
  //int  T;//constants, Number of bodies, Time
  int n, i, a,t,T, PID; //iterators
  double A[N][D] = {{25, 400, 400,  0, 0},   //each body [N][0:Mass, 1: Pos. in X., 2: Pos. in Y,  3:V in X,  4: V in Y]
                    {20, 200, 400,  3, 4},
                    {30,  50, 600,  1, 0},
                    {50, 400, 200,  1, 0},
                    {40, 700, 700, -1, 0},
                    {70, 200, 100, -1, 0}};
  double Force[N][N][2];  //force between body [N1], [N2], in [0:X, 1:Y]
  double x_diff , y_diff, r, F, Fx, Fy;


  SZ_Init(P);


  SZ_Parallel_begin
  PID = SZ_Get_process_num();
if(PID == 0){
  printf("Enter the Number of Itterations:\n");
  scanf("%d", &T);
  printf("Performing N-Body Master Slave Computation with %d bodies, time interals of %f, and %d itterations\n", N, deltaT, T);
  print_bodies(A);
}
SZ_Broadcast(&T);
  for(t=0; t<T;t++ ){ //for each time interval
    Fx = 0;
    Fy = 0;
    SZ_Broadcast(A);



      if(A[PID][0] != 0){
        for(i=0; i<N; i++){
              if(PID != i ){ // bodies don't  need to check agains them se
                x_diff = A[i][1] - A[PID][1];
                y_diff = A[i][2] - A[PID][2];
                r = sqrt((x_diff * x_diff ) + (y_diff * y_diff)); // actual distance between points
                if(r < ((A[PID][0]+A[i][0])/2))
                {
                  printf("Collision detected during time interval %d\n", t);
                  armagedon(A, PID, i);
                }
                else{
                  F = (G*A[PID][0]*A[i][0])/(r*r); // cumulative force F = G*M1M2/r^2
                  Fx += (F*(x_diff / r)); //sum forces in the X direction
                  Fy += (F*(y_diff / r)); //sum forces in the y direction
                }

              }
            }



      if(A[PID][0] != 0){
      A[PID][3] += ((Fx*deltaT)/A[PID][0]);//x velocity
      A[PID][4] += ((Fy*deltaT)/A[PID][0]);//y velocity
      A[PID][1] += (A[PID][3]*deltaT);//x position = v*t
      A[PID][2] += (A[PID][4]*deltaT);//y position = v*t
    }//gather
  }

    SZ_AllBroadcast(A);
}

SZ_Parallel_end;
print_bodies(A);
SZ_Finalize();

  return 0;
}
