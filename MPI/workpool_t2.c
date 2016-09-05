#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"
#define PI 3.14159265358979

int main(int argc, char *argv[]){
  int id, i, seed, seeds, ieer =0;                //process id, and root (root process id == 0)
  int n,iterations  = 0;                        //number of processe 'n', number of seeds to disperce 's'
  double x, y,t, pi, seed_time, start, end;       //coordinates inside or outside the circle
  int final_seed_time, total = 0;             //inside circle
  char machine_name[256];
    gethostname(machine_name, 255);
  int result[2];                                  //result, and process id
  int s_and_n[2];                                 //random see, and number of itterations

  MPI_Status status;
  ieer = MPI_Init(&argc, &argv);
  ieer = MPI_Comm_rank(MPI_COMM_WORLD, &id);
  ieer = MPI_Comm_size(MPI_COMM_WORLD, &n);
  //if root, send a seed to each slaves, then receive loop while seeds are left
  //else, receive, do work , send, repeat



  if(id == 0)
  {
        printf("-Master Process running on machine: %s\n", machine_name);
    printf("Please enter a number of iterations:\n");
    scanf("%d", &iterations);
    if(iterations < 1){ iterations = 1; }
    s_and_n[1] = iterations;                        //each process needs this, in retrospect I could get away with sending it once

    printf("Please enter the number of seeds:\n");
    scanf("%d",&seed);
    if(seed <= n){                                 //error catching, seeds should be less than or equal to processes
        printf("That's not enough seeds, using %d\n", n);
        seed = n;
    }


    seeds = seed; //seeds get incremented, need to keep the original value
      //send initial seeds to each process
      //to make sure the numbers are random, I use MPI_Time, but srand takes an Int,
      //a simple cast from a double to int truncates the decimal points,
      //at high speeds, the difference between two MPI_Wtime statements is entirely in those deicimal places
      //so I have to preserve them. seed time * 1000000 gets the least dignificant digits past the decimal point
      //(int)((insigned long long int)seed_time  % 1000000) gets the original 6 least significant digits
    seed_time= MPI_Wtime();     //mpi_wtime
      //  printf("initial time %f\n", seed_time);
    seed_time *= 1000000;
      //printf("seed time %f\n", seed_time);
      //printf("int time %llu\n", (unsigned long long int)seed_time);
    final_seed_time = (int)(((unsigned long long int )seed_time) % 1000000 );
      //printf("initial time %d\n", final_seed_time);
    start = MPI_Wtime();
    srand((int) final_seed_time); //seed random with time.
    for(i = 1; i< n; i++){
      s_and_n[0] = rand();
      //  printf("Sending to id %d\n",i);
      MPI_Send(s_and_n, 2, MPI_INT, i, 99, MPI_COMM_WORLD);
      seeds--;
    }
    //loop while there are still seeds to distrubute
    while(seeds > 0){
      //receive, tabulate, send
      MPI_Recv(result, 2, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      total += result[0];
      //  printf("just recveived from and Sending to process %d\n",result[1]);
      seed_time= MPI_Wtime();
      seed_time *= 100000;
      final_seed_time = (int)(((unsigned long long int )seed_time) % 1000000 );
      srand((int) final_seed_time); //reseed random numbers with time
      s_and_n[0] = rand();
      MPI_Send(s_and_n, 2, MPI_INT, result[1], 99, MPI_COMM_WORLD);
      seeds--;
    }
    //send an end message to each process
    printf("All seeds distributed\n");
    s_and_n[0] = -1;
    for(i = 0; i< n-1; i++){
      MPI_Recv(result, 2, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      total += result[0];
      MPI_Send(s_and_n, 2, MPI_INT, result[1], 99, MPI_COMM_WORLD);
    }
    fflush(stdout);
    printf("All workers have finished\n");
  }
  else
  {

  printf("-Slace process %d running on machine: %s\n", id, machine_name);
    while(1==1){
    //  printf("Id is %d waiting to receive \n",id);
      MPI_Recv(s_and_n, 2, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    //  printf("process %d, just got %d from 0 \n",id, s_and_n[0]);
      if(s_and_n[0] == -1){
        break;
      }
      else{
        result[0] = 0;
        result[1] = id;
        srand(s_and_n[0]); //seed slave's random number sequence
        for(i = 0; i<s_and_n[1];i++){
          x = (double)rand() / (double)RAND_MAX;
          y = (double)rand() / (double)RAND_MAX;
          t = (x*x) + (y*y);
          //printf("process %d just calculated X and Y as %f %f which makes y %f\n", id, x, y,t);
          if(t<=1){
            result[0]++;
          }
        }
        //return the number of points inside the circle
        MPI_Send(result, 2, MPI_INT, 0, 99, MPI_COMM_WORLD);
      }
    }
  }
  if(id == 0){
    end = MPI_Wtime();
    pi = (double) (4*total)/(iterations*seed);
    printf("total : %d  out of %d\n", total, (iterations*seed));
    printf("Pi is about %f\n",pi);
      printf("percent error = %f\n", ((pi - PI)/PI)*100);
      printf("Computation time : %f\n", (end-start));
  }
  MPI_Finalize();
  return (0);
}
