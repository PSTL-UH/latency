#ifndef __LAT_SEQ_WRITE__
#define __LAT_SEQ_WRITE__

/* Public interface */
#define LAT_OBJTYPE      int

#define LAT_FILE_METHODOLOGY_STRING "sequential write()"
#define LAT_FILE_METHODOLOGY        LAT_seq_write

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
#define LAT_FILE_MEASUREMENT       LAT_seq_write_test

/* define whether we are reading or writing */
#define LAT_WRITE 1

/* define how the file shall be opened */
#define LAT_FILE_MODE "w"

/* Implementation of methodology specific initialization functions */
#define LAT_FILE_METHODOLOGY_INIT_FN(_fd,_path,_filename,_mode){ \
    char *_realpath;                                             \
    asprintf(_realpath,"%s/%s",_path,_filename);                 \
    _fd = open (_realpath,_mode);                                \
    free(_realpath);                                             \
}

#define LAT_FILE_METHODOLOGY_FIN_FN(_fd) {   \
    close(_fd);                             \
}                  


/* bandwidth measurement functions */
#define LAT_FILE_MEASUREMENT_INIT_FN(_c) {    \
  long _a;                                    \
  char *_c_ptr = _c.buf;                      \
  size_t _num = _c.len ;                      \
  do {                                        \
    _a = write ( _c.obj, _c_ptr, _num );      \
    if ( _a == -1 ) {                         \
        if ( errno == EINTR ) continue;       \
    }	                                      \
    _num   -= _a;                             \
    _c_ptr += _a;                             \
  }  while ( _num > 0 );                      \
}


#define LAT_FILE_MEASUREMENT_FIN_FN(_c) {}

#endif /* __LAT_SEQ_WRITE__ */
