/* Example main file executing a basic file-write 
   test using integer datatypes
*/

#include "latency.h"


int main ( int argc, char **argv)
{

  int  mynode, numnode;
  char key[]={"lat_info_overlap"};
  char value[]={"true"};
  MPI_Info info;

  MPI_Init ( &argc, &argv );
  MPI_Comm_size ( MPI_COMM_WORLD, &numnode );
  MPI_Comm_rank ( MPI_COMM_WORLD, &mynode );


 /* Test iwrite without overlap */

  if ( mynode == 0 )
      LAT_mpi_iwrite ( MPI_COMM_WORLD, /* communicator */
                      MPI_INT,       /* datatype */
                      MAX_LEN/4,     /* max. count number */
                      1,             /* active process (yes/no) */
                      "sequential, datatype MPI_INT", 
                      NULL,           /* filename, NULL=stdout */
		      "/data",         /* path for the resulting file */
		      "outfile.txt",  /* name for the resulting file */ 
                      MPI_INFO_NULL); /* options/hints */

  if ( mynode == 1 )
      LAT_mpi_iwrite ( MPI_COMM_WORLD, /* communicator */
		       MPI_INT,       /* datatype */
		       MAX_LEN/4,     /* max. count number */
		       0,             /* active process (yes/no) */
		       "sequential, datatype MPI_INT", 
		       NULL,           /* filename, NULL=stdout */
		       "/data",         /* path for the resulting file */
		       "outfile.txt",  /* name for the resulting file */ 
		       MPI_INFO_NULL); /* options/hints */
  sync();
  MPI_Barrier ( MPI_COMM_WORLD );

  MPI_Info_create (&info);
  MPI_Info_set(info, key, value);

 /* Test iwrite with  overlap */

  if ( mynode == 0 )
      LAT_mpi_iwrite ( MPI_COMM_WORLD, /* communicator */
                      MPI_INT,       /* datatype */
                      MAX_LEN/4,     /* max. count number */
                      1,             /* active process (yes/no) */
                      "sequential, datatype MPI_INT", 
                      NULL,          /* filename, NULL=stdout */
		      "/data",       /* path for the resulting file */
		      "outfile.txt", /* name for the resulting file */ 
                      info  );       /* options/hints */

  if ( mynode == 1 )
      LAT_mpi_iwrite ( MPI_COMM_WORLD, /* communicator */
		       MPI_INT,       /* datatype */
		       MAX_LEN/4,     /* max. count number */
		       0,             /* active process (yes/no) */
		       "sequential, datatype MPI_INT", 
		       NULL,          /* filename, NULL=stdout */
		       "/data",       /* path for the resulting file */
		       "outfile.txt", /* name for the resulting file */ 
		       info);         /* options/hints */

  MPI_Finalize ();
  return ( 0 ) ;
}
