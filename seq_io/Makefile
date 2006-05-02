
include ../Makefile.defs

all: send_recv isend_recv send_irecv sendrecv isend_irecv \
	put_startpost get_startpost put_winfence get_winfence \
	put_startpost_duplex put_winfence_duplex \
	get_winfence_duplex get_startpost_duplex 

nompi2: send_recv isend_recv send_irecv isend_irecv sendrecv 


# Two-sided operations
send_recv:	send_recv.h ping_pong.c ../include/latency.h ../utils/utils.c
	$(CC) -c ping_pong.c -o send_recv.o -DSEND_RECV $(CFLAGS)

isend_recv:	isend_recv.h ping_pong.c ../include/latency.h ../utils/utils.c
	$(CC) -c ping_pong.c -o isend_recv.o -DISEND_RECV $(CFLAGS)

send_irecv:	send_irecv.h ping_pong.c ../include/latency.h ../utils/utils.c
	$(CC) -c ping_pong.c -o send_irecv.o -DSEND_IRECV $(CFLAGS)

isend_irecv:	isend_irecv.h ping_pong.c ../include/latency.h ../utils/utils.c
	$(CC) -c ping_pong.c -o isend_irecv.o -DISEND_IRECV $(CFLAGS)

sendrecv:	sendrecv.h ping_pong.c ../include/latency.h ../utils/utils.c
	$(CC) -c ping_pong.c -o sendrecv.o -DSENDRECV $(CFLAGS)


# One-sided operations
put_startpost: put_startpost.h ping_pong.c ../include/latency.h ../utils/utils.c	
	$(CC) -c ping_pong.c -o put_startpost.o -DPUT_STARTPOST $(CFLAGS)	

get_startpost: get_startpost.h ping_pong.c ../include/latency.h ../utils/utils.c	
	$(CC) -c ping_pong.c -o get_startpost.o -DGET_STARTPOST $(CFLAGS)	

put_winfence:	put_winfence.h ping_pong.c ../include/latency.h ../utils/utils.c	
	$(CC) -c ping_pong.c -o put_winfence.o -DPUT_WINFENCE $(CFLAGS)	

get_winfence:	get_winfence.h ping_pong.c ../include/latency.h ../utils/utils.c	
	$(CC) -c ping_pong.c -o get_winfence.o -DGET_WINFENCE $(CFLAGS)	

put_startpost_duplex:	put_startpost_duplex.h ping_pong.c ../include/latency.h ../utils/utils.c	
	$(CC) -c ping_pong.c -o put_startpost_duplex.o -DPUT_STARTPOST_DUPLEX $(CFLAGS)	

put_winfence_duplex:	put_winfence_duplex.h ping_pong.c ../include/latency.h ../utils/utils.c	
	$(CC) -c ping_pong.c -o put_winfence_duplex.o -DPUT_WINFENCE_DUPLEX $(CFLAGS)	

get_startpost_duplex:	get_startpost_duplex.h ping_pong.c ../include/latency.h ../utils/utils.c	
	$(CC) -c ping_pong.c -o get_startpost_duplex.o -DGET_STARTPOST_DUPLEX $(CFLAGS)	

get_winfence_duplex:	get_winfence_duplex.h ping_pong.c ../include/latency.h ../utils/utils.c	
	$(CC) -c ping_pong.c -o get_winfence_duplex.o -DGET_WINFENCE_DUPLEX $(CFLAGS)	

clean:
	rm -f *.o *~ 
