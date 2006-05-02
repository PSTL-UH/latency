/* Example main file executing a basic ping pong 
   benchmark on a static MPI_COMM_WORLD using
   all implemented two-sided communication primitives.
*/

#include "latency.h"


int main ( int argc, char **argv)
{

  int  mynode, numnode;

  MPI_Init ( &argc, &argv );
  MPI_Comm_size ( MPI_COMM_WORLD, &numnode );
  MPI_Comm_rank ( MPI_COMM_WORLD, &mynode );

  /* Test MPI_Send with MPI_Recv */
  if ( mynode == 0 ) 
      LAT_send_recv ( MPI_COMM_WORLD, /* communicator */
                      MPI_BYTE, /* datatype */
                      MAX_LEN, /* max. count number */
                      1, /* communication partner */
                      1, /* sender flag (yes/no) */
                      "MPI_COMM_WORLD, datatype MPI_Byte", 
                      NULL, /* filename, NULL=stdout */
                      MPI_INFO_NULL); /* options/hints */
  if ( mynode == 1 )
      LAT_send_recv ( MPI_COMM_WORLD, /* communicator */
                      MPI_BYTE, /* datatype */
                      MAX_LEN, /* max. count number */
                      0, /* communication partner */
                      0, /* sender flag (yes/no) */
                      "MPI_COMM_WORLD, datatype MPI_Byte", 
                      NULL, /* filename, NULL=stdout */
                      MPI_INFO_NULL); /* options/hints */

  MPI_Barrier ( MPI_COMM_WORLD );

  /* Test MPI_Sendrecv */
  if ( mynode == 0 ) 
      LAT_sendrecv ( MPI_COMM_WORLD,   /* communicator */
                     MPI_BYTE,         /* datatype */
                     MAX_LEN,          /* max. count number */
                     1,                /* communication partner */
                     1,                /* sender flag (yes/no) */
                     "MPI_COMM_WORLD, datatype MPI_Byte", 
                     NULL,             /* filename, NULL=stdout */
                     MPI_INFO_NULL);   /* options/hints */
  if ( mynode == 1 )
      LAT_sendrecv ( MPI_COMM_WORLD,   /* communicator */
                     MPI_BYTE,         /* datatype */
                     MAX_LEN,          /* max. count number */
                     0,                /* communication partner */
                     0,                /* sender flag (yes/no) */
                     "MPI_COMM_WORLD, datatype MPI_Byte", 
                     NULL,             /* filename, NULL=stdout */
                     MPI_INFO_NULL);   /* options/hints */
  MPI_Barrier ( MPI_COMM_WORLD );

  /* Test MPI_Isend/MPI_Wait with MPI_Recv */

  if ( mynode == 0 ) 
      LAT_isend_recv ( MPI_COMM_WORLD,   /* communicator */
                       MPI_BYTE,         /* datatype */
                       MAX_LEN,          /* max. count number */
                       1,                /* communication partner */
                       1,                /* sender flag (yes/no) */
                       "MPI_COMM_WORLD, datatype MPI_Byte", 
                       NULL,             /* filename, NULL=stdout */
                       MPI_INFO_NULL);   /* options/hints */
  if ( mynode == 1 )
      LAT_isend_recv ( MPI_COMM_WORLD,   /* communicator */
                       MPI_BYTE,         /* datatype */
                       MAX_LEN,          /* max. count number */
                       0,                /* communication partner */
                       0,                /* sender flag (yes/no) */
                       "MPI_COMM_WORLD, datatype MPI_Byte", 
                       NULL,             /* filename, NULL=stdout */
                       MPI_INFO_NULL);   /* options/hints */
  MPI_Barrier ( MPI_COMM_WORLD );

  /* Test MPI_Send with MPI_Irecv/MPI_Wait */
  if ( mynode == 0 ) 
      LAT_send_irecv ( MPI_COMM_WORLD,   /* communicator */
                       MPI_BYTE,         /* datatype */
                       MAX_LEN,          /* max. count number */
                       1,                /* communication partner */
                       1,                /* sender flag (yes/no) */
                       "MPI_COMM_WORLD, datatype MPI_Byte", 
                       NULL,             /* filename, NULL=stdout */
                       MPI_INFO_NULL);   /* options/hints */
  if ( mynode == 1 )
      LAT_send_irecv ( MPI_COMM_WORLD,   /* communicator */
                       MPI_BYTE,         /* datatype */
                       MAX_LEN,          /* max. count number */
                       0,                /* communication partner */
                       0,                /* sender flag (yes/no) */
                       "MPI_COMM_WORLD, datatype MPI_Byte", 
                       NULL,             /* filename, NULL=stdout */
                       MPI_INFO_NULL);   /* options/hints */

  /* Test MPI_Isend/MPI_Irecv and MPI_Waitall */
  if ( mynode == 0 ) 
      LAT_isend_irecv ( MPI_COMM_WORLD,   /* communicator */
                        MPI_BYTE,         /* datatype */
                        MAX_LEN,          /* max. count number */
                        1,                /* communication partner */
                        1,                /* sender flag (yes/no) */
                        "MPI_COMM_WORLD, datatype MPI_Byte", 
                        NULL,             /* filename, NULL=stdout */
                        MPI_INFO_NULL);   /* options/hints */
  if ( mynode == 1 )
      LAT_isend_irecv ( MPI_COMM_WORLD,   /* communicator */
                        MPI_BYTE,         /* datatype */
                        MAX_LEN,          /* max. count number */
                        0,                /* communication partner */
                        0,                /* sender flag (yes/no) */
                        "MPI_COMM_WORLD, datatype MPI_Byte", 
                        NULL,             /* filename, NULL=stdout */
                        MPI_INFO_NULL);   /* options/hints */

  MPI_Finalize ();
  return ( 0 ) ;
}
