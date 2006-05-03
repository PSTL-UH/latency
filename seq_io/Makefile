
include ../Makefile.defs

all: seq_write


# Two-sided operations
seq_write: seq_write.h single_op.c ../include/latency.h ../utils/utils.c
	$(CC) -c single_op.c -o seq_write.o -DSEQ_WRITE $(CFLAGS)


clean:
	rm -f *.o *~ 
