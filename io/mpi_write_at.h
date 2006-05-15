#ifndef __LAT_MPI_WRITE_AT__
#define __LAT_MPI_WRITE_AT__

/* Public interface */
#define LAT_OBJTYPE      MPI_File

#define LAT_FILE_METHODOLOGY_STRING "MPI_File_write_at()"
#define LAT_FILE_METHODOLOGY        LAT_mpi_write_at

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
#define LAT_FILE_MEASUREMENT       LAT_mpi_write_at_test

/* define whether we are reading or writing */
#define LAT_WRITE 1

/* define how the file shall be opened */
#define LAT_FILE_MODE  MPI_MODE_WRONLY|MPI_MODE_CREATE

/* Implementation of methodology specific initialization functions */
#define LAT_FILE_METHODOLOGY_INIT_FN(_fd,_path,_filename,_mode,_c){ \
    char *_realpath;                                             \
    int _ret;                                                    \
    asprintf(&_realpath,"%s/%s",_path,_filename);                \
    _ret = MPI_File_open ( MPI_COMM_SELF, _realpath, _mode,      \
                           MPI_INFO_NULL, &_fd);                 \
    if (_ret != MPI_SUCCESS ) MPI_Abort (MPI_COMM_WORLD, 1);     \
    free(_realpath);                                             \
    _c.offset = 0;                                               \
}

#define LAT_FILE_METHODOLOGY_FIN_FN(_fd) {   \
    MPI_File_close(&_fd);                    \
}                  


/* bandwidth measurement functions */
#define LAT_FILE_MEASUREMENT_INIT_FN(_c, _obj) {    \
  MPI_Status _status;                         \
  MPI_File_write_at ( _obj, (MPI_Offset) _c.offset, _c.buf, \
                      _c.cnt, _c.dat, &_status); \
  _c.offset += _c.cnt;                           \
} 

#define LAT_FILE_MEASUREMENT_FIN_FN(_c, _obj) {}

#endif /* __LAT_SEQ_WRITE__ */
