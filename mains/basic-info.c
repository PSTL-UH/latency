/* Example main file executing a basic ping pong 
   benchmark on a static MPI_COMM_WORLD using
   the datatype MPI_Byte
*/

#include "latency.h"


int main ( int argc, char **argv)
{

  int  mynode, numnode;
  char key[]={"lat_info_testresult"};
  char value[]={"true"};
  MPI_Info info;

  MPI_Init ( &argc, &argv );
  MPI_Comm_size ( MPI_COMM_WORLD, &numnode );
  MPI_Comm_rank ( MPI_COMM_WORLD, &mynode );

  if ( mynode == 0 )
      LAT_isend_irecv ( MPI_COMM_WORLD, /* communicator */
			MPI_BYTE,       /* datatype */
			MAX_LEN,        /* max. count number */
			1,              /* communication partner */
			1,              /* sender flag (yes/no) */
			"MPI_COMM_WORLD, datatype MPI_Byte", 
			"isend_irecv.out", /* filename, NULL=stdout */
			MPI_INFO_NULL); /* options/hints */

  if ( mynode == 1 )
      LAT_isend_irecv ( MPI_COMM_WORLD, /* communicator */
			MPI_BYTE,       /* datatype */
			MAX_LEN,        /* max. count number */
			0,              /* communication partner */
			0,              /* sender flag (yes/no) */
			"MPI_COMM_WORLD, datatype MPI_Byte", 
			"isend_irecv.out", /* filename, NULL=stdout */
			MPI_INFO_NULL); /* options/hints */

  MPI_Barrier (MPI_COMM_WORLD );

  /* The same test enabling data verification */
  MPI_Info_create (&info);
  MPI_Info_set(info, key, value);

  if ( mynode == 0 )
      LAT_isend_irecv ( MPI_COMM_WORLD, /* communicator */
			MPI_BYTE,       /* datatype */
			MAX_LEN,        /* max. count number */
			1,              /* communication partner */
			1,              /* sender flag (yes/no) */
			"MPI_COMM_WORLD, datatype MPI_Byte", 
			"isend_irecv_overlap.out",/* filename, NULL=stdout */
			info);          /* options/hints */
  
  if ( mynode == 1 )
      LAT_isend_irecv ( MPI_COMM_WORLD, /* communicator */
			MPI_BYTE,       /* datatype */
			MAX_LEN,        /* max. count number */
			0,              /* communication partner */
			0,              /* sender flag (yes/no) */
			"MPI_COMM_WORLD, datatype MPI_Byte", 
			"isend_irecv_overlap.out", /* filename, NULL=stdout */
			info);          /* options/hints */
  

  MPI_Finalize ();
  return ( 0 ) ;
}
