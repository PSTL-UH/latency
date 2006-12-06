
include ../Makefile.defs

all: 	seq_write seq_writev seq_pwrite aio_write seq_fwrite \
	mpi_write mpi_write_at mpi_write_shared \
        mpi_iwrite mpi_iwrite_at mpi_iwrite_shared \
	seq_read seq_readv seq_pread aio_read seq_fread \
	mpi_read mpi_read_at mpi_read_shared \
	mpi_iread mpi_iread_at mpi_iread_shared \
	mpi_write_all mpi_write_at_all mpi_write_ordered \
	mpi_write_all_begin mpi_write_at_all_begin mpi_write_ordered_begin

# write  operations
seq_write: seq_write.h single_op.c ../include/latency.h ../utils/utils.c
	$(CC) -c single_op.c -o seq_write.o -DSEQ_WRITE $(CFLAGS)

seq_writev: seq_writev.h single_op.c ../include/latency.h ../utils/utils.c
	$(CC) -c single_op.c -o seq_writev.o -DSEQ_WRITEV $(CFLAGS)

seq_pwrite: seq_pwrite.h single_op.c ../include/latency.h ../utils/utils.c
	$(CC) -c single_op.c -o seq_pwrite.o -DSEQ_PWRITE $(CFLAGS)

aio_write: aio_write.h single_op.c ../include/latency.h ../utils/utils.c
	$(CC) -c single_op.c -o aio_write.o -DAIO_WRITE $(CFLAGS)

seq_fwrite: seq_fwrite.h single_op.c ../include/latency.h ../utils/utils.c
	$(CC) -c single_op.c -o seq_fwrite.o -DSEQ_FWRITE $(CFLAGS)

mpi_write: mpi_write.h single_op.c ../include/latency.h ../utils/utils.c
	$(CC) -c single_op.c -o mpi_write.o -DMPI_WRITE $(CFLAGS)

mpi_write_at: mpi_write_at.h single_op.c ../include/latency.h ../utils/utils.c
	$(CC) -c single_op.c -o mpi_write_at.o -DMPI_WRITE_AT $(CFLAGS)

mpi_write_shared: mpi_write_shared.h single_op.c ../include/latency.h \
		../utils/utils.c
	$(CC) -c single_op.c -o mpi_write_shared.o -DMPI_WRITE_SHARED $(CFLAGS)

mpi_iwrite: mpi_iwrite.h single_op.c ../include/latency.h ../utils/utils.c
	$(CC) -c single_op.c -o mpi_iwrite.o -DMPI_IWRITE $(CFLAGS)

mpi_iwrite_at: mpi_iwrite_at.h single_op.c ../include/latency.h ../utils/utils.c
	$(CC) -c single_op.c -o mpi_iwrite_at.o -DMPI_IWRITE_AT $(CFLAGS)

mpi_iwrite_shared: mpi_iwrite_shared.h single_op.c ../include/latency.h \
		../utils/utils.c
	$(CC) -c single_op.c -o mpi_iwrite_shared.o -DMPI_IWRITE_SHARED $(CFLAGS)

# read  operations
seq_read: seq_read.h single_op.c ../include/latency.h ../utils/utils.c
	$(CC) -c single_op.c -o seq_read.o -DSEQ_READ $(CFLAGS)

seq_fread: seq_fread.h single_op.c ../include/latency.h ../utils/utils.c
	$(CC) -c single_op.c -o seq_fread.o -DSEQ_FREAD $(CFLAGS)

seq_readv: seq_readv.h single_op.c ../include/latency.h ../utils/utils.c
	$(CC) -c single_op.c -o seq_readv.o -DSEQ_READV $(CFLAGS)

seq_pread: seq_pread.h single_op.c ../include/latency.h ../utils/utils.c
	$(CC) -c single_op.c -o seq_pread.o -DSEQ_PREAD $(CFLAGS)

aio_read: aio_read.h single_op.c ../include/latency.h ../utils/utils.c
	$(CC) -c single_op.c -o aio_read.o -DAIO_READ $(CFLAGS)

mpi_read: mpi_read.h single_op.c ../include/latency.h ../utils/utils.c
	$(CC) -c single_op.c -o mpi_read.o -DMPI_READ $(CFLAGS)

mpi_read_at: mpi_read_at.h single_op.c ../include/latency.h ../utils/utils.c
	$(CC) -c single_op.c -o mpi_read_at.o -DMPI_READ_AT $(CFLAGS)

mpi_read_shared: mpi_read_shared.h single_op.c ../include/latency.h \
		../utils/utils.c
	$(CC) -c single_op.c -o mpi_read_shared.o -DMPI_READ_SHARED $(CFLAGS)

mpi_iread: mpi_iread.h single_op.c ../include/latency.h ../utils/utils.c
	$(CC) -c single_op.c -o mpi_iread.o -DMPI_IREAD $(CFLAGS)

mpi_iread_at: mpi_iread_at.h single_op.c ../include/latency.h ../utils/utils.c
	$(CC) -c single_op.c -o mpi_iread_at.o -DMPI_IREAD_AT $(CFLAGS)

mpi_iread_shared: mpi_iread_shared.h single_op.c ../include/latency.h \
		../utils/utils.c
	$(CC) -c single_op.c -o mpi_iread_shared.o -DMPI_IREAD_SHARED $(CFLAGS)

mpi_write_all: mpi_write_all.h coll_op.c ../include/latency.h ../utils/utils.c
	$(CC) -c coll_op.c -o mpi_write_all.o -DMPI_WRITE_ALL $(CFLAGS)

mpi_write_at_all: mpi_write_at_all.h coll_op.c ../include/latency.h ../utils/utils.c
	$(CC) -c coll_op.c -o mpi_write_at_all.o -DMPI_WRITE_AT_ALL $(CFLAGS)

mpi_write_ordered: mpi_write_ordered.h coll_op.c ../include/latency.h ../utils/utils.c
	$(CC) -c coll_op.c -o mpi_write_ordered.o -DMPI_WRITE_ORDERED $(CFLAGS)

mpi_write_all_begin: mpi_write_all_begin.h coll_op.c ../include/latency.h ../utils/utils.c
	$(CC) -c coll_op.c -o mpi_write_all_begin.o -DMPI_WRITE_ALL_BEGIN $(CFLAGS)

mpi_write_at_all_begin: mpi_write_at_all_begin.h coll_op.c ../include/latency.h ../utils/utils.c
	$(CC) -c coll_op.c -o mpi_write_at_all_begin.o -DMPI_WRITE_AT_ALL_BEGIN $(CFLAGS)

mpi_write_ordered_begin: mpi_write_ordered_begin.h coll_op.c ../include/latency.h ../utils/utils.c
	$(CC) -c coll_op.c -o mpi_write_ordered_begin.o -DMPI_WRITE_ORDERED_BEGIN $(CFLAGS)

clean:
	rm -f *.o *~ 
