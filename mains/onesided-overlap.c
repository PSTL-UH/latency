/* Example main file executing a basic ping pong 
   benchmark on a static MPI_COMM_WORLD using
   the datatype MPI_Byte
   
   This version tests one-sided operations
*/

#include "latency.h"


int main ( int argc, char **argv)
{
  int  mynode, numnode;
  MPI_Info info;
  char key[]={"lat_info_overlap"};
  char value[]={"true"};

  MPI_Init ( &argc, &argv );
  MPI_Comm_size ( MPI_COMM_WORLD, &numnode );
  MPI_Comm_rank ( MPI_COMM_WORLD, &mynode );

  MPI_Info_create (&info);
  MPI_Info_set(info, key, value);

  /* Test 1: MPI_Put with MPI_Win_fence */
  if ( mynode == 0 )
      LAT_put_winfence ( MPI_COMM_WORLD, /* communicator */
                         MPI_BYTE, /* datatype */
                         MAX_LEN, /* max. count number */
                         1, /* communication partner */
                         1, /* sender flag (yes/no) */
                         "MPI_COMM_WORLD, datatype MPI_Byte", 
                         "put-overlap.winfence", /* filename, NULL=stdout */
                         info); /* options/hints */
  if ( mynode == 1 )
      LAT_put_winfence ( MPI_COMM_WORLD, /* communicator */
                         MPI_BYTE, /* datatype */
                         MAX_LEN, /* max. count number */
                         0, /* communication partner */
                         0, /* sender flag (yes/no) */
                         "MPI_COMM_WORLD, datatype MPI_Byte", 
                         "put-overlap.winfence", /* filename, NULL=stdout */
                         info); /* options/hints */

  MPI_Barrier ( MPI_COMM_WORLD );

  /* Test 2: MPI_Get with MPI_Win_fence */
  if ( mynode == 0 )
      LAT_get_winfence ( MPI_COMM_WORLD,             /* communicator */
                         MPI_BYTE,                   /* datatype */
                         MAX_LEN,                    /* max. count number */
                         1,                          /* communication partner */
                         1,                          /* sender flag (yes/no) */
                         "MPI_COMM_WORLD, datatype MPI_Byte", 
                         "get-overlap.winfence",     /* filename, NULL=stdout */
                         info);             /* options/hints */
  if ( mynode == 1 )
      LAT_get_winfence ( MPI_COMM_WORLD,             /* communicator */
                         MPI_BYTE,                   /* datatype */
                         MAX_LEN,                    /* max. count number */
                         0,                          /* communication partner */
                         0,                          /* sender flag (yes/no) */
                         "MPI_COMM_WORLD, datatype MPI_Byte", 
                         "get-overlap.winfence",          /* filename, NULL=stdout */
                         info);             /* options/hints */

  MPI_Barrier ( MPI_COMM_WORLD );

  /* Test 3: MPI_Put with MPI_Win_start/post */
  if ( mynode == 0 )
      LAT_put_startpost ( MPI_COMM_WORLD,             /* communicator */
                          MPI_BYTE,                   /* datatype */
                          MAX_LEN,                    /* max. count number */
                          1,                          /* communication partner */
                          1,                          /* sender flag (yes/no) */
                          "MPI_COMM_WORLD, datatype MPI_Byte", 
                          "put-overlap.startpost",    /* filename, NULL=stdout */
                          info);             /* options/hints */
  if ( mynode == 1 )
      LAT_put_startpost ( MPI_COMM_WORLD,             /* communicator */
                          MPI_BYTE,                   /* datatype */
                          MAX_LEN,                    /* max. count number */
                          0,                          /* communication partner */
                          0,                          /* sender flag (yes/no) */
                          "MPI_COMM_WORLD, datatype MPI_Byte", 
                          "put-overlap.startpost",    /* filename, NULL=stdout */
                          info);             /* options/hints */

  MPI_Barrier ( MPI_COMM_WORLD );

  /* Test 4: MPI_get with MPI_Win_start/post */
  if ( mynode == 0 )
      LAT_get_startpost ( MPI_COMM_WORLD,             /* communicator */
                          MPI_BYTE,                   /* datatype */
                          MAX_LEN,                    /* max. count number */
                          1,                          /* communication partner */
                          1,                          /* sender flag (yes/no) */
                          "MPI_COMM_WORLD, datatype MPI_Byte", 
                          "get-overlap.startpost",    /* filename, NULL=stdout */
                          info);             /* options/hints */
  if ( mynode == 1 )
      LAT_get_startpost ( MPI_COMM_WORLD,             /* communicator */
                          MPI_BYTE,                   /* datatype */
                          MAX_LEN,                    /* max. count number */
                          0,                          /* communication partner */
                          0,                          /* sender flag (yes/no) */
                          "MPI_COMM_WORLD, datatype MPI_Byte", 
                          "get-overlap.startpost",    /* filename, NULL=stdout */
                          info);             /* options/hints */


  MPI_Finalize ();
  return ( 0 ) ;
}
