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

#ifdef SEQ_READ
#include "seq_read.h"
#endif
