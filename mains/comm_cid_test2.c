#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"
#include <sys/time.h>

int main( int argc, char *argv[] )
{
  struct timeval tv,tv1;
  struct timezone tz,tz1;
  /*struct tm *tm,*tm1;*/
  MPI_Comm comm,comm_self,dup1,dup2;
  int rank, size, rank2, size2, i , n; 
  MPI_Comm *commList; 

  /*
  printf(" %d:%02d:%02d %d \n", tm->tm_hour, tm->tm_min,
	 m->tm_sec, tv.tv_usec);
  */

  MPI_Init ( &argc, &argv );
  comm = MPI_COMM_WORLD;
  comm_self = MPI_COMM_SELF;
  n = 2000;
  i = 0;

  MPI_Comm_rank ( comm, &rank );
  MPI_Comm_size ( comm, &size );

  MPI_Comm_dup ( comm_self, &dup1 );
  MPI_Comm_dup ( comm_self, &dup2 );

  if(rank % 2 == 0) 
      MPI_Comm_free(&dup1);
  else 
      MPI_Comm_free(&dup2);

  commList = (MPI_Comm *)malloc(n * (sizeof(MPI_Comm)) );
  commList[0] = comm;

  gettimeofday(&tv, &tz);
  /*tm=localtime(&tv.tv_sec);*/
  for (i = 1 ; i < n ; i++) {
      MPI_Comm_dup ( commList[i-1], &commList[i] );
      MPI_Comm_size ( commList[i], &size2 );
      MPI_Comm_rank ( commList[i], &rank2 );
  }
  gettimeofday(&tv1, &tz1);
  /*tm1=localtime(&tv1.tv_sec);*/

  printf("Execution time = %d sec %d microseconds \n",
	 //(tm1->tm_hour - tm->tm_hour), (tm1->tm_min - tm->tm_min),
	 tv1.tv_usec > tv.tv_usec ? (tv1.tv_sec - tv.tv_sec)
	 : (tv1.tv_sec - tv.tv_sec - 1),
	 tv1.tv_usec > tv.tv_usec ? (tv1.tv_usec - tv.tv_usec) 
	 : 1000000 + (tv1.tv_usec - tv.tv_usec) );
  MPI_Finalize ();

  return ( 0 );
}
