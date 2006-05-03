/* Example main file executing a basic file-write 
   test using integer datatypes
*/

#include "latency.h"


int main ( int argc, char **argv)
{

  int  mynode, numnode;
  MPI_Init ( &argc, &argv );
  MPI_Comm_size ( MPI_COMM_WORLD, &numnode );
  MPI_Comm_rank ( MPI_COMM_WORLD, &mynode );


  if ( mynode == 0 )
      LAT_seq_fwrite ( MPI_COMM_WORLD, /* communicator */
                      MPI_INT,       /* datatype */
                      MAX_LEN/4,     /* max. count number */
                      1,             /* active process (yes/no) */
                      "sequential, datatype MPI_INT", 
                      NULL,           /* filename, NULL=stdout */
		      "/data",         /* path for the resulting file */
		      "foutfile.txt",  /* name for the resulting file */ 
                      MPI_INFO_NULL); /* options/hints */

  if ( mynode == 1 )
      LAT_seq_fwrite ( MPI_COMM_WORLD, /* communicator */
                      MPI_INT,       /* datatype */
                      MAX_LEN/4,     /* max. count number */
                      0,             /* active process (yes/no) */
                      "sequential, datatype MPI_INT", 
                      NULL,           /* filename, NULL=stdout */
		      "/data",         /* path for the resulting file */
		      "foutfile.txt",  /* name for the resulting file */ 
                      MPI_INFO_NULL); /* options/hints */

  if ( mynode == 0 )
      LAT_seq_write ( MPI_COMM_WORLD, /* communicator */
                      MPI_INT,       /* datatype */
                      MAX_LEN/4,     /* max. count number */
                      1,             /* active process (yes/no) */
                      "sequential, datatype MPI_INT", 
                      NULL,           /* filename, NULL=stdout */
		      "/data",         /* path for the resulting file */
		      "outfile.txt",  /* name for the resulting file */ 
                      MPI_INFO_NULL); /* options/hints */

  if ( mynode == 1 )
      LAT_seq_write ( MPI_COMM_WORLD, /* communicator */
                      MPI_INT,       /* datatype */
                      MAX_LEN/4,     /* max. count number */
                      0,             /* active process (yes/no) */
                      "sequential, datatype MPI_INT", 
                      NULL,           /* filename, NULL=stdout */
		      "/data",         /* path for the resulting file */
		      "outfile.txt",  /* name for the resulting file */ 
                      MPI_INFO_NULL); /* options/hints */




  MPI_Finalize ();
  return ( 0 ) ;
}
