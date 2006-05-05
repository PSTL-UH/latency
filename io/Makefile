
include ../Makefile.defs

all: seq_write seq_fwrite mpi_write mpi_iwrite seq_read seq_fread mpi_read


# Two-sided operations
seq_write: seq_write.h single_op.c ../include/latency.h ../utils/utils.c
	$(CC) -c single_op.c -o seq_write.o -DSEQ_WRITE $(CFLAGS)

seq_fwrite: seq_fwrite.h single_op.c ../include/latency.h ../utils/utils.c
	$(CC) -c single_op.c -o seq_fwrite.o -DSEQ_FWRITE $(CFLAGS)

mpi_write: mpi_write.h single_op.c ../include/latency.h ../utils/utils.c
	$(CC) -c single_op.c -o mpi_write.o -DMPI_WRITE $(CFLAGS)

mpi_iwrite: mpi_iwrite.h single_op.c ../include/latency.h ../utils/utils.c
	$(CC) -c single_op.c -o mpi_iwrite.o -DMPI_IWRITE $(CFLAGS)

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
