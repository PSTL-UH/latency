# Generated automatically from Makefile.in by configure.
#Path of MPI-Library on the Machine

include Makefile.defs


all:	
	cd utils; make ; cd ..
	cd transfer ; make ; cd ..
	cd communicators; make ; cd ..
	cd datatypes ; make ; cd ..
	cd mains; make ; cd ..

nompi2:
	cd utils; make ; cd ..
	cd transfer ; make nompi2 ; cd ..
	cd datatypes ; make ; cd ..
	cd mains; make nompi2 ; cd ..


clean:
	cd utils; make clean ; cd ..
	cd transfer; make clean ; cd ..
	cd datatypes ; make clean ; cd ..
	cd communicators; make clean ; cd ..
	cd mains; make clean ; cd ..
	rm -f bin/basic bin/basic-info bin/basictoall bin/alltransfer bin/allbasictypes
	rm -f bin/alldertypes bin/onesided bin/spawn-father
	rm -f bin/spawn-son bin/connect-brother bin/connect-sister
	rm -f bin/onesided-multiseg bin/onesided-alldertypes bin/duplex
