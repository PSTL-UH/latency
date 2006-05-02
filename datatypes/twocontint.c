/* This file creates a contiguous datatype
   consisting of 2 contiguous integers
*/
#include "latency.h"

MPI_Datatype LAT_twocontint ()
{
  MPI_Datatype newtype;
  
  MPI_Type_contiguous ( 2, MPI_INT, &newtype );
  MPI_Type_commit ( &newtype );

  return ( newtype );
}

  
