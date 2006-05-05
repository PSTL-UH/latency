#include "latency.h"

#ifdef SEQ_WRITE
#include "seq_write.h"
#endif

#ifdef SEQ_FWRITE
#include "seq_fwrite.h"
#endif

#ifdef MPI_WRITE
#include "mpi_write.h"
#endif

#ifdef MPI_IWRITE
#include "mpi_iwrite.h"
#endif

#ifdef SEQ_READ
#include "seq_read.h"
#endif

#ifdef SEQ_FREAD
#include "seq_fread.h"
#endif

#ifdef MPI_READ
#include "mpi_read.h"
#endif

#ifdef MPI_IREAD
#include "mpi_iread.h"
#endif
