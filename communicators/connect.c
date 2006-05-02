/* These routines create an inter-communicator
   using the MPI-2 connect/accept model.
   The first one has to be used by the
   processes offering the "service", the
   second one by the processes connecting
   to the other application.

   You can switch between two different setting
   in both routines, with or without the usage of
   name-publishing mechanism of MPI-2.
*/

#include "latency.h"
#include <string.h>

static int accepting=-1;
static int portopened=-1;
static char port_name[MPI_MAX_PORT_NAME];

#undef USE_NAMESERVICE

MPI_Comm LAT_comm_connect_brother ( MPI_Comm comm, MPI_Info info )
{
  MPI_Comm icomm;
  int ret;
  int rank;

  MPI_Comm_rank ( comm, &rank );

  if ( rank == 0 ) {
      memset ( port_name, 0, MPI_MAX_PORT_NAME );
      ret = MPI_Open_port ( info, port_name );
      if ( ret != MPI_SUCCESS ) {
          printf("%d: Error opening port, errcode = %d\n",rank, ret);
          MPI_Abort ( MPI_COMM_WORLD, ret );
      }
      portopened=1;
#ifdef USE_NAMESERVICE
      ret = MPI_Publish_name ( "PERFTEST", info, port_name );
      if ( ret != MPI_SUCCESS ) {
          printf("%d: Error publishing name, errcode = %d\n",rank, ret);
          MPI_Abort ( MPI_COMM_WORLD, ret );
      }
#else
      printf("%d: Open port returned port_name %s\n", rank, port_name );
#endif 
  }

  ret = MPI_Comm_accept ( port_name, info, 0, comm, &icomm);
  if ( ret != MPI_SUCCESS ){
       printf("%d: Error accepting connection, errcode = %d\n",
              (int)icomm, ret);
       MPI_Abort ( MPI_COMM_WORLD, ret );
  }
  
  /* Needed for the disconnect */
  accepting = 1;
  
  return ( icomm );
}




MPI_Comm LAT_comm_connect_sister ( MPI_Comm comm, MPI_Info info,
                                   int argc, char **argv )
{
  int ret, rank;
  MPI_Comm icomm;
  double t1, t2;

  MPI_Comm_rank ( comm, &rank );
  memset ( port_name, 0, MPI_MAX_PORT_NAME );

#ifdef USE_NAMESERVICE
  if ( rank == 0 ) {
      ret = MPI_Lookup_name("PERFTEST", info, port_name);
      if ( ret != MPI_SUCCESS ) {
          printf("%d: Error looking up service, ret = %d\n",rank, ret );
          MPI_Abort ( MPI_COMM_WORLD, ret );
      }
  }
#else 
  if ( argc < 2 ) {
      printf("Since you did not activate the usage of a name-service during"
	     "compiletime, please add the port-name as a startup option\n\n");
      printf("usage: connect-sister <port_name>\n");
      MPI_Abort ( MPI_COMM_WORLD, 1);
  }

  strcpy (port_name, argv[1]);
#endif 
    
  if ( rank == 0 )
      printf("connect-sister: connecting to port %s\n", port_name );
  
  t1  = MPI_Wtime();
  ret = MPI_Comm_connect ( port_name, info, 0, comm, &icomm);
  t2  = MPI_Wtime ();

#ifdef DYNCOMM_TIMING
  printf("%d: connecting to port %s took %lf seconds\n",
         rank, port_name, (t2-t1) );
#endif

  if ( ( ret != MPI_SUCCESS) || ( icomm == MPI_COMM_NULL)) {
      printf("%d: Error connecting to service, ret = %d\n",rank, ret );
      MPI_Abort ( MPI_COMM_WORLD, ret );
  }

  return ( icomm );
}


void LAT_comm_connect_end ( MPI_Comm *comm, MPI_Info info )
{

  MPI_Comm_disconnect ( comm );

  if ( accepting == 1 )
    {
        if ( portopened == 1 ) {
#ifdef USE_NAMESERVICE
            MPI_Unpublish_name ( "PERFTEST", info, port_name );
#endif
            MPI_Close_port ( port_name );
            portopened=-1;
        }
        accepting = -1;
    }
}
