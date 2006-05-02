/* Example main file executing a basic ping pong 
   benchmark on a dynamic communicator using
   the datatype MPI_Byte

   The communicator is created using the connect/accept
   model of the MPI-2 spec.
*/

#include "latency.h"


int main ( int argc, char **argv)
{
  int  mynode, numnode;
  MPI_Comm comm;

  
  MPI_Init ( &argc, &argv );
  MPI_Comm_size ( MPI_COMM_WORLD, &numnode );
  MPI_Comm_rank ( MPI_COMM_WORLD, &mynode );

  comm = LAT_comm_connect_brother (MPI_COMM_WORLD, MPI_INFO_NULL);

  if ( mynode == 0 )
    LAT_send_recv ( comm, MPI_BYTE, MAX_LEN, 0, 1,
		    "Intercomm created with MPI_Comm_connect/accept, datatype MPI_Byte",
		    NULL, MPI_INFO_NULL);

  /* Disconnect */
  LAT_comm_connect_end (&comm, MPI_INFO_NULL);

  MPI_Finalize ();
  return ( 0 ) ;
}
