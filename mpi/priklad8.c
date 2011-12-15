#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define DEBUG 1
#define KOLIKRAT 10
#define VELIKOST 100000

int main(int argc, char **argv) {
  int my_rank;
  int p;
  int from, to;
  int tag2=1;
  int tag1=100;
  int length;
  int source;
  MPI_Status status;
  MPI_Request request;
  int *message, *message2;
  int param[2];
  int i, flg=0;

  /* start up MPI */
  MPI_Init( &argc, &argv );

  /* find out process rank */
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  /* find out number of processes */
  MPI_Comm_size(MPI_COMM_WORLD, &p);

  if (my_rank != 0) {
      /* blocking receive */
#ifdef DEBUG
    printf("Receving params, proc %d\n",my_rank);
#endif
      MPI_Recv(param, 2, MPI_INT, 0, tag1, MPI_COMM_WORLD, &status);
#ifdef DEBUG
    printf("Got params, proc %d, %d\n",param[0],param[1],my_rank);
#endif
     
  }
  else {
    /* my_rank == 0 */
    param[0] = KOLIKRAT;
    param[1] = VELIKOST;
    for (source=1;source<p;source++) {
      /* this is blocking send */
#ifdef DEBUG
    printf("Sending params to %d\n",source);
#endif
      MPI_Send (param, 2, MPI_INT, source, tag1, MPI_COMM_WORLD);
    }
  }

  MPI_Barrier(MPI_COMM_WORLD);

#ifdef DEBUG
    printf("After params, proc %d\n",my_rank);
#endif

  message=(int *) malloc(param[1]*sizeof(int));
  message2=(int *) malloc(param[1]*sizeof(int));
  to=(my_rank+1)%p;
  from=my_rank-1;
  if (from<0) from=p-1;

  for (i=0;i<param[0];i++) {
#ifdef DEBUG
    printf("Send/Receive, proc %d\n",my_rank);
#endif
    MPI_Isend (message, param[1], MPI_INT, to, tag2, MPI_COMM_WORLD, &request);
    do {
      MPI_Iprobe (from, tag2, MPI_COMM_WORLD, &flg, &status);
    } while (!flg);
    MPI_Recv(message2, param[1], MPI_INT, from, tag2, MPI_COMM_WORLD, &status);
    do {
      MPI_Test(&request, &flg, &status);
    } while (!flg);
  }

  /* shut down MPI */
  MPI_Finalize();
  
  free(message2);
  free(message);

  return 0;
}
