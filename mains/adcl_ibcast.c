/* Example main file executing a basic ibcast 
   benchmark on a static MPI_COMM_WORLD using
   the datatype MPI_Byte
*/

#include <stdio.h>
#include "mpi.h"
#include "ADCL.h"
#include "nbc.h"
#include "latency.h"

int main ( int argc, char **argv)
{

  int  mynode, numnode;
  MPI_Init ( &argc, &argv );
  MPI_Comm_size ( MPI_COMM_WORLD, &numnode );
  MPI_Comm_rank ( MPI_COMM_WORLD, &mynode );

  ADCL_Init();

  printf("rank : %d\n",mynode);

  if ( mynode == 0 )
      LAT_coll_adcl_ibcast ( MPI_COMM_WORLD, /* communicator */
			     MPI_BYTE,       /* datatype */
			     1024*1024*2,        /* max. count number */
			     1,              /* sender flag (yes/no) */
			     "MPI_COMM_WORLD, datatype MPI_Byte", 
			     NULL,           /* filename, NULL=stdout */
			     MPI_INFO_NULL); /* options/hints */
  else
      LAT_coll_adcl_ibcast ( MPI_COMM_WORLD, /* communicator */
                             MPI_BYTE,       /* datatype */
                             1024*1024*2,        /* max. count number */
                             1,              /* sender flag (yes/no) */
                             "MPI_COMM_WORLD, datatype MPI_Byte",
                             NULL,           /* filename, NULL=stdout */
                             MPI_INFO_NULL); /* options/hints */

  ADCL_Finalize ();
  MPI_Finalize ();
  return ( 0 ) ;
}
