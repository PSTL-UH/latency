/* Example main file for one-sided operations
   using derived datatypes
*/

#include "latency.h"


int main ( int argc, char **argv)
{
  int  mynode, numnode;
  MPI_Datatype dat;
  int size;

  MPI_Init ( &argc, &argv );
  MPI_Comm_size ( MPI_COMM_WORLD, &numnode );
  MPI_Comm_rank ( MPI_COMM_WORLD, &mynode );

#ifdef HAVE_STARTPOST
  /* Testing a contiguous datatype consisting of two ints*/
  dat = LAT_twocontint ();
  MPI_Type_size ( dat, &size );
  if ( mynode == 0 )
      LAT_put_startpost ( MPI_COMM_WORLD,             /* communicator */
                          dat,                        /* datatype */
                          MAX_LEN/size,               /* max. count number */
                          1,                          /* communication partner */
                          1,                          /* sender flag (yes/no) */
                          "MPI_COMM_WORLD, contiguous datatype ", 
                          "put.startpost.twocontint", /* filename, NULL=stdout */
                          MPI_INFO_NULL)              /* options/hints */; 
  if ( mynode == 1 )
      LAT_put_startpost ( MPI_COMM_WORLD,             /* communicator */
                          dat,                        /* datatype */
                          MAX_LEN/size,               /* max. count number */
                          0,                          /* communication partner */
                          0,                          /* sender flag (yes/no) */
                          "MPI_COMM_WORLD, contiguous datatype ", 
                          "put.startpost.twocontint", /* filename, NULL=stdout */
                          MPI_INFO_NULL)              /* options/hints */; 

  MPI_Barrier ( MPI_COMM_WORLD );
  if ( mynode == 0 )
      LAT_get_startpost ( MPI_COMM_WORLD,             /* communicator */
                          dat,                        /* datatype */
                          MAX_LEN/size,               /* max. count number */
                          1,                          /* communication partner */
                          1,                          /* sender flag (yes/no) */
                          "MPI_COMM_WORLD, contiguous datatype ", 
                          "get.startpost.twocontint", /* filename, NULL=stdout */
                          MPI_INFO_NULL)              /* options/hints */; 
  if ( mynode == 1 )
      LAT_get_startpost ( MPI_COMM_WORLD,             /* communicator */
                          dat,                        /* datatype */
                          MAX_LEN/size,               /* max. count number */
                          0,                          /* communication partner */
                          0,                           /* sender flag (yes/no) */
                          "MPI_COMM_WORLD, contiguous datatype ", 
                          "get.startpost.twocontint", /* filename, NULL=stdout */
                          MPI_INFO_NULL)              /* options/hints */; 
  MPI_Barrier ( MPI_COMM_WORLD );
#endif

  if ( mynode == 0 )
      LAT_put_winfence ( MPI_COMM_WORLD,             /* communicator */
                         dat,                        /* datatype */
                         MAX_LEN/size,               /* max. count number */
                         1,                          /* communication partner */
                         1,                          /* sender flag (yes/no) */
                         "MPI_COMM_WORLD, contiguous datatype ", 
                         "put.winfence.twocontint", /* filename, NULL=stdout */
                         MPI_INFO_NULL              )/* options/hints */; 
  if ( mynode == 1 )
      LAT_put_winfence ( MPI_COMM_WORLD,             /* communicator */
                         dat,                        /* datatype */
                         MAX_LEN/size,               /* max. count number */
                         0,                          /* communication partner */
                         0,                          /* sender flag (yes/no) */
                         "MPI_COMM_WORLD, contiguous datatype ", 
                         "put.winfence.twocontint", /* filename, NULL=stdout */
                         MPI_INFO_NULL              )/* options/hints */; 

  MPI_Barrier ( MPI_COMM_WORLD );
  if ( mynode == 0 )
      LAT_get_winfence ( MPI_COMM_WORLD,             /* communicator */
                         dat,                        /* datatype */
                         MAX_LEN/size,               /* max. count number */
                         1,                          /* communication partner */
                         1,                          /* sender flag (yes/no) */
                         "MPI_COMM_WORLD, contiguous datatype ", 
                         "get.winfence.twocontint", /* filename, NULL=stdout */
                         MPI_INFO_NULL              )/* options/hints */; 
  if ( mynode == 1 )
      LAT_get_winfence ( MPI_COMM_WORLD,             /* communicator */
                         dat,                        /* datatype */
                         MAX_LEN/size,               /* max. count number */
                         0,                          /* communication partner */
                         0,                          /* sender flag (yes/no) */
                         "MPI_COMM_WORLD, contiguous datatype ", 
                         "get.winfence.twocontint",  /* filename, NULL=stdout */
                         MPI_INFO_NULL);             /* options/hints */

  MPI_Type_free ( &dat );
  MPI_Barrier ( MPI_COMM_WORLD );


  /* Testing vector type consisting of 2 ints and having no
     gap between each element */
  dat = LAT_twovecint_nogap ();
  MPI_Type_size ( dat, &size );
#ifdef HAVE_STARTPOST
  if ( mynode == 0 )
      LAT_put_startpost ( MPI_COMM_WORLD,             /* communicator */
                          dat,                        /* datatype */
                          MAX_LEN/size,               /* max. count number */
                          1,                          /* communication partner */
                          1,                          /* sender flag (yes/no) */
                          "MPI_COMM_WORLD, vector type without gap", 
                          "put.startpost.twovecint-nogap", /* filename, NULL=stdout */
                          MPI_INFO_NULL);             /* options/hints */
  if ( mynode == 1 )
      LAT_put_startpost ( MPI_COMM_WORLD,             /* communicator */
                          dat,                        /* datatype */
                          MAX_LEN/size,               /* max. count number */
                          0,                          /* communication partner */
                          0,                          /* sender flag (yes/no) */
                          "MPI_COMM_WORLD, vector type without gap", 
                          "put.startpost.twovecint-nogap", /* filename, NULL=stdout */
                          MPI_INFO_NULL);             /* options/hints */

  MPI_Barrier ( MPI_COMM_WORLD );
  if ( mynode == 0 )
      LAT_get_startpost ( MPI_COMM_WORLD,             /* communicator */
                          dat,                        /* datatype */
                          MAX_LEN/size,               /* max. count number */
                          1,                          /* communication partner */
                          1,                          /* sender flag (yes/no) */
                          "MPI_COMM_WORLD, vector type without gap", 
                          "get.startpost.twovecint-nogap",/* filename, NULL=stdout */ 
                          MPI_INFO_NULL               /* options/hints */);
  if ( mynode == 1 )
      LAT_get_startpost ( MPI_COMM_WORLD,             /* communicator */
                          dat,                        /* datatype */
                          MAX_LEN/size,               /* max. count number */
                          0,                          /* communication partner */
                          0,                          /* sender flag (yes/no) */
                          "MPI_COMM_WORLD, vector type without gap", 
                          "get.startpost.twovecint-nogap",/* filename, NULL=stdout */ 
                          MPI_INFO_NULL);             /* options/hints */

  MPI_Barrier ( MPI_COMM_WORLD );
#endif

  if ( mynode == 0 )
      LAT_put_winfence ( MPI_COMM_WORLD,             /* communicator */
                         dat,                        /* datatype */
                         MAX_LEN/size,               /* max. count number */
                         1,                          /* communication partner */
                         1,                          /* sender flag (yes/no) */
                         "MPI_COMM_WORLD, vector type without gap", 
                         "put.winfence.twovecint-nogap" /* filename, NULL=stdout */, 
                         MPI_INFO_NULL);             /* options/hints */
  if ( mynode == 1 )
      LAT_put_winfence ( MPI_COMM_WORLD,             /* communicator */
                         dat,                        /* datatype */
                         MAX_LEN/size,               /* max. count number */
                         0,                          /* communication partner */
                         0,                          /* sender flag (yes/no) */
                         "MPI_COMM_WORLD, vector type without gap", 
                         "put.winfence.twovecint-nogap"/* filename, NULL=stdout */, 
                         MPI_INFO_NULL);             /* options/hints */

  MPI_Barrier ( MPI_COMM_WORLD );
  if ( mynode == 0 )
      LAT_get_winfence ( MPI_COMM_WORLD,             /* communicator */
                         dat,                        /* datatype */
                         MAX_LEN/size,               /* max. count number */
                         1,                          /* communication partner */
                         1,                          /* sender flag (yes/no) */
                         "MPI_COMM_WORLD, vector type without gap", 
                         "get.winfence.twovecint-nogap"/* filename, NULL=stdout */, 
                         MPI_INFO_NULL);             /* options/hints */
  if ( mynode == 1 )
      LAT_get_winfence ( MPI_COMM_WORLD,             /* communicator */
                         dat,                        /* datatype */
                         MAX_LEN/size,               /* max. count number */
                         0,                          /* communication partner */
                         0,                          /* sender flag (yes/no) */
                         "MPI_COMM_WORLD, vector type without gap", 
                         "get.winfence.twovecint-nogap"/* filename, NULL=stdout */, 
                         MPI_INFO_NULL);             /* options/hints */

  MPI_Type_free ( &dat );
  MPI_Barrier ( MPI_COMM_WORLD );

  /* Testing vector type consisting of 2 ints and having a 
     gap of two ints between each element */
  dat = LAT_twovecint_twointgap ();
  MPI_Type_size ( dat, &size );
#ifdef HAVE_STARTPOST
  if ( mynode == 0 )
      LAT_put_startpost ( MPI_COMM_WORLD,             /* communicator */
                          dat,                        /* datatype */
                          MAX_LEN/size,               /* max. count number */
                          1,                          /* communication partner */
                          1,                          /* sender flag (yes/no) */
                          "MPI_COMM_WORLD, vector type with gap", 
                          "put.startpost.twovecint-gap", /* filename, NULL=stdout */
                          MPI_INFO_NULL);             /* options/hints */
  if ( mynode == 1 )
      LAT_put_startpost ( MPI_COMM_WORLD,             /* communicator */
                          dat,                        /* datatype */
                          MAX_LEN/size,               /* max. count number */
                          0,                          /* communication partner */
                          0,                          /* sender flag (yes/no) */
                          "MPI_COMM_WORLD, vector type with gap", 
                          "put.startpost.twovecint-gap", /* filename, NULL=stdout */
                          MPI_INFO_NULL);             /* options/hints */

  MPI_Barrier ( MPI_COMM_WORLD );
  if ( mynode == 0 )
      LAT_get_startpost ( MPI_COMM_WORLD,             /* communicator */
                          dat,                        /* datatype */
                          MAX_LEN/size,               /* max. count number */
                          1,                          /* communication partner */
                          1,                         /* sender flag (yes/no) */
                          "MPI_COMM_WORLD, vector type with gap", 
                          "get.startpost.twovecint-gap"/* filename, NULL=stdout */, 
                          MPI_INFO_NULL);             /* options/hints */
  if ( mynode == 1 )
      LAT_get_startpost ( MPI_COMM_WORLD,             /* communicator */
                          dat,                        /* datatype */
                          MAX_LEN/size,               /* max. count number */
                          0,                          /* communication partner */
                          0,                          /* sender flag (yes/no) */
                          "MPI_COMM_WORLD, vector type with gap", 
                          "get.startpost.twovecint-gap" /* filename, NULL=stdout */, 
                          MPI_INFO_NULL);             /* options/hints */

  MPI_Barrier ( MPI_COMM_WORLD );

#endif

  if ( mynode == 0 )
      LAT_put_winfence ( MPI_COMM_WORLD,             /* communicator */
                         dat,                        /* datatype */
                         MAX_LEN/size,               /* max. count number */
                         1,                          /* communication partner */
                         1,                          /* sender flag (yes/no) */
                         "MPI_COMM_WORLD, vector type with gap", 
                         "put.winfence.twovecint-gap"/* filename, NULL=stdout */, 
                         MPI_INFO_NULL);             /* options/hints */
  if ( mynode == 1 )
      LAT_put_winfence ( MPI_COMM_WORLD,             /* communicator */
                         dat,                        /* datatype */
                         MAX_LEN/size,               /* max. count number */
                         0,                          /* communication partner */
                         0,                          /* sender flag (yes/no) */
                         "MPI_COMM_WORLD, vector type with gap", 
                         "put.winfence.twovecint-gap", /* filename, NULL=stdout */
                         MPI_INFO_NULL);              /* options/hints */

  MPI_Barrier ( MPI_COMM_WORLD );
  if ( mynode == 0 )
      LAT_get_winfence ( MPI_COMM_WORLD,             /* communicator */
                         dat,                        /* datatype */
                         MAX_LEN/size,               /* max. count number */
                         1,                          /* communication partner */
                         1,                          /* sender flag (yes/no) */
                         "MPI_COMM_WORLD, vector type with gap", 
                         "get.winfence.twovecint-gap",/* filename, NULL=stdout */ 
                         MPI_INFO_NULL);              /* options/hints */
  if ( mynode == 1 )
      LAT_get_winfence ( MPI_COMM_WORLD,             /* communicator */
                         dat,                        /* datatype */
                         MAX_LEN/size,               /* max. count number */
                         0,                          /* communication partner */
                         0,                          /* sender flag (yes/no) */
                         "MPI_COMM_WORLD, vector type with gap", 
                         "get.winfence.twovecint-gap"/* filename, NULL=stdout */, 
                         MPI_INFO_NULL);             /* options/hints */

  MPI_Type_free ( &dat );
  MPI_Barrier ( MPI_COMM_WORLD );

  /* Testing a type struct consisting of 92 elements */
  dat = LAT_92elements ();
  MPI_Type_size ( dat, &size );
#ifdef HAVE_STARTPOST
  if ( mynode == 0 )
      LAT_put_startpost ( MPI_COMM_WORLD,             /* communicator */
                          dat,                        /* datatype */
                          MAX_LEN/size,               /* max. count number */
                          1,                          /* communication partner */
                          1,                          /* sender flag (yes/no) */
                          "MPI_COMM_WORLD, struct with 92 elements", 
                          "put.startpost.92elem",     /* filename, NULL=stdout */
                          MPI_INFO_NULL);             /* options/hints */
  if ( mynode == 1 )
      LAT_put_startpost ( MPI_COMM_WORLD,             /* communicator */
                          dat,                        /* datatype */
                          MAX_LEN/size,               /* max. count number */
                          0,                          /* communication partner */
                          0,                          /* sender flag (yes/no) */
                          "MPI_COMM_WORLD, struct with 92 elements", 
                          "put.startpost.92elem",     /* filename, NULL=stdout */
                          MPI_INFO_NULL);             /* options/hints */
  
  MPI_Barrier ( MPI_COMM_WORLD );
  if ( mynode == 0 )
      LAT_get_startpost ( MPI_COMM_WORLD,             /* communicator */
                          dat,                        /* datatype */
                          MAX_LEN/size,               /* max. count number */
                          1,                          /* communication partner */
                          1,                          /* sender flag (yes/no) */
                          "MPI_COMM_WORLD, struct with 92 elements", 
                          "get.startpost.92elem",     /* filename, NULL=stdout */
                          MPI_INFO_NULL);             /* options/hints */
  if ( mynode == 1 )
      LAT_get_startpost ( MPI_COMM_WORLD,             /* communicator */
                          dat,                        /* datatype */
                          MAX_LEN/size,               /* max. count number */
                          0,                          /* communication partner */
                          0,                          /* sender flag (yes/no) */
                          "MPI_COMM_WORLD, struct with 92 elements", 
                          "get.startpost.92elem",     /* filename, NULL=stdout */
                          MPI_INFO_NULL);             /* options/hints */

  MPI_Barrier ( MPI_COMM_WORLD );
#endif

  if ( mynode == 0 )
      LAT_put_winfence ( MPI_COMM_WORLD,             /* communicator */
                         dat,                        /* datatype */
                         MAX_LEN/size,               /* max. count number */
                         1,                          /* communication partner */
                         1,                          /* sender flag (yes/no) */
                         "MPI_COMM_WORLD, struct with 92 elements", 
                         "put.winfence.92elem",      /* filename, NULL=stdout */
                         MPI_INFO_NULL);             /* options/hints */
  if ( mynode == 1 )
      LAT_put_winfence ( MPI_COMM_WORLD,             /* communicator */
                         dat,                        /* datatype */
                         MAX_LEN/size,               /* max. count number */
                         0,                          /* communication partner */
                         0,                          /* sender flag (yes/no) */
                         "MPI_COMM_WORLD, struct with 92 elements", 
                         "put.winfence.92elem",      /* filename, NULL=stdout */
                         MPI_INFO_NULL);             /* options/hints */

  MPI_Barrier ( MPI_COMM_WORLD );
  if ( mynode == 0 )
      LAT_get_winfence ( MPI_COMM_WORLD,             /* communicator */
                         dat,                        /* datatype */
                         MAX_LEN/size,               /* max. count number */
                         1,                          /* communication partner */
                         1,                          /* sender flag (yes/no) */
                         "MPI_COMM_WORLD, struct with 92 elements", 
                         "get.winfence.92elem",      /* filename, NULL=stdout */
                         MPI_INFO_NULL);             /* options/hints */
  if ( mynode == 1 )
      LAT_get_winfence ( MPI_COMM_WORLD,             /* communicator */
                         dat,                        /* datatype */
                         MAX_LEN/size,               /* max. count number */
                         0,                          /* communication partner */
                         0,                          /* sender flag (yes/no) */
                         "MPI_COMM_WORLD, struct with 92 elements", 
                         "get.winfence.92elem",      /* filename, NULL=stdout */
                         MPI_INFO_NULL);             /* options/hints */
  
  MPI_Type_free ( &dat );
  MPI_Barrier ( MPI_COMM_WORLD );


  MPI_Finalize ();
  return ( 0 ) ;
}
