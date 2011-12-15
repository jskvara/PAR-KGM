#include "mpi.h"
#include <stdio.h>

#define LENGTH 100

int main(int argc, char **argv) {
  int my_rank;
  int p;
  int dest;
  int tag=1;
  int source;
  MPI_Status status;
  char buffer[LENGTH];
  int position=0;
  float a=1.0;
  int b=2;

  /* start up MPI */
  MPI_Init( &argc, &argv );

  /* find out process rank */
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  /* find out number of processes */
  MPI_Comm_size(MPI_COMM_WORLD, &p);

  if (my_rank != 0) {
    /* create message */
    MPI_Pack(&a, 1, MPI_FLOAT, buffer, LENGTH, &position, MPI_COMM_WORLD);
    MPI_Pack(&b, 1, MPI_INT, buffer, LENGTH, &position, MPI_COMM_WORLD);
    dest=0;
    MPI_Send (buffer, position, MPI_PACKED, dest, tag, MPI_COMM_WORLD);
  }
  else {
    /* my_rank == 0 */
    printf("There are %d processes.\n", p);
    for (source=1;source<p;source++) {
      position=0;
      MPI_Recv(buffer, LENGTH, MPI_PACKED, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      MPI_Unpack(buffer, LENGTH, &position, &a, 1, MPI_FLOAT, MPI_COMM_WORLD);
      MPI_Unpack(buffer, LENGTH, &position, &b, 1, MPI_INT, MPI_COMM_WORLD);
    printf ("I have got message %f %d\n",a,b);
    }
  }

  /* shut down MPI */
  MPI_Finalize();
  
  return 0;
}
