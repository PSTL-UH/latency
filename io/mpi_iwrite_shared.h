#ifndef __LAT_MPI_IWRITE_SHARED__
#define __LAT_MPI_IWRITE_SHARED__

/* Public interface */
#define LAT_FD      MPI_File

#define LAT_FILE_METHODOLOGY_STRING "MPI_File_iwrite_shared()/MPI_Wait()"
#define LAT_FILE_METHODOLOGY        LAT_mpi_iwrite_shared

/* set for methods doing simplex data transfer to 1, 
   set for methods doing duplex data transfer  to 2 */
#define LAT_FACTOR       1

/* define whether you need a second buffer:
   set to 0 if not, to 1 if you need */
#define LAT_NEED_SECOND_BUF  0

/* define LAT_BUF_FACTOR to 1, if you would like to 
   allocate maxcount * extent(datatype) buffer length.
   Some methods need this size * 2 */
#define LAT_BUF_FACTOR     1

/* define LAT_TIME_FACTOR to 2 for ping-pong tests
   (you are dividing the measured time by two)
   and to 1 for duplex tests */
#define LAT_TIME_FACTOR     1

/* internal interfaces */
#define LAT_FILE_MEASUREMENT       LAT_mpi_iwrite_shared_test

/* define whether we are reading or writing */
#define LAT_WRITE 1

/* define how the file shall be opened */
#define LAT_FILE_MODE  MPI_MODE_WRONLY|MPI_MODE_CREATE

/* Implementation of methodology specific initialization functions */
#define LAT_FILE_OPEN_FN(_c,_name,_mode){                    \
    int _ret;                                                \
    _ret = MPI_File_open ( MPI_COMM_SELF, _name, _mode,      \
                           _c.info, &_c.fd);                 \
    if (_ret != MPI_SUCCESS ) MPI_Abort (MPI_COMM_WORLD, 1); \
    MPI_File_set_view (_c.fd,0,_c.dat,_c.dat,"native",_c.info); \
}

#define LAT_FILE_CLOSE_FN(_c) MPI_File_close(&_c.fd)
#define LAT_FILE_SYNC_FN(_c) MPI_File_sync (_c.fd)

/* bandwidth measurement functions */
#define LAT_FILE_MEASUREMENT_INIT_FN(_c) MPI_File_iwrite_shared(_c.fd,_c.buf,_c.cnt,_c.dat,&_c.req)

#define LAT_FILE_MEASUREMENT_FIN_FN(_c ) {\
  MPI_Status _status;                     \
  MPI_Wait(&(_c.req),&_status);           \
}

#endif /* __LAT_MPI_IWRITE__ */
