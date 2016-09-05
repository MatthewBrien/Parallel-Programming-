#include <omp.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "X11Macros.h"		// X11 macros


void printroom(double** room, int length){
  int i, k;
  for( i = 0; i<length; i+=(length/10)){
    for( k =0; k<length;k+=(length/10)){
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
      int i;
      double percent = (((double) length)/100)*40;
      int start = myceil(percent);
      int end = length - start;
      for( i = start; i < end; i++)
      {
        wall[i] = 100; //initialize the fireplace
      }
  }

int main(int args){
  int length, i, j, Z, itterations, numThreads;
  double start, end;
	numThreads = 2;
	omp_set_num_threads(numThreads);//set number of threads here
	printf("Executing with %d Threads \n", numThreads);


    //get user input, room size and iterations, and x and y resolution
  printf("Please enter a length for the room: ");
	  scanf("%d", &length);
	  printf("\nPlease enter the number of iterations: ");
	  scanf("%d", &itterations);

  //Generate two array of array pointers. Each array is length long,
  //and each element will point to an array of doubles that is lenght long
  double** roomA;
	  double** roomB;

		double**tempRoom; //used when switching roomA and roomB, and roomC and roomD
	  roomA = (double**) malloc(length*sizeof(double*));
	  roomB = (double**) malloc(length*sizeof(double*));

  //generate sperately in hopes of getting some benefit from reference of locality
    for( i = 0; i<length; i++){
	      roomA[i] = (double*) malloc(length*8);
	    }
	    for( i = 0; i<length; i++){
	      roomB[i] = (double*) malloc(length*8);
	    }

  //initialize the arrays.
  for( i =0; i<length; i++){
    for( j = 0; j < length; j++){
    roomA[i][j] = 20;
    roomB[i][j] = 20;
  }}

  //initialize the fireplace
  setupfireplace(roomA[0], length);
	  setupfireplace(roomB[0], length);

//parallel iteration
start = omp_get_wtime();
	  {
	  for(Z = 0; Z < itterations; Z++){

	    for(i = 1; i < length-1; i++){
			#pragma omp parallel for schedule(static, 10) private(j)
	      for(j = 1; j<length-1; j++){
	        roomA[i][j] = 0.25*( roomB[i-1][j]+roomB[i+1][j]+roomB[i][j-1]+roomB[i][j+1]);
	      }
	    }
	    //swap the room's pointers
	    tempRoom = roomA;
	    roomA = roomB;
	    roomB = tempRoom;
	  }
	}
	 	end = omp_get_wtime();	//end time measurement

		printf("Time of parallel computation: %f seconds\n", end-start);

		printroom(roomB, length);


//display the results
	initX11(length,length);

/* ----- End of X11 graphics setup, continue with application code, sample given here ----- */

	XClearWindow(display, win);  				// clear window for next drawing
	XSetForeground(display,gc,(long) 0xDC143C);  		// color of foreground (applies to object to be drawn)

	for(i = 0; i<length; i++){
		for(j = 0; j<length; j++){

      if(roomA[i][j]==100){
      XSetForeground(display,gc,WHITE);
      XDrawPoint (display, win, gc, j, i);
      }
      else if(roomA[i][j]>=90){
          XSetForeground(display,gc,MAGENTA);
          XDrawPoint (display, win, gc, j, i);
      }
      else if(roomA[i][j]>=80){
          XSetForeground(display,gc,RED);
          XDrawPoint (display, win, gc, j, i);
      }
      else if(roomA[i][j]>=70){
          XSetForeground(display,gc,PINK);
          XDrawPoint (display, win, gc, j, i);
      }
      else if(roomA[i][j]>=60){
          XSetForeground(display,gc,ORANGE);
          XDrawPoint (display, win, gc, j, i);
      }
      else if(roomA[i][j]>=50){
          XSetForeground(display,gc,YELLOW);
          XDrawPoint (display, win, gc, j, i);
      }
      else if(roomA[i][j]>=40){
          XSetForeground(display,gc,GREEN);
          XDrawPoint (display, win, gc, j, i);
      }
      else if(roomA[i][j]>=30){
          XSetForeground(display,gc,BLUE);
          XDrawPoint (display, win, gc, j, i);
      }
      else{
          XSetForeground(display,gc,BLACK);
          XDrawPoint (display, win, gc, j, i);
      }

		}


	}

printf("press Ctrl-C to Quit\n");
	while(1==1)
  {

    XFlush(display);
  }
return (0);
}
