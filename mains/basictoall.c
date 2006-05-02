/* Example main file executing a basic ping pong 
   between rank 0 and every other rank of 
   MPI_COMM_WORLD
*/

#include "latency.h"


int main ( int argc, char **argv)
{

  int  mynode, numnode;
  int i;
  char mystr[1024];

  MPI_Init ( &argc, &argv );
  MPI_Comm_size ( MPI_COMM_WORLD, &numnode );
  MPI_Comm_rank ( MPI_COMM_WORLD, &mynode );

  for ( i = 1; i < numnode; i++) {
    sprintf(mystr, "MPI_COMM_WORLD to %d, datatype MPI_BYTE", i );

    if ( mynode == 0 )
        LAT_send_recv ( MPI_COMM_WORLD,             /* communicator */
                        MPI_BYTE,                   /* datatype */
                        MAX_LEN,                    /* max. count number */
                        i,                          /* communication partner */
                        1,                          /* sender flag (yes/no) */
                        mystr,                      /* comment */
                        NULL,                       /* filename, NULL=stdout */
                        MPI_INFO_NULL);             /* options/hints */
    if ( mynode == i )
        LAT_send_recv ( MPI_COMM_WORLD,             /* communicator */
                        MPI_BYTE,                   /* datatype */
                        MAX_LEN,                    /* max. count number */
                        0,                          /* communication partner */
                        0,                          /* sender flag (yes/no) */
                        mystr,                      /* comment */
                        NULL,                       /* filename, NULL=stdout */
                        MPI_INFO_NULL);             /* options/hints */

    MPI_Barrier ( MPI_COMM_WORLD );
  }

  MPI_Finalize ();
  return ( 0 ) ;
}
