
include ../Makefile.defs

all: 	spawn connect join

spawn: spawn.c ../include/latency.h
	$(CC) -c spawn.c $(CFLAGS)

connect: connect.c ../include/latency.h
	$(CC) -c connect.c $(CFLAGS)

join:	join.c ../include/latency.h
	$(CC) -c join.c $(CFLAGS) 

clean:
	rm -f *.o *~ 
