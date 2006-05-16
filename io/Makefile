
include ../Makefile.defs

all: 	seq_write seq_writev seq_pwrite aio_write seq_fwrite \
	mpi_write mpi_write_at mpi_write_shared \
        mpi_iwrite mpi_iwrite_at mpi_iwrite_shared \
	seq_read seq_fread \
	mpi_read mpi_iread


# Two-sided operations
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

seq_read: seq_read.h single_op.c ../include/latency.h ../utils/utils.c
	$(CC) -c single_op.c -o seq_read.o -DSEQ_READ $(CFLAGS)

seq_fread: seq_fread.h single_op.c ../include/latency.h ../utils/utils.c
	$(CC) -c single_op.c -o seq_fread.o -DSEQ_FREAD $(CFLAGS)

mpi_read: mpi_read.h single_op.c ../include/latency.h ../utils/utils.c
	$(CC) -c single_op.c -o mpi_read.o -DMPI_READ $(CFLAGS)

mpi_iread: mpi_iread.h single_op.c ../include/latency.h ../utils/utils.c
	$(CC) -c single_op.c -o mpi_iread.o -DMPI_IREAD $(CFLAGS)

clean:
	rm -f *.o *~ 
