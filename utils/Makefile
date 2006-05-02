
include ../Makefile.defs

all: 	utils.o eddhr.o calc.o bTOmb interpolate diff

UTILFLAGS = 

utils.o: utils.c ../include/latency.h 
	$(CC) -c utils.c $(CFLAGS) $(UTILFLAGS)

eddhr.o: eddhr.c ../include/latency.h 
	$(CC) -c eddhr.c $(CFLAGS) $(UTILFLAGS)

calc.o: calc.c ../include/latency.h 
	$(CC) -c calc.c $(CFLAGS) $(UTILFLAGS)

bTOmb:	bTOmb.c
	$(CC) -o bTOmb bTOmb.c $(CFLAGS)
	mv bTOmb ../bin/

interpolate:	interpolate.c
	$(CC) -o interpolate interpolate.c $(CFLAGS)
	mv interpolate ../bin/

diff:	diff.c
	$(CC) -o diff diff.c $(CFLAGS) -lm
	mv diff ../bin/


clean:
	rm -f *.o *~ 
