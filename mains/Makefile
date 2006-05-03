
include ../Makefile.defs

all: basic basic-info basictoall allbasictypes alltransfer alldertypes \
	spawn connect onesided onesided-multiseg onesided-alldertypes \
	duplex join-brother join-sister

nompi2: basic basictoall allbasictypes alltransfer alldertypes 
io:	write_test

# Two-sided operations
basic.o: basic.c ../include/latency.h 
	$(CC) -c basic.c $(CFLAGS)

basic: basic.o ../utils/utils.o ../utils/eddhr.o ../utils/calc.o \
		../transfer/send_recv.o 
	$(CC) -o basic basic.o $(CFLAGS)  \
		../transfer/send_recv.o \
		../utils/eddhr.o ../utils/calc.o        \
		../utils/utils.o $(LDFLAGS)
	mv basic $(BIN_DIR)

basic-info.o: basic-info.c ../include/latency.h 
	$(CC) -c basic-info.c $(CFLAGS)

basic-info: basic-info.o ../utils/utils.o ../utils/eddhr.o ../utils/calc.o \
		../transfer/isend_irecv.o 
	$(CC) -o basic-info basic-info.o $(CFLAGS)  \
		../transfer/isend_irecv.o \
		../utils/eddhr.o ../utils/calc.o        \
		../utils/utils.o $(LDFLAGS)
	mv basic-info $(BIN_DIR)

basictoall.o: basictoall.c ../include/latency.h 
	$(CC) -c basictoall.c $(CFLAGS)

basictoall: basictoall.o ../utils/utils.o ../utils/eddhr.o ../utils/calc.o \
		../transfer/send_recv.o
	$(CC) -o basictoall basictoall.o $(CFLAGS)  \
		../transfer/send_recv.o \
		../utils/eddhr.o ../utils/calc.o        \
		../utils/utils.o $(LDFLAGS)
	mv basictoall $(BIN_DIR)

allbasictypes.o: allbasictypes.c ../include/latency.h 
	$(CC) -c allbasictypes.c $(CFLAGS)

allbasictypes: allbasictypes.o ../utils/utils.o ../utils/eddhr.o \
		../utils/calc.o ../transfer/send_recv.o
	$(CC) -o allbasictypes allbasictypes.o $(CFLAGS)  \
		../transfer/send_recv.o \
		../utils/eddhr.o ../utils/calc.o        \
		../utils/utils.o $(LDFLAGS)
	mv allbasictypes $(BIN_DIR)

alltransfer.o: alltransfer.c ../include/latency.h 
	$(CC) -c alltransfer.c $(CFLAGS)

alltransfer: alltransfer.o ../utils/utils.o ../transfer/send_recv.o \
		../transfer/sendrecv.o ../transfer/isend_recv.o \
		../transfer/send_irecv.o ../transfer/isend_irecv.o \
		../utils/eddhr.o ../utils/calc.o
	$(CC) -o alltransfer alltransfer.o $(CFLAGS)  \
		../transfer/send_recv.o \
		../utils/eddhr.o ../utils/calc.o        \
		../utils/utils.o \
		../transfer/sendrecv.o \
		../transfer/send_irecv.o \
		../transfer/isend_irecv.o \
		../transfer/isend_recv.o $(LDFLAGS)
	mv alltransfer $(BIN_DIR)

# Testing derived datatypes 

alldertypes.o: alldertypes.c ../include/latency.h 
	$(CC) -c alldertypes.c $(CFLAGS)

alldertypes: alldertypes.o ../utils/utils.o ../transfer/send_recv.o \
		../datatypes/twocontint.o ../datatypes/twovecint_nogap.o \
		../datatypes/twovecint_twointgap.o ../datatypes/92elements.o \
		../utils/eddhr.o ../utils/calc.o
	$(CC) -o alldertypes alldertypes.o $(CFLAGS)  \
		../transfer/send_recv.o \
		../utils/eddhr.o ../utils/calc.o        \
		../utils/utils.o \
		../datatypes/twocontint.o \
		../datatypes/twovecint_nogap.o \
		../datatypes/twovecint_twointgap.o \
		../datatypes/92elements.o \
		$(LDFLAGS) 
	mv alldertypes $(BIN_DIR)

# Testing one-sided operations 

onesided.o: onesided.c ../include/latency.h 
	$(CC) -c onesided.c $(CFLAGS)

