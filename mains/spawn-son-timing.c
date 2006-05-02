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
#ifdef PRINT_HOSTNAME
  char hostname[MAXHOSTNAMELEN];
#endif

  MPI_Init ( &argc, &argv );
  MPI_Comm_size ( MPI_COMM_WORLD, &numnode );
  MPI_Comm_rank ( MPI_COMM_WORLD, &mynode );

  comm = LAT_comm_spawn_son ();

#ifdef PRINT_HOSTNAME
  gethostname(hostname, MAXHOSTNAMELEN);
  printf("%d: on host %s\n", mynode, hostname);
#endif

  MPI_Comm_disconnect (&comm);
  MPI_Finalize ();
  return ( 0 ) ;
}
