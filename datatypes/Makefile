
include ../Makefile.defs

all: 	twocontint.o twovecint_nogap.o twovecint_twointgap.o \
	92elements.o  hpl_types.o


twocontint.o:	twocontint.c ../include/latency.h 
	$(CC) -c twocontint.c $(CFLAGS)

twovecint_nogap.o:	twovecint_nogap.c ../include/latency.h 
	$(CC) -c twovecint_nogap.c $(CFLAGS)

twovecint_twointgap.o:	twovecint_twointgap.c ../include/latency.h 
	$(CC) -c twovecint_twointgap.c $(CFLAGS)

92elements.o:	92elements.c ../include/latency.h 
	$(CC) -c 92elements.c $(CFLAGS)

hpl_types.o:	hpl_types.c ../include/latency.h 
	$(CC) -c hpl_types.c $(CFLAGS)


clean:
	rm -f *.o *~ 
