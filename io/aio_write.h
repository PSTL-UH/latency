#ifndef __LAT_AIO_WRITE__
#define __LAT_AIO_WRITE__

/* Public interface */
#define LAT_OBJTYPE      struct aiocb

#define LAT_FILE_METHODOLOGY_STRING "aio_write()"
#define LAT_FILE_METHODOLOGY        LAT_aio_write

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
#define LAT_FILE_MEASUREMENT       LAT_aio_write_test

/* define whether we are reading or writing */
#define LAT_WRITE 1

/* define how the file shall be opened */
#define LAT_FILE_MODE O_CREAT|O_WRONLY

/* Implementation of methodology specific initialization functions */
#define LAT_FILE_METHODOLOGY_INIT_FN(_fd,_path,_filename,_mode,_c){ \
    char *_realpath;                                             \
    int _fp;                                                     \
    asprintf(&_realpath,"%s/%s",_path,_filename);                \
    _fp = open (_realpath,_mode,0644);                           \
    if (_fp == -1 ) MPI_Abort (MPI_COMM_WORLD, 1);               \
    free(_realpath);                                             \
    _fd.aio_fildes = _fp;                                        \
    _fd.aio_reqprio = 0;                                         \
    _fd.aio_sigevent.sigev_notify = SIGEV_NONE;                  \
    _fd.aio_offset = 0;                                          \
}

#define LAT_FILE_METHODOLOGY_FIN_FN(_fd) {   \
    close( _fd.aio_fildes);                   \
}                  


/* bandwidth measurement functions */
#define LAT_FILE_MEASUREMENT_INIT_FN(_c, _obj) {    \
  int _ret;                                   \
  _obj.aio_nbytes = _c.len;                   \
  _obj.aio_buf    = _c.buf;                   \
  _ret = aio_write ( &_obj );                 \
  if (_ret != 0 ){                            \
    perror ( "aio_write");                    \
    MPI_Abort ( MPI_COMM_WORLD, 1 );          \
  }                                           \
}   


#define LAT_FILE_MEASUREMENT_FIN_FN(_c, _obj ) {             \
  int _ret, _bread;                                          \
  do {                                                       \
     while ( (_ret=aio_error (&_obj ) == EINPROGRESS ));     \
     _bread = aio_return ( &_obj );                          \
     _obj.aio_buf = ((char *)(_obj.aio_buf)) + _bread;       \
     _obj.aio_offset += _bread;                              \
     _obj.aio_nbytes -= _bread;                              \
      if ( _obj.aio_nbytes > 0 )                             \
	    _ret = aio_write ( &_obj );                      \
  } while ( _obj.aio_nbytes > 0 );                           \
}

#endif /* __LAT_AIO_WRITE__ */
