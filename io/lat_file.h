#include "latency.h"

/* WRITE SPECIFICATIONS */

#ifdef SEQ_WRITE
#include "seq_write.h"
#endif

#ifdef SEQ_WRITEV
#include "seq_writev.h"
#endif

#ifdef SEQ_PWRITE
#include "seq_pwrite.h"
#endif

#ifdef AIO_WRITE
#include "aio_write.h"
#endif

#ifdef SEQ_FWRITE
#include "seq_fwrite.h"
#endif

#ifdef MPI_WRITE
#include "mpi_write.h"
#endif

#ifdef MPI_WRITE_AT
#include "mpi_write_at.h"
#endif

#ifdef MPI_WRITE_SHARED
#include "mpi_write_shared.h"
#endif

#ifdef MPI_IWRITE
#include "mpi_iwrite.h"
#endif

#ifdef MPI_IWRITE_AT
#include "mpi_iwrite_at.h"
#endif

#ifdef MPI_IWRITE_SHARED
#include "mpi_iwrite_shared.h"
#endif

/* READ SPECIFICATIONS */

#ifdef SEQ_READ
#include "seq_read.h"
#endif

#ifdef SEQ_READV
#include "seq_readv.h"
#endif

#ifdef SEQ_PREAD
#include "seq_pread.h"
#endif

#ifdef SEQ_FREAD
#include "seq_fread.h"
#endif

#ifdef AIO_READ
#include "aio_read.h"
#endif


#ifdef MPI_READ
#include "mpi_read.h"
#endif

#ifdef MPI_READ_AT
#include "mpi_read_at.h"
#endif

#ifdef MPI_READ_SHARED
#include "mpi_read_shared.h"
#endif

#ifdef MPI_IREAD
#include "mpi_iread.h"
#endif

#ifdef MPI_IREAD_AT
#include "mpi_iread_at.h"
#endif

#ifdef MPI_IREAD_SHARED
#include "mpi_iread_shared.h"
#endif

#ifdef MPI_WRITE_ALL
#include "mpi_write_all.h"
#endif

#ifdef MPI_WRITE_AT_ALL
#include "mpi_write_at_all.h"
#endif

