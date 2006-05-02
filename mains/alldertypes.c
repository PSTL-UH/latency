/* Example main file callin a p2p benchmark for the 
   C-datatypes int, float and double 
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


  /* Testing a contiguous datatype consisting of two ints*/
  dat = LAT_twocontint ();
  MPI_Type_size ( dat, &size );
  if ( mynode == 0 )
      LAT_send_recv ( MPI_COMM_WORLD,   /* communicator */
                      dat,              /* datatype */
                      MAX_LEN/size,     /* max. count number */
                      1,                /* communication partner */
                      1,                /* sender flag (yes/no) */
                      "MPI_COMM_WORLD, contiguous datatype ", 
                      "twocontint.out", /* filename, NULL=stdout */
                      MPI_INFO_NULL );  /* options/hints */
  if ( mynode == 1 )
      LAT_send_recv ( MPI_COMM_WORLD,   /* communicator */
                      dat,              /* datatype */
                      MAX_LEN/size,     /* max. count number */
                      0,                /* communication partner */
                      0,                /* sender flag (yes/no) */
                      "MPI_COMM_WORLD, contiguous datatype ", 
                      "twocontint.out", /* filename, NULL=stdout */
                      MPI_INFO_NULL);   /* options/hints */

  MPI_Type_free ( &dat );
  MPI_Barrier ( MPI_COMM_WORLD );


  /* Testing vector type consisting of 2 ints and having no
     gap between each element */
  dat = LAT_twovecint_nogap ();
  MPI_Type_size ( dat, &size );

  if ( mynode == 0 )
      LAT_send_recv ( MPI_COMM_WORLD,   /* communicator */
                      dat,              /* datatype */
                      MAX_LEN/size,     /* max. count number */
                      1,                /* communication partner */
                      1,                /* sender flag (yes/no) */
                      "MPI_COMM_WORLD, vector type without gap", 
                      "twovecint-nogap.out", /* filename, NULL=stdout */
                      MPI_INFO_NULL);   /* options/hints */
  if ( mynode == 1 )
      LAT_send_recv ( MPI_COMM_WORLD,   /* communicator */
                      dat,              /* datatype */
                      MAX_LEN/size,     /* max. count number */
                      0,                /* communication partner */
                      0,                /* sender flag (yes/no) */
                      "MPI_COMM_WORLD, vector type without gap", 
                      "twovecint-nogap.out", /* filename, NULL=stdout */
                      MPI_INFO_NULL);   /* options/hints */

  MPI_Type_free ( &dat );
  MPI_Barrier ( MPI_COMM_WORLD );

  /* Testing vector type consisting of 2 ints and having a 
     gap of two ints between each element */
  dat = LAT_twovecint_twointgap ();
  MPI_Type_size ( dat, &size );

  if ( mynode == 0 )
      LAT_send_recv ( MPI_COMM_WORLD,   /* communicator */
                      dat,              /* datatype */
                      MAX_LEN/size,     /* max. count number */
                      1,                /* communication partner */
                      1,                /* sender flag (yes/no) */
                      "MPI_COMM_WORLD, vector type with gap", 
                      "twovecint-gap.out", /* filename, NULL=stdout */
                      MPI_INFO_NULL);   /* options/hints */
  if ( mynode == 1 )
      LAT_send_recv ( MPI_COMM_WORLD,   /* communicator */
                      dat,              /* datatype */
                      MAX_LEN/size,     /* max. count number */
                      0,                /* communication partner */
                      0,                /* sender flag (yes/no) */
                      "MPI_COMM_WORLD, vector type with gap", 
                      "twovecint-gap.out", /* filename, NULL=stdout */
                      MPI_INFO_NULL);   /* options/hints */

  MPI_Type_free ( &dat );
  MPI_Barrier ( MPI_COMM_WORLD );

  /* Testing a type struct consisting of 92 elements */
  dat = LAT_92elements ();
  MPI_Type_size ( dat, &size );

  if ( mynode == 0 )
      LAT_send_recv ( MPI_COMM_WORLD,   /* communicator */
                      dat,              /* datatype */
                      MAX_LEN/size,     /* max. count number */
                      1,                /* communication partner */
                      1,                /* sender flag (yes/no) */
                      "MPI_COMM_WORLD, struct with 92 elements", 
                      "92elements.out", /* filename, NULL=stdout */
                      MPI_INFO_NULL);   /* options/hints */
  if ( mynode == 1 )
      LAT_send_recv ( MPI_COMM_WORLD,   /* communicator */
                      dat,              /* datatype */
                      MAX_LEN/size,     /* max. count number */
                      0,                /* communication partner */
                      0,                /* sender flag (yes/no) */
                      "MPI_COMM_WORLD, struct with 92 elements", 
                      "92elements.out", /* filename, NULL=stdout */
                      MPI_INFO_NULL);   /* options/hints */

  MPI_Type_free ( &dat );


  MPI_Finalize ();
  return ( 0 ) ;
}
