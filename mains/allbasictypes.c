/* Example main file callin a p2p benchmark for the 
   C-datatypes int, float and double 
*/

#include "latency.h"


int main ( int argc, char **argv)
{

  int  mynode, numnode;

  MPI_Init ( &argc, &argv );
  MPI_Comm_size ( MPI_COMM_WORLD, &numnode );
  MPI_Comm_rank ( MPI_COMM_WORLD, &mynode );

  /* Testing MPI_INT */
  if ( mynode == 0 )
      LAT_send_recv ( MPI_COMM_WORLD,       /* communicator */
                      MPI_INT,              /* datatype */
                      MAX_LEN/sizeof(int),  /* max. count number */
                      1,                    /* communication partner */
                      1,                    /* sender flag (yes/no) */
                      "MPI_COMM_WORLD, datatype MPI_INT", 
                      NULL,                 /* filename, NULL=stdout */
                      MPI_INFO_NULL);       /* options/hints */
  if ( mynode == 1 )
      LAT_send_recv ( MPI_COMM_WORLD,       /* communicator */
                      MPI_INT,              /* datatype */
                      MAX_LEN/sizeof(int),  /* max. count number */
                      0,                    /* communication partner */
                      0,                    /* sender flag (yes/no) */
                      "MPI_COMM_WORLD, datatype MPI_INT", 
                      NULL,                 /* filename, NULL=stdout */
                      MPI_INFO_NULL);       /* options/hints */


  MPI_Barrier ( MPI_COMM_WORLD );

  /* Testing MPI_FLOAT */
  if ( mynode == 0 )
      LAT_send_recv ( MPI_COMM_WORLD,       /* communicator */
                      MPI_FLOAT,            /* datatype */
                      MAX_LEN/sizeof(float),/* max. count number */
                      1,                    /* communication partner */
                      1,                    /* sender flag (yes/no) */
                      "MPI_COMM_WORLD, datatype MPI_FLOAT", 
                      NULL,                 /* filename, NULL=stdout */
                      MPI_INFO_NULL);       /* options/hints */
  if ( mynode == 1 )
      LAT_send_recv ( MPI_COMM_WORLD,       /* communicator */
                      MPI_FLOAT,            /* datatype */
                      MAX_LEN/sizeof(float),/* max. count number */
                      0,                    /* communication partner */
                      0,                    /* sender flag (yes/no) */
                      "MPI_COMM_WORLD, datatype MPI_FLOAT", 
                      NULL,                 /* filename, NULL=stdout */
                      MPI_INFO_NULL);       /* options/hints */

  MPI_Barrier ( MPI_COMM_WORLD );

  /* Testing MPI_DOUBLES */
  if ( mynode == 0 )
      LAT_send_recv ( MPI_COMM_WORLD,       /* communicator */
                      MPI_DOUBLE,           /* datatype */
                      MAX_LEN/sizeof(double),/* max. count number */
                      1,                    /* communication partner */
                      1,                    /* sender flag (yes/no) */
                      "MPI_COMM_WORLD, datatype MPI_DOUBLE", 
                      NULL,                 /* filename, NULL=stdout */
                      MPI_INFO_NULL);       /* options/hints */
  if ( mynode == 1 )
      LAT_send_recv ( MPI_COMM_WORLD,       /* communicator */
                      MPI_DOUBLE,           /* datatype */
                      MAX_LEN/sizeof(double),/* max. count number */
                      0,                    /* communication partner */
                      0,                    /* sender flag (yes/no) */
                      "MPI_COMM_WORLD, datatype MPI_DOUBLE", 
                      NULL,                 /* filename, NULL=stdout */
                      MPI_INFO_NULL);       /* options/hints */

  MPI_Finalize ();
  return ( 0 ) ;
}
