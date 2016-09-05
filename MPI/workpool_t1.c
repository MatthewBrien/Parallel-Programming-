#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define PI 3.14159265358979

int main (int argc, char *argv[]){
  int n,i,out, in = 0; //number of iterations, and for loop counter
  double x, y; //random numbers
  srand(time(NULL));
  printf("Please enter a number of iterations:"); //get user input
  scanf("%d", &n);
  clock_t t1,t2; //set up clocks
  t1 = clock();
  for(i =0; i<n;i++)
  { //generate a random x and y value, between 0 and 1
    x = (double)rand() / (double)RAND_MAX;
    y = (double)rand() / (double)RAND_MAX;
    double t = (x*x) + (y*y);
    if( t <= 1 ){  in++;  }
  }
  double pi = (double) in/n*4;
  t2 = clock();
  float diff = ((float)(t2-t1) / 1000000.0F ) ;
  printf("Time to complete was %f\n", diff);
  printf("Pi is about %f\n", pi);
  printf("percent error = %f\n", ((pi - PI)/PI)*100);

}