onesided: onesided.o ../utils/utils.o ../utils/eddhr.o \
		../utils/calc.o ../transfer/put_winfence.o \
		../transfer/get_winfence.o ../transfer/put_startpost.o \
		../transfer/get_startpost.o
	$(CC) -o onesided onesided.o $(CFLAGS)  \
		../transfer/put_winfence.o \
		../transfer/get_winfence.o \
		../transfer/put_startpost.o \
		../transfer/get_startpost.o \
		../utils/utils.o ../utils/eddhr.o ../utils/calc.o $(LDFLAGS)
	mv onesided $(BIN_DIR)

onesided-overlap.o: onesided-overlap.c ../include/latency.h 
	$(CC) -c onesided-overlap.c $(CFLAGS)

onesided-overlap: onesided-overlap.o ../utils/utils.o ../utils/eddhr.o \
		../utils/calc.o ../transfer/put_winfence.o \
		../transfer/get_winfence.o ../transfer/put_startpost.o \
		../transfer/get_startpost.o
	$(CC) -o onesided-overlap onesided-overlap.o $(CFLAGS)  \
		../transfer/put_winfence.o \
		../transfer/get_winfence.o \
		../transfer/put_startpost.o \
		../transfer/get_startpost.o \
		../utils/utils.o ../utils/eddhr.o ../utils/calc.o $(LDFLAGS)
	mv onesided-overlap $(BIN_DIR)

onesided-multiseg.o: onesided-multiseg.c ../include/latency.h 
	$(CC) -c onesided-multiseg.c $(CFLAGS)

onesided-multiseg: onesided-multiseg.o ../utils/utils.o \
		../utils/eddhr.o ../utils/calc.o\
		../transfer/put_winfence.o \
		../transfer/put_startpost.o \
		../transfer/get_winfence.o \
		../transfer/get_startpost.o 
	$(CC) -o onesided-multiseg onesided-multiseg.o $(CFLAGS)  \
		../transfer/put_winfence.o \
		../transfer/put_startpost.o \
		../transfer/get_winfence.o \
		../transfer/get_startpost.o \
		../utils/utils.o ../utils/eddhr.o ../utils/calc.o $(LDFLAGS)
	mv onesided-multiseg $(BIN_DIR)


onesided-alldertypes.o: onesided-alldertypes.c ../include/latency.h 
	$(CC) -c onesided-alldertypes.c $(CFLAGS)

onesided-alldertypes: onesided-alldertypes.o ../utils/utils.o \
		../utils/eddhr.o ../utils/calc.o \
		../transfer/put_startpost.o    \
		../datatypes/twocontint.o      \
		../datatypes/twovecint_nogap.o \
		../datatypes/twovecint_twointgap.o \
		../datatypes/92elements.o
	$(CC) -o onesided-alldertypes onesided-alldertypes.o $(CFLAGS)  \
		../transfer/put_startpost.o    \
		../transfer/get_startpost.o    \
		../transfer/put_winfence.o    \
		../transfer/get_winfence.o    \
		../datatypes/twocontint.o      \
		../datatypes/twovecint_nogap.o \
		../datatypes/twovecint_twointgap.o \
		../datatypes/92elements.o \
		../utils/utils.o ../utils/eddhr.o ../utils/calc.o $(LDFLAGS)
	mv onesided-alldertypes $(BIN_DIR)


duplex.o: duplex.c ../include/latency.h 
	$(CC) -c duplex.c $(CFLAGS)

duplex: duplex.o ../utils/utils.o ../utils/eddhr.o ../utils/calc.o \
		../transfer/put_winfence_duplex.o \
		../transfer/get_winfence_duplex.o  \
		../transfer/put_startpost_duplex.o \
		../transfer/get_startpost_duplex.o \
		../transfer/sendrecv.o
	$(CC) -o duplex duplex.o $(CFLAGS)         \
		../transfer/put_winfence_duplex.o  \
		../transfer/get_winfence_duplex.o  \
		../transfer/put_startpost_duplex.o \
		../transfer/get_startpost_duplex.o \
		../transfer/sendrecv.o \
		../utils/utils.o ../utils/eddhr.o ../utils/calc.o $(LDFLAGS)
	mv duplex $(BIN_DIR)


# Testing dynamic communicators 

spawn-father.o: spawn-father.c ../include/latency.h
	$(CC) -c spawn-father.c $(CFLAGS)
spawn-father:	spawn-father.o ../transfer/send_recv.o \
		../communicators/spawn.o ../utils/eddhr.o ../utils/calc.o
	$(CC) -o spawn-father spawn-father.o ../transfer/send_recv.o \
		../utils/eddhr.o ../utils/calc.o        \
		../utils/utils.o ../communicators/spawn.o $(LDFLAGS)

