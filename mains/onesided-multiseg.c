/* 
   This version tests whether it is more efficient
   to put/get data in one single one-sided operation,
   or to split it into several operations.
*/

#include "latency.h"
#include <string.h>

int main ( int argc, char **argv)
{

  int  mynode, numnode;
  int i;
  char key[]={"lat_info_numseg"};
  char value[20];
  char filename[80];
  MPI_Info info;

  MPI_Init ( &argc, &argv );
  MPI_Comm_size ( MPI_COMM_WORLD, &numnode );
  MPI_Comm_rank ( MPI_COMM_WORLD, &mynode );

  memset ( filename, 0, sizeof(filename));
  MPI_Info_create ( &info );
 
  /* Test 1: MPI_Put with MPI_Win_fence */
  for ( i = 1; i < 17; i*=2 ) {
      sprintf(filename, "%s.%d", "put.winfence", i);
      sprintf(value,"%d",i);  
      MPI_Info_set (info,key,value);

      if ( mynode == 0 )
          LAT_put_winfence ( MPI_COMM_WORLD, /* communicator */
                             MPI_BYTE,       /* datatype */
                             MAX_LEN,        /* max. count number */
                             1,              /* communication partner */
                             1,              /* sender flag (yes/no) */
                             "MPI_COMM_WORLD, datatype MPI_Byte", 
                             filename,       /* filename, NULL=stdout */
                             info            /* options/hints */);
      if ( mynode == 1 )
          LAT_put_winfence ( MPI_COMM_WORLD, /* communicator */
                             MPI_BYTE,       /* datatype */
                             MAX_LEN,        /* max. count number */
                             0,              /* communication partner */
                             0,              /* sender flag (yes/no) */
                             "MPI_COMM_WORLD, datatype MPI_Byte", 
                             filename,       /* filename, NULL=stdout */
                             info            /* options/hints */);
      MPI_Barrier ( MPI_COMM_WORLD );
  }


  /* Test 2: MPI_Put with MPI_Win_start/post */
  for ( i = 1; i < 17; i*=2 ) {
      sprintf(filename, "%s.%d", "put.startpost", i);
      sprintf(value,"%d",i);  
      MPI_Info_set (info,key,value);

      if ( mynode == 0 )
          LAT_put_startpost ( MPI_COMM_WORLD, /* communicator */
                              MPI_BYTE,       /* datatype */
                              MAX_LEN,        /* max. count number */
                              1,              /* communication partner */
                              1,              /* sender flag (yes/no) */
                              "MPI_COMM_WORLD, datatype MPI_Byte", 
                              filename,       /* filename, NULL=stdout */
                              info            /* options/hints */);
      if ( mynode == 1 )
          LAT_put_startpost ( MPI_COMM_WORLD, /* communicator */
                              MPI_BYTE,       /* datatype */
                              MAX_LEN,        /* max. count number */
                              0,              /* communication partner */
                              0,              /* sender flag (yes/no) */
                              "MPI_COMM_WORLD, datatype MPI_Byte", 
                              filename,       /* filename, NULL=stdout */
                              info            /* options/hints */);
      MPI_Barrier ( MPI_COMM_WORLD );
  }

  /* Test 3: MPI_Get with MPI_Win_fence */
  for ( i = 1; i < 17; i*=2 ) {
      sprintf(filename, "%s.%d", "get.winfence", i);
      sprintf(value,"%d",i);  
      MPI_Info_set (info,key,value);

      if ( mynode == 0 )
          LAT_get_winfence ( MPI_COMM_WORLD, /* communicator */
                             MPI_BYTE,       /* datatype */
                             MAX_LEN,        /* max. count number */
                             1,              /* communication partner */
                             1,              /* sender flag (yes/no) */
                             "MPI_COMM_WORLD, datatype MPI_Byte", 
                             filename,       /* filename, NULL=stdout */
                             info            /* options/hints */);
      if ( mynode == 1 )
          LAT_get_winfence ( MPI_COMM_WORLD, /* communicator */
                             MPI_BYTE,       /* datatype */
                             MAX_LEN,        /* max. count number */
                             0,              /* communication partner */
                             0,              /* sender flag (yes/no) */
                           "MPI_COMM_WORLD, datatype MPI_Byte", 
                             filename,       /* filename, NULL=stdout */
                             info            /* options/hints */);
      MPI_Barrier ( MPI_COMM_WORLD );
  }
  
  /* Test 4: MPI_Get with MPI_Win_start/post */
  for ( i = 1; i < 17; i*=2 ) {
      sprintf(filename, "%s.%d", "get.startpost", i);
      sprintf(value,"%d",i);  
      MPI_Info_set (info,key,value);

      if ( mynode == 0 )
          LAT_get_startpost ( MPI_COMM_WORLD, /* communicator */
                              MPI_BYTE,       /* datatype */
                              MAX_LEN,        /* max. count number */
                              1,              /* communication partner */
                              1,              /* sender flag (yes/no) */
                              "MPI_COMM_WORLD, datatype MPI_Byte", 
                              filename,       /* filename, NULL=stdout */
                              info            /* options/hints */);
      if ( mynode == 1 )
          LAT_get_startpost ( MPI_COMM_WORLD, /* communicator */
                              MPI_BYTE,       /* datatype */
                              MAX_LEN,        /* max. count number */
                              0,              /* communication partner */
                              0,              /* sender flag (yes/no) */
                              "MPI_COMM_WORLD, datatype MPI_Byte", 
                              filename,       /* filename, NULL=stdout */
                              info            /* options/hints */);
      MPI_Barrier ( MPI_COMM_WORLD );
  }


  MPI_Finalize ();
  return ( 0 ) ;
}
