
include ../Makefile.defs

all: basic basic-info basictoall allbasictypes alltransfer alldertypes \
	spawn connect onesided onesided-multiseg onesided-alldertypes \
	duplex join group_excl_next_elt comm_cid_test1 comm_cid_test2

nompi2: basic basictoall allbasictypes alltransfer alldertypes 
io:	write_test write_test_overlap read_test read_test_overlap

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


group_excl_next_elt.o: group_excl_next_elt.c ../include/latency.h 
	$(CC) -c group_excl_next_elt.c $(CFLAGS)

group_excl_next_elt: group_excl_next_elt.o ../utils/utils.o ../utils/eddhr.o ../utils/calc.o \
		../transfer/send_recv.o 
	$(CC) -o group_excl_next_elt group_excl_next_elt.o $(CFLAGS)  \
		../transfer/send_recv.o \
		../utils/eddhr.o ../utils/calc.o        \
		../utils/utils.o $(LDFLAGS)
	mv group_excl_next_elt $(BIN_DIR)

comm_cid_test2.o: comm_cid_test2.c ../include/latency.h 
	$(CC) -c comm_cid_test2.c $(CFLAGS)

comm_cid_test2: comm_cid_test2.o ../utils/utils.o ../utils/eddhr.o ../utils/calc.o \
		../transfer/send_recv.o 
	$(CC) -o comm_cid_test2 comm_cid_test2.o $(CFLAGS)  \
		../transfer/send_recv.o \
		../utils/eddhr.o ../utils/calc.o        \
		../utils/utils.o $(LDFLAGS)
	mv comm_cid_test2 $(BIN_DIR)

comm_cid_test1.o: comm_cid_test1.c ../include/latency.h 
	$(CC) -c comm_cid_test1.c $(CFLAGS)

comm_cid_test1: comm_cid_test1.o ../utils/utils.o ../utils/eddhr.o ../utils/calc.o \
		../transfer/send_recv.o 
	$(CC) -o comm_cid_test1 comm_cid_test1.o $(CFLAGS)  \
		../transfer/send_recv.o \
		../utils/eddhr.o ../utils/calc.o        \
		../utils/utils.o $(LDFLAGS)
	mv comm_cid_test1 $(BIN_DIR)

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
	../io/seq_write.o ../io/seq_fwrite.o \
	../io/mpi_write.o 
	$(CC) -o write_test write_test.o \
		../utils/eddhr.o       \
		../utils/utils.o       \
		../utils/calc.o        \
		../io/seq_write.o  \
		../io/seq_writev.o  \
		../io/seq_pwrite.o  \
		../io/aio_write.o  \
		../io/mpi_write.o  \
		../io/mpi_write_at.o  \
		../io/mpi_write_shared.o  \
		../io/mpi_iwrite.o  \
		../io/mpi_iwrite_at.o  \
		../io/mpi_iwrite_shared.o  \
		../io/mpi_write_all.o \
		../io/mpi_write_at_all.o \
		../io/mpi_write_ordered.o \
		../io/mpi_write_all_begin.o \
		../io/seq_fwrite.o  $(LDFLAGS)
	mv write_test ../bin/

write_test_overlap.o:	write_test_overlap.c ../include/latency.h
	$(CC) -c write_test_overlap.c $(CFLAGS)
write_test_overlap:	write_test_overlap.o ../utils/eddhr.o  \
	../utils/calc.o ../utils/utils.o 
	$(CC) -o write_test_overlap write_test_overlap.o \
		../utils/eddhr.o       \
		../utils/utils.o       \
		../utils/calc.o        \
		../io/seq_write.o  \
		../io/seq_writev.o  \
		../io/seq_pwrite.o  \
		../io/aio_write.o  \
		../io/mpi_write.o  \
		../io/mpi_write_at.o  \
		../io/mpi_write_shared.o  \
		../io/mpi_iwrite.o  \
		../io/mpi_iwrite_at.o  \
		../io/mpi_iwrite_shared.o  \
		../io/seq_fwrite.o  $(LDFLAGS)
	mv write_test_overlap ../bin/


read_test.o:	read_test.c ../include/latency.h
	$(CC) -c read_test.c $(CFLAGS)
read_test:	read_test.o ../utils/eddhr.o  \
	../utils/calc.o ../utils/utils.o       \
	../io/seq_read.o ../io/seq_fread.o \
	../io/mpi_read.o ../io/seq_read.o
	$(CC) -o read_test read_test.o \
		../utils/eddhr.o       \
		../utils/utils.o       \
		../utils/calc.o        \
		../io/seq_write.o  \
		../io/seq_read.o  \
		../io/seq_readv.o  \
		../io/seq_pread.o  \
		../io/aio_read.o  \
		../io/mpi_read.o  \
		../io/mpi_read_at.o  \
		../io/mpi_read_shared.o  \
		../io/mpi_iread.o  \
		../io/mpi_iread_at.o  \
		../io/mpi_iread_shared.o  \
		../io/seq_fread.o  $(LDFLAGS)
	mv read_test ../bin/

read_test_overlap.o:	read_test_overlap.c ../include/latency.h
	$(CC) -c read_test_overlap.c $(CFLAGS)
read_test_overlap:	read_test_overlap.o ../utils/eddhr.o  \
	../utils/calc.o ../utils/utils.o 
	$(CC) -o read_test_overlap read_test_overlap.o \
		../utils/eddhr.o       \
		../utils/utils.o       \
		../utils/calc.o        \
		../io/seq_write.o  \
		../io/seq_read.o  \
		../io/seq_readv.o  \
		../io/seq_pread.o  \
		../io/aio_read.o  \
		../io/mpi_read.o  \
		../io/mpi_read_at.o  \
		../io/mpi_read_shared.o  \
		../io/mpi_iread.o  \
		../io/mpi_iread_at.o  \
		../io/mpi_iread_shared.o  \
		../io/seq_fread.o  $(LDFLAGS)
	mv read_test_overlap ../bin/

clean:
	rm -f *.o *~ 
