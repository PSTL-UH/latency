#ifndef __LAT_SEQ_FREAD__
#define __LAT_SEQ_FREAD__

/* Public interface */
#define LAT_FD      FILE*

#define LAT_FILE_METHODOLOGY_STRING "sequential fread()"
#define LAT_FILE_METHODOLOGY        LAT_seq_fread

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
#define LAT_FILE_MEASUREMENT       LAT_seq_fread_test

/* define whether we are reading or writing */
#define LAT_WRITE 0

/* define how the file shall be opened */
#define LAT_FILE_MODE "r"

/* Implementation of methodology specific initialization functions */
#define LAT_FILE_OPEN_FN(_c,_name,_mode) {             \
    _c.fd = fopen (_name,_mode);                       \
    if (_c.fd == NULL ) MPI_Abort (MPI_COMM_WORLD, 1); \
}

#define LAT_FILE_CLOSE_FN(_c) fclose(_c.fd)
#define LAT_FILE_SYNC_FN(_c) fflush(_c.fd) 


/* bandwidth measurement functions */
#define LAT_FILE_MEASUREMENT_INIT_FN(_c) {    \
  size_t _a;                                  \
  char *_c_ptr = _c.buf;                      \
  size_t _num = _c.len ;                      \
  do {                                        \
    _a = fread ( _c_ptr, 1, _num, _c.fd);     \
    _num   -= _a;                             \
    _c_ptr += _a;                             \
  }  while ( _num > 0 );                      \
}


#define LAT_FILE_MEASUREMENT_FIN_FN(_c)

#endif /* __LAT_SEQ_FREAD__ */
