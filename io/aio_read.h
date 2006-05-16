#ifndef __LAT_AIO_READ__
#define __LAT_AIO_READ__

/* Public interface */
#define LAT_FD      struct aiocb

#define LAT_FILE_METHODOLOGY_STRING "aio_read()"
#define LAT_FILE_METHODOLOGY        LAT_aio_read

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
#define LAT_FILE_MEASUREMENT       LAT_aio_read_test

/* define whether we are reading or writing */
#define LAT_WRITE 0

/* define how the file shall be opened */
#define LAT_FILE_MODE O_RDONLY

/* Implementation of methodology specific initialization functions */
#define LAT_FILE_OPEN_FN(_c,_name,_mode){                        \
    _c.fd.aio_fildes = open (_name,_mode,0644);                  \
    if (_c.fd.aio_fildes == -1 ) MPI_Abort (MPI_COMM_WORLD, 1);  \
    _c.fd.aio_reqprio = 0;                                       \
    _c.fd.aio_sigevent.sigev_notify = SIGEV_NONE;                \
    _c.fd.aio_offset = 0;                                        \
}

#define LAT_FILE_CLOSE_FN(_c) close( _c.fd.aio_fildes)
#define LAT_FILE_SYNC_FN(_c) fsync(_c.fd.aio_fildes)


/* bandwidth measurement functions */
#define LAT_FILE_MEASUREMENT_INIT_FN(_c ) {   \
  int _ret;                                   \
  _c.fd.aio_nbytes = _c.len;                  \
  _c.fd.aio_buf    = _c.buf;                  \
  _ret = aio_read ( &(_c.fd) );               \
  if (_ret != 0 ){                            \
    perror ( "aio_read");                     \
    MPI_Abort ( MPI_COMM_WORLD, 1 );          \
  }                                           \
}   


#define LAT_FILE_MEASUREMENT_FIN_FN(_c ) {                   \
  int _ret, _bread;                                          \
  do {                                                       \
     while ( (_ret=aio_error (&(_c.fd) ) == EINPROGRESS ));  \
     _bread = aio_return ( &(_c.fd) );                       \
     _c.fd.aio_buf = ((char *)(_c.fd.aio_buf)) + _bread;     \
     _c.fd.aio_offset += _bread;                             \
     _c.fd.aio_nbytes -= _bread;                             \
      if ( _c.fd.aio_nbytes > 0 )                            \
	    _ret = aio_read ( &(_c.fd) );                    \
  } while ( _c.fd.aio_nbytes > 0 );                          \
}

#endif /* __LAT_AIO_READ__ */
