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
  int i;
  char buffer[LENGTH];
  int position=0;
  float a=1.0;
  int b=2;
  char c='a';
  int d[10];
  short e=50;
  double t1, t2;

  /* start up MPI */
  MPI_Init( &argc, &argv );

  /* find out process rank */
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  /* find out number of processes */
  MPI_Comm_size(MPI_COMM_WORLD, &p);

  /* time measuring - start */
  t1 = MPI_Wtime();

  if (my_rank != 0) {
    /* create message */
    for (i=0;i<10;i++) d[i] = i;
    MPI_Pack(&a, 1, MPI_FLOAT, buffer, LENGTH, &position, MPI_COMM_WORLD);
    MPI_Pack(&b, 1, MPI_INT, buffer, LENGTH, &position, MPI_COMM_WORLD);
    MPI_Pack(&c, 1, MPI_CHAR, buffer, LENGTH, &position, MPI_COMM_WORLD);
    MPI_Pack(d, 10, MPI_INT, buffer, LENGTH, &position, MPI_COMM_WORLD);
    MPI_Pack(&e, 1, MPI_SHORT, buffer, LENGTH, &position, MPI_COMM_WORLD);
    dest=0;
    MPI_Send (buffer, position, MPI_PACKED, dest, tag, MPI_COMM_WORLD);
  }
  else {
    /* my_rank == 0 */
    printf("%d: There are %d processes.\n", my_rank, p);
    for (source=1;source<p;source++) {
      position=0;
      MPI_Recv(buffer, LENGTH, MPI_PACKED, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      MPI_Unpack(buffer, LENGTH, &position, &a, 1, MPI_FLOAT, MPI_COMM_WORLD);
      MPI_Unpack(buffer, LENGTH, &position, &b, 1, MPI_INT, MPI_COMM_WORLD);
      MPI_Unpack(buffer, LENGTH, &position, &c, 1, MPI_CHAR, MPI_COMM_WORLD);
      MPI_Unpack(buffer, LENGTH, &position, d, 10, MPI_INT, MPI_COMM_WORLD);
      MPI_Unpack(buffer, LENGTH, &position, &e, 1, MPI_SHORT, MPI_COMM_WORLD);
    printf ("%d: I have got message %f %d %c ", my_rank,a,b,c);
    for (i=0;i<10;i++) printf("%d ",i);
    printf ("%d \n",e);
    }
  }

  /* time measuring - stop */
  t2 = MPI_Wtime();

  printf ("%d: Elapsed time is %f.\n",my_rank,t2-t1);

  /* shut down MPI */
  MPI_Finalize();
  
  return 0;
}