spawn-son.o: spawn-son.c ../include/latency.h
	$(CC) -c spawn-son.c $(CFLAGS)
spawn-son:	spawn-son.o ../transfer/send_recv.o \
		../communicators/spawn.o ../utils/eddhr.o ../utils/calc.o
	$(CC) -o spawn-son spawn-son.o ../transfer/send_recv.o \
		../utils/eddhr.o ../utils/calc.o        \
		../utils/utils.o ../communicators/spawn.o $(LDFLAGS)

spawn:  spawn-father spawn-son
	mv spawn-father spawn-son $(BIN_DIR)

spawn-father-timing.o: spawn-father-timing.c ../include/latency.h
	$(CC) -c spawn-father-timing.c $(CFLAGS)
spawn-father-timing:	spawn-father-timing.o ../communicators/spawn.o 	
	$(CC) -o spawn-father-timing spawn-father-timing.o \
	../communicators/spawn.o $(LDFLAGS)

spawn-son-timing.o: spawn-son-timing.c ../include/latency.h
	$(CC) -c spawn-son-timing.c $(CFLAGS)
spawn-son-timing:   spawn-son-timing.o ../communicators/spawn.o 
	$(CC) -o spawn-son-timing spawn-son-timing.o \
	../utils/utils.o ../communicators/spawn.o $(LDFLAGS)

spawn-timing: spawn-father-timing spawn-son-timing
	mv spawn-father-timing spawn-son-timing $(BIN_DIR)



connect-brother.o: connect-brother.c ../include/latency.h
	$(CC) -c connect-brother.c $(CFLAGS)
connect-brother: connect-brother.o ../transfer/send_recv.o \
		../utils/eddhr.o ../utils/calc.o        \
		../communicators/connect.o
	$(CC) -tcpip -o connect-brother connect-brother.o ../transfer/send_recv.o \
		../utils/eddhr.o ../utils/calc.o        \
		../utils/utils.o ../communicators/connect.o $(LDFLAGS)

connect-sister.o: connect-sister.c ../include/latency.h
	$(CC) -c connect-sister.c $(CFLAGS)
connect-sister:	connect-sister.o ../transfer/send_recv.o \
		../utils/eddhr.o ../utils/calc.o        \
		../communicators/connect.o
	$(CC) -tcpip -o connect-sister connect-sister.o ../transfer/send_recv.o \
		../utils/eddhr.o ../utils/calc.o        \
		../utils/utils.o ../communicators/connect.o $(LDFLAGS)

connect:  connect-brother connect-sister
	mv connect-brother connect-sister $(BIN_DIR)


join-brother.o: join-brother.c ../include/latency.h
	$(CC) -c join-brother.c $(CFLAGS)
join-brother: join-brother.o ../transfer/send_recv.o \
		../utils/eddhr.o ../utils/calc.o     \
		../communicators/join.o              \
		../communicators/connect.o
	$(CC) -o join-brother join-brother.o \
		../transfer/send_recv.o      \
		../utils/eddhr.o ../utils/calc.o     \
		../utils/utils.o             \
		../communicators/join.o      \
		../communicators/connect.o $(LDFLAGS)

join-sister.o: join-sister.c ../include/latency.h
	$(CC) -c join-sister.c $(CFLAGS)
join-sister: join-sister.o ../transfer/send_recv.o \
		../utils/eddhr.o ../utils/calc.o    \
		../communicators/join.o              \
		../communicators/connect.o
	$(CC) -o join-sister join-sister.o \
		../transfer/send_recv.o      \
		../utils/eddhr.o ../utils/calc.o     \
		../utils/utils.o             \
		../communicators/join.o      \
		../communicators/connect.o $(LDFLAGS)

join: join-brother join-sister
	mv join-brother join-sister $(BIN_DIR)

write_test.o:	write_test.c ../include/latency.h
	$(CC) -c write_test.c $(CFLAGS)
write_test:	write_test.o ../utils/eddhr.o  \
	../utils/calc.o ../utils/utils.o       \
	../seq_io/seq_write.o ../seq_io/seq_fwrite.o \
	../seq_io/mpi_write.o
	$(CC) -o write_test write_test.o \
		../utils/eddhr.o       \
		../utils/utils.o       \
		../utils/calc.o        \
		../seq_io/seq_write.o  \
		../seq_io/mpi_write.o  \
		../seq_io/seq_fwrite.o  
	mv write_test ../bin/

clean:
	rm -f *.o *~ 
