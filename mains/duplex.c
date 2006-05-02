/* 
   Testcode comparing the performance of two-sided operations using
   the communication channels in a duplex mode (MPI_Sendrecv) to 
   equivalent one-sided communication patterns.
*/

#include "latency.h"


int main ( int argc, char **argv)
{

  int  mynode, numnode;

  MPI_Init ( &argc, &argv );
  MPI_Comm_size ( MPI_COMM_WORLD, &numnode );
  MPI_Comm_rank ( MPI_COMM_WORLD, &mynode );

  /* Test 1: MPI_Put with MPI_Win_fence */
  if ( mynode == 0 )
      LAT_put_winfence_duplex ( MPI_COMM_WORLD,        /* communicator */
                                MPI_BYTE,              /* datatype */
                                MAX_LEN,               /* max. count number */
                                1,                     /* communication partner */
                                1,                     /* sender flag (yes/no) */
                                "MPI_COMM_WORLD, datatype MPI_Byte", 
                                "put.winfence.duplex", /* filename, NULL=stdout */
                                MPI_INFO_NULL);        /* options/hints */
  if ( mynode == 1 )
      LAT_put_winfence_duplex ( MPI_COMM_WORLD,        /* communicator */
                                MPI_BYTE,              /* datatype */
                                MAX_LEN,               /* max. count number */
                                0,                     /* communication partner */
                                0,                     /* sender flag (yes/no) */
                                "MPI_COMM_WORLD, datatype MPI_Byte", 
                                "put.winfence.duplex", /* filename, NULL=stdout */
                                MPI_INFO_NULL);        /* options/hints */

  MPI_Barrier ( MPI_COMM_WORLD );

  /* Test 2: MPI_Get with MPI_Win_fence */
  if ( mynode == 0 )
      LAT_get_winfence_duplex ( MPI_COMM_WORLD,         /* communicator */
                                MPI_BYTE,               /* datatype */
                                MAX_LEN,                /* max. count number */
                                1,                      /* communication partner */
                                1,                      /* sender flag (yes/no) */
                                "MPI_COMM_WORLD, datatype MPI_Byte", 
                                "get.winfence.duplex",  /* filename, NULL=stdout */
                                MPI_INFO_NULL);         /* options/hints */
  if ( mynode == 1 )
      LAT_get_winfence_duplex ( MPI_COMM_WORLD,         /* communicator */
                                MPI_BYTE,               /* datatype */
                                MAX_LEN,                /* max. count number */
                                0,                      /* communication partner */
                                0,                      /* sender flag (yes/no) */
                                "MPI_COMM_WORLD, datatype MPI_Byte", 
                                "get.winfence.duplex",  /* filename, NULL=stdout */
                                MPI_INFO_NULL);         /* options/hints */

  MPI_Barrier ( MPI_COMM_WORLD );

  /* Test 3: MPI_Put with MPI_Win_start/post */
  if ( mynode == 0 )
      LAT_put_startpost_duplex ( MPI_COMM_WORLD,         /* communicator */
                                 MPI_BYTE,               /* datatype */
                                 MAX_LEN,                /* max. count number */
                                 1,                      /* communication partner */
                                 1,                      /* sender flag (yes/no) */
                                 "MPI_COMM_WORLD, datatype MPI_Byte", 
                                 "put.startpost.duplex", /* filename, NULL=stdout */
                                 MPI_INFO_NULL);         /* options/hints */
  if ( mynode == 1 )
      LAT_put_startpost_duplex ( MPI_COMM_WORLD,         /* communicator */
                                 MPI_BYTE,               /* datatype */
                                 MAX_LEN,                /* max. count number */
                                 0,                      /* communication partner */
                                 0,                      /* sender flag (yes/no) */
                                 "MPI_COMM_WORLD, datatype MPI_Byte", 
                                 "put.startpost.duplex", /* filename, NULL=stdout */
                                 MPI_INFO_NULL);         /* options/hints */

  MPI_Barrier ( MPI_COMM_WORLD );

  /* Test 4: MPI_get with MPI_Win_start/post */
  if ( mynode == 0 )
      LAT_get_startpost_duplex ( MPI_COMM_WORLD,         /* communicator */
                                 MPI_BYTE,               /* datatype */
                                 MAX_LEN,                /* max. count number */
                                 1,                      /* communication partner */
                                 1,                      /* sender flag (yes/no) */
                                 "MPI_COMM_WORLD, datatype MPI_Byte", 
                                 "get.startpost.duplex", /* filename, NULL=stdout */
                                 MPI_INFO_NULL);         /* options/hints */
  if ( mynode == 1 )
      LAT_get_startpost_duplex ( MPI_COMM_WORLD,         /* communicator */
                                 MPI_BYTE,               /* datatype */
                                 MAX_LEN,                /* max. count number */
                                 0,                      /* communication partner */
                                 0,                      /* sender flag (yes/no) */
                                 "MPI_COMM_WORLD, datatype MPI_Byte", 
                                 "get.startpost.duplex", /* filename, NULL=stdout */
                                 MPI_INFO_NULL);         /* options/hints */


  /* Test MPI_Sendrecv */
  if ( mynode == 0 ) 
      LAT_sendrecv ( MPI_COMM_WORLD,        /* communicator */
                     MPI_BYTE,              /* datatype */
                     MAX_LEN,               /* max. count number */
                     1,                     /* communication partner */
                     1,                     /* sender flag (yes/no) */
                     "MPI_COMM_WORLD, datatype MPI_Byte", 
                     "sendrecv.out",        /* filename, NULL=stdout */
                     MPI_INFO_NULL);        /* options/hints */
  if ( mynode == 1 )
      LAT_sendrecv ( MPI_COMM_WORLD,        /* communicator */
                     MPI_BYTE,              /* datatype */
                     MAX_LEN,               /* max. count number */
                     0,                     /* communication partner */
                     0,                     /* sender flag (yes/no) */
                     "MPI_COMM_WORLD, datatype MPI_Byte", 
                     "sendrecv.out",        /* filename, NULL=stdout */
                     MPI_INFO_NULL);        /* options/hints */

  MPI_Barrier ( MPI_COMM_WORLD );


  MPI_Finalize ();
  return ( 0 ) ;
}
