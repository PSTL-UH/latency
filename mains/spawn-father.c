/* Example main file executing a basic ping pong 
   benchmark on a static MPI_COMM_WORLD using
   the datatype MPI_Byte

   Testing ping-pong performance for an inter-communicator
   created with MPI_Comm_spawn
*/

#include "latency.h"


int main ( int argc, char **argv)
{
  int  mynode, numnode;
  MPI_Comm comm;

  MPI_Init ( &argc, &argv );
  MPI_Comm_size ( MPI_COMM_WORLD, &numnode );
  MPI_Comm_rank ( MPI_COMM_WORLD, &mynode );

  comm = LAT_comm_spawn_father ( "./spawn-son", 1, MPI_COMM_WORLD,
                                 MPI_INFO_NULL );

  if ( mynode == 0 )
    LAT_send_recv ( comm, MPI_BYTE, MAX_LEN,0, 1,
		    "Intercomm created by  MPI_Comm_spawn, datatype MPI_Byte", 
		    "spawn.out", MPI_INFO_NULL);

  MPI_Finalize ();
  return ( 0 ) ;
}
