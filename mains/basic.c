/* Example main file executing a basic ping pong 
   benchmark on a static MPI_COMM_WORLD using
   the datatype MPI_Byte
*/

#include "latency.h"


int main ( int argc, char **argv)
{

  int  mynode, numnode;
  MPI_Init ( &argc, &argv );
  MPI_Comm_size ( MPI_COMM_WORLD, &numnode );
  MPI_Comm_rank ( MPI_COMM_WORLD, &mynode );

  if ( mynode == 0 )
      LAT_send_recv ( MPI_COMM_WORLD, /* communicator */
                      MPI_BYTE,       /* datatype */
                      MAX_LEN,        /* max. count number */
                      1,              /* communication partner */
                      1,              /* sender flag (yes/no) */
                      "MPI_COMM_WORLD, datatype MPI_Byte", 
                      NULL,           /* filename, NULL=stdout */
                      MPI_INFO_NULL); /* options/hints */

  if ( mynode == 1 )
      LAT_send_recv ( MPI_COMM_WORLD, /* communicator */
                      MPI_BYTE,       /* datatype */
                      MAX_LEN,        /* max. count number */
                      0,              /* communication partner */
                      0,              /* sender flag (yes/no) */
                      "MPI_COMM_WORLD, datatype MPI_Byte", 
                      NULL,           /* filename, NULL=stdout */
                      MPI_INFO_NULL); /* options/hints */

  MPI_Finalize ();
  return ( 0 ) ;
}
