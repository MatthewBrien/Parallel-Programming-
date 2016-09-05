#include <omp.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "X11Macros.h"		// X11 macros


              // X11 macros


void printwall(double* wall, int length)
  //Receive a pointer to an array, and the length of the array, and print out the elements
  {
    int i;
    if(length >=20)
    {
      for( i=0; i< length; i+=(length / 10))
      {
        printf("%.2f ", wall[i]);
      }
    }
    else
    {
      for( i=0; i< length; i++)
      {
        printf("%.2f ", wall[i]);
      }
  }
  printf("\n");
}

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
int comparerooms(double** room1, double** room2, int length){
	//check both arrays against one another
  int i, j;
	for( i = 0; i < length; i++)
		{
			for(  j = 0; j<length; j++)
			{

				if (abs(room1[i][j] - room2[i][j]) > .001)
				{
					printf("i and j are at [%d,%d], and equal to %f %f", i, j, room1[i][j], room2[i][j]);
					return(1);
				}

			}
		}
		return(0);

}

int main(int args){
  int length, i, j, Z, itterations, numThreads;
  double start, end, t1, t2 = 0;
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
		double** roomC;
		double** roomD;
		double**tempRoom; //used when switching roomA and roomB, and roomC and roomD
	  roomA = (double**) malloc(length*sizeof(double*));
	  roomB = (double**) malloc(length*sizeof(double*));
		roomC = (double**) malloc(length*sizeof(double*));
		roomD = (double**) malloc(length*sizeof(double*));

  //generate sperately in hopes of getting some benefit from reference of locality
    for( i = 0; i<length; i++){
	      roomA[i] = (double*) malloc(length*8);
	    }
	    for( i = 0; i<length; i++){
	      roomB[i] = (double*) malloc(length*8);
	    }
			for( i = 0; i<length; i++){
				roomC[i] = (double*) malloc(length*8);
			}
			for( i = 0; i<length; i++){
			  roomD[i] = (double*) malloc(length*8);
			}
  //initialize the arrays.
  for( i =0; i<length; i++){
    for( j = 0; j < length; j++){
    roomA[i][j] = 20;
    roomB[i][j] = 20;
		roomC[i][j] = 20;
		roomD[i][j] = 20;
  }}

  //initialize the fireplace
  setupfireplace(roomA[0], length);
	  setupfireplace(roomB[0], length);
		setupfireplace(roomC[0], length);
		setupfireplace(roomD[0], length);
//printf("Initial Values: \n");



//sequential
start = omp_get_wtime();
	//computer the average Jacobi iterations
	//setting max iterations initialy to 4
	//if iterations is an even number, the last iterations has the fireplace
	//if the iterations is an odd number, the second to last iterations has the fireplace.
	for( Z = 0; Z < itterations; Z++){
	  for( i = 1; i < length-1; i++){
	    for( j = 1; j<length-1; j++){
	      roomB[i][j] = 0.25*( roomA[i-1][j]+roomA[i+1][j]+roomA[i][j-1]+roomA[i][j+1]);
	    }
	  }
	  //swap the room's pointers
	  tempRoom = roomA;
	  roomA = roomB;
	  roomB = tempRoom;
	}
		end = omp_get_wtime();
		t1 = end - start;
		printf("Time of sequential computation: %f seconds\n", end-start);
			printroom(roomA, length);

//parallel iteration
start = omp_get_wtime();
	  {
	  for(Z = 0; Z < itterations; Z++){

	    for(i = 1; i < length-1; i++){
			#pragma omp parallel for schedule(static, 10) private(j)
	      for(j = 1; j<length-1; j++){
	        roomC[i][j] = 0.25*( roomD[i-1][j]+roomD[i+1][j]+roomD[i][j-1]+roomD[i][j+1]);
	      }
	    }
	    //swap the room's pointers
	    tempRoom = roomC;
	    roomC = roomD;
	    roomD = tempRoom;
	  }
	}
	 	end = omp_get_wtime();	//end time measurement
		t2 = end-start;
		printf("Time of parallel computation: %f seconds\n", end-start);
	//printf("parallel: \n");
		printroom(roomC, length);


//Compare rooms, and print if they were the same or not
if(comparerooms(roomA, roomC, length) == 1){
		printf("NOOOOOOOOOOOOOOOOOOOO, there done was a problem! \n");
	}
	else
	{
		printf("The rooms are the same \n");

		printf("The speed up factor was %.5f\n", t1/t2);
	}

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

	while(1==1)
  {
    XFlush(display);
  }
return (0);
}
