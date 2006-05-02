/* Example main file executing a basic ping pong 
   benchmark on an dynamic intercomm using
   the datatype MPI_Byte

   This part of the code is executed by the spawned
   processes
*/

#include "latency.h"


int main ( int argc, char **argv)
{
  int  mynode, numnode;
  MPI_Comm comm;

  MPI_Init ( &argc, &argv );
  MPI_Comm_size ( MPI_COMM_WORLD, &numnode );
  MPI_Comm_rank ( MPI_COMM_WORLD, &mynode );

  comm = LAT_comm_spawn_son ();

  if ( mynode == 0 )
    LAT_send_recv ( comm, MPI_BYTE, MAX_LEN, 0, 0,
		    "Intercomm created with MPI_Comm_spawn, datatype MPI_Byte",
		    "spawn.out", MPI_INFO_NULL);


  MPI_Finalize ();
  return ( 0 ) ;
}
