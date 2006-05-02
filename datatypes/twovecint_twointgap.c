/* This file creates a contiguous datatype
   consisting of 2 contiguous integers
*/

#include "latency.h"

MPI_Datatype LAT_twovecint_twointgap ()
{
  MPI_Datatype newtype;
  
  MPI_Type_vector ( 1,2,4, MPI_INT, &newtype );
  MPI_Type_commit ( &newtype );

  return ( newtype );
}

  
