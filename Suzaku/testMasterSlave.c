#define N 6
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "suzaku.h"

int main(int argc, char *argv[]){
  int i;
  int P = 6;
  int z;
  SZ_Init(P);
  SZ_Parallel_begin
  i = SZ_Get_process_num();

  if(i == 0){
  printf("Please give user input:\n");
  scanf("%d", &z);
}
  SZ_Parallel_end;
  SZ_Finalize();
}
