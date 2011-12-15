#include "mpi.h"
#include <stdio.h>

int main(int argc, char **argv) {
  int my_rank;
  int p;
  MPI_Status status;
  int messageIn[2]={0,0};
  int messageOut[2]={0,0};

  /* start up MPI */
  MPI_Init( &argc, &argv );

  /* find out process rank */
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  /* find out number of processes */
  MPI_Comm_size(MPI_COMM_WORLD, &p);

  messageIn[0] = my_rank*100;
  messageIn[1] = my_rank*100+1;
  /* make reduction - find maximum from all values in all messages and return it to process 0 */
  MPI_Reduce (&messageIn, &messageOut, 2, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

  if (my_rank == 0)
    printf("Maximum is %d, %d.\n",messageOut[0],messageOut[1]);

  /* shut down MPI */
  MPI_Finalize();
  
  return 0;
}
