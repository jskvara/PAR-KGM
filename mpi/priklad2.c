#include "mpi.h"
#include <stdio.h>

int main(int argc, char **argv) {
  int my_rank;
  int p;
  int dest;
  int tag=1;
  int source;
  MPI_Status status;
  int message=999;
  int m=100;

  /* start up MPI */
  MPI_Init( &argc, &argv );

  /* find out process rank */
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  /* find out number of processes */
  MPI_Comm_size(MPI_COMM_WORLD, &p);

  if (my_rank != 0) {
    /* receive message from process 0 by blocking receive subroutine */
    MPI_Recv (&message, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

    dest=0;
    //    message=my_rank;
    MPI_Send (&message, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
  }
  else {
    /* my_rank == 0 */
    printf("There are %d processes.\n", p);

    /* broadcast message to all other processes */
    for (source=1;source<p;source++)
      MPI_Send(&message, 1, MPI_INT, source, tag, MPI_COMM_WORLD);

    message=0;

    for (source=1;source<p;source++) {
      MPI_Recv(&message, 1, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
      printf ("I have got: %d\n",message);
    }
  }

  /* shut down MPI */
  MPI_Finalize();
  
  return 0;
}
