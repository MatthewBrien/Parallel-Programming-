#include <stdio.h>
#include <math.h>
#define N 6 //bodies
#define D 5 //dimensions (mass, x & y coor, x & y vel)
#define G 100 // gravitational constant rounded from 6.674×10−11 N⋅m2/kg2
#define deltaT 0.05


void print_bodies(double A[N][D]){
  int i;
  printf("Body\tmass\t\tX\t\tY\t\txVel\t\tYvel\n");
  for(i=0; i<N; i++){
      printf("%d  \t%.4f  \t%.4f  \t%.4f  \t%.4f  \t%.4f\n", i, A[i][0], A[i][1], A[i][2], A[i][3], A[i][4]);
  }
}


int main (int argc, char *argv[]){
  //int  T;//constants, Number of bodies, Time
  int n, i, a,t,T; //iterators
  double A[N][D] = {{25, 400, 400,  0, 0},   //each body [N][0:Mass, 1: Pos. in X., 2: Pos. in Y,  3:V in X,  4: V in Y]
                    {20, 200, 400,  3, 4},
                    {30,  50, 600,  1, 0},
                    {50, 400, 200,  1, 0},
                    {40, 700, 700, -1, 0},
                    {70, 200, 100, -1, 0}};
  double Force[N][N][2];  //force between body [N1], [N2], in [0:X, 1:Y]
  double x_diff , y_diff, r, Fx[N], Fy[N], F;

  printf("Time interval is %f\n", deltaT);
  printf("Number of itterations:");
  scanf("%d",&T);
  print_bodies(A);



  for(t=0; t<T;t++ ){ //for each time interval
    for(i=0;i<N;i++){
      Fx[i] = 0;
      Fy[i] = 0;
    }
      for(a=0; a<N; a++){
          for(i=0; i<N; i++){
              if(a != i && A[a][0] != 0){ // bodies don't  need to check agains them selves
                x_diff = A[i][1] - A[a][1];
                y_diff = A[i][2] - A[a][2];
                r = sqrt((x_diff * x_diff ) + (y_diff * y_diff)); // actual distance between points
                if(r <= ((A[a][0]+A[i][0])/2))
                {
                  printf("Planet %d and %d were pretty close at (%f,%f) and (%f, %f) and then died\n", a, i, A[a][1], A[a][2], A[i][1], A[i][2]);
                  A[a][0]=0;
                  A[i][0]=0;
                  A[a][1]=0;
                  A[i][1]=0;
                  A[a][2]=0;
                  A[i][2]=0;

                }
                else{
                  F = (G*A[a][0]*A[i][0])/(r*r); // cumulative force F = G*M1M2/r^2
                  Fx[a] += (F*(x_diff / r)); //sum forces in the X direction
                  Fy[a] += (F*(y_diff / r)); //sum forces in the y direction
                }

              }
            }
          }

    for(a=0; a<N;a++)  {
      if(A[a][0] != 0){
        A[a][3] += ((Fx[a]*deltaT)/A[a][0]);//x velocity
        A[a][4] += ((Fy[a]*deltaT)/A[a][0]);//y velocity
        A[a][1] += (A[a][3]*deltaT);//x position = v*t
        A[a][2] += (A[a][4]*deltaT);//y position = v*t
      }
    }

}
print_bodies(A);

  return 0;
}
