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
  MPI_Comm comm;
  int rank, size, rank2, size2, i , n; 
  MPI_Comm *commList; 

  /*
  printf(" %d:%02d:%02d %d \n", tm->tm_hour, tm->tm_min,
	 m->tm_sec, tv.tv_usec);
  */

  MPI_Init ( &argc, &argv );
  comm = MPI_COMM_WORLD;
  n = 200;
  i = 0;

  MPI_Comm_rank ( comm, &rank );
  MPI_Comm_size ( comm, &size );

  commList = (MPI_Comm *)malloc(n * (sizeof(MPI_Comm)) );

  gettimeofday(&tv, &tz);
  //tm=localtime(&tv.tv_sec);
  for (i = 0 ; i < n ; i++) {
      MPI_Comm_dup ( comm, &commList[i] );
      MPI_Comm_size ( commList[i], &size2 );
      MPI_Comm_rank ( commList[i], &rank2 );
  }
  gettimeofday(&tv1, &tz1);
  //tm1=localtime(&tv1.tv_sec);

  printf("Execution time = %d sec %d microseconds \n",
	 //(tm1->tm_hour - tm->tm_hour), (tm1->tm_min - tm->tm_min),
	 (tv1.tv_sec - tv.tv_sec),(tv1.tv_usec - tv.tv_usec));

  MPI_Finalize ();

  return ( 0 );
}
