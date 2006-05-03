
include ../Makefile.defs

all: seq_write seq_fwrite mpi_write


# Two-sided operations
seq_write: seq_write.h single_op.c ../include/latency.h ../utils/utils.c
	$(CC) -c single_op.c -o seq_write.o -DSEQ_WRITE $(CFLAGS)

seq_fwrite: seq_fwrite.h single_op.c ../include/latency.h ../utils/utils.c
	$(CC) -c single_op.c -o seq_fwrite.o -DSEQ_FWRITE $(CFLAGS)

mpi_write: mpi_write.h single_op.c ../include/latency.h ../utils/utils.c
	$(CC) -c single_op.c -o mpi_write.o -DMPI_WRITE $(CFLAGS)

clean:
	rm -f *.o *~ 
