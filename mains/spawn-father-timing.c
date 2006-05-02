/* Example main file executing a basic ping pong 
   benchmark on a static MPI_COMM_WORLD using
   the datatype MPI_Byte

   Testing ping-pong performance for an inter-communicator
   created with MPI_Comm_spawn
*/

#include "latency.h"

#define MAXCHILD      5
#define MAXREPETITION 5

int main ( int argc, char **argv)
{
  int  mynode, numnode;
  MPI_Comm comm;
  int i, j;
  MPI_Info info=MPI_INFO_NULL;
  char key[]={"host"};
  char value[]={"node4"};

  MPI_Init ( &argc, &argv );
  MPI_Comm_size ( MPI_COMM_WORLD, &numnode );
  MPI_Comm_rank ( MPI_COMM_WORLD, &mynode );

  MPI_Info_create (&info);
  MPI_Info_set(info, key, value);
  
  for (i=1; i<=MAXCHILD; i++) {
    for (j=0; j<MAXREPETITION; j++) {
      comm = LAT_comm_spawn_father ( "./spawn-son-timing", i, MPI_COMM_WORLD,
				     info);
      MPI_Comm_disconnect (&comm);
    }
  }


  MPI_Finalize ();
  return ( 0 ) ;
}
