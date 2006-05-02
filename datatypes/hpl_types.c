/* This file creates a derived datatype
   similar to the ones used in the HPL 
   benchmark
*/

#include "latency.h"


static char*  allocate_memory_double ( int numblocks, int blocklength, 
				       int distance );

MPI_Datatype LAT_hpl_type (int numblocks, int blocklength, int distance)
{
  MPI_Datatype newtype;
  double *dbuf, *dbufp;
  int i;
  MPI_Aint *adr, base;
  int *carr;
  MPI_Datatype *tarr;
  char *buf;

  adr  = (MPI_Aint *) malloc ( numblocks * sizeof ( MPI_Aint));
  carr = (int *) malloc ( numblocks * sizeof ( int ));
  tarr =  (MPI_Datatype *) malloc ( numblocks * sizeof ( MPI_Datatype ));
  if ( (adr == NULL ) || ( carr == NULL ) || ( tarr == NULL ))
    MPI_Abort ( MPI_COMM_WORLD, 1 );

  buf = allocate_memory_double ( numblocks, blocklength, distance );

  dbuf = ( double *) buf;
  dbufp = dbuf;

  MPI_Address ( dbuf, &base );
  for ( i = 0; i < numblocks; i++)
    {
      dbufp = dbuf + (i * (blocklength + distance));
      MPI_Address ( dbufp, &(adr[i]));
      adr[i] -= base;
      carr[i] = blocklength;
      tarr[i] = MPI_DOUBLE;
    }

  MPI_Type_struct ( numblocks, carr, adr, tarr, &newtype);
  MPI_Type_commit ( &newtype);

  free ( adr );
  free ( carr );
  free ( tarr );
  
  free ( buf );

  return ( newtype );
}

/*----------------------------------------------------------------------*/

static char* allocate_memory_double ( int numblocks, int blocklength, 
				      int distance )
{
  int tlength=0;
  char *buf;

  tlength = ((numblocks * blocklength) + ( (numblocks - 1)* distance )) 
    * sizeof ( double );
  
  buf = (char *) malloc ( tlength );
  if ( buf == NULL ) 
    {
      printf("allocate_memory_double: error allocating %d bytes of "
	     "memory\n", tlength );
      MPI_Abort ( MPI_COMM_WORLD,1 );
    }

  return ( buf );
}
  
