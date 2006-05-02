/* This file contains two routines used for creating 
   an Inter-communicator using MPI_Comm_spawn. The first
   routine is needed by the father (the processes executing
   the Comm_spawn), the second one by the children (the 
   spawned processes themselves.
*/

#include "latency.h"

MPI_Comm LAT_comm_spawn_father ( char *cmd, int size, MPI_Comm comm, 
                                 MPI_Info info)
{
  int ret, errcode, rank, root=0;
  MPI_Comm icomm;
  double t1, t2;
#ifdef DYNCOMM_TIMING
  double maxt;
#endif

  MPI_Comm_rank ( comm, &rank );
  
  /* Spawn an additional process */
  t1  = MPI_Wtime();
  ret = MPI_Comm_spawn(cmd, MPI_ARGV_NULL, size, info, 
                       root, comm, &icomm, &errcode );
  t2  = (MPI_Wtime() - t1);

#ifdef DYNCOMM_TIMING
  MPI_Reduce (&t2, &maxt, 1, MPI_DOUBLE, MPI_MAX, root, comm);
  if ( rank == root ) {
      printf("%d: spawning %d processes took %lf milliseconds\n",
             rank, size, maxt*100 );
  }
#endif
       
  if ( ret != MPI_SUCCESS ) {
      printf("Error spawning a process, errcode = %d\n",
             ret);
      MPI_Abort ( MPI_COMM_WORLD, ret );
  }

  return (icomm );
}


MPI_Comm LAT_comm_spawn_son ( void )
{
  int ret;
  MPI_Comm icomm;

  ret = MPI_Comm_get_parent ( &icomm );
  if ( ( ret != MPI_SUCCESS ) || ( icomm == MPI_COMM_NULL )) {
      printf("Error retrieving parents, ret = %d\n", ret );
      MPI_Abort ( MPI_COMM_WORLD, ret );
  }

  return ( icomm );
}

