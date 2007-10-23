#ifndef __LAT_MPI_READ_AT_ALL_BEGIN__
#define __LAT_MPI_READ_AT_ALL_BEGIN__

/* Public interface */
#define LAT_FD      MPI_File

#define LAT_FILE_METHODOLOGY_STRING "MPI_File_read_at_all_begin()/MPI_File_read_at_all_end()"
#define LAT_FILE_METHODOLOGY        LAT_mpi_read_at_all_begin

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
#define LAT_FILE_MEASUREMENT       LAT_mpi_read_at_all_begin_test

/* define whether we are reading or writing */
#define LAT_WRITE 0

/* define how the file shall be opened */
#define LAT_FILE_MODE  MPI_MODE_RDONLY

/* Implementation of methodology specific initialization functions */
#define LAT_FILE_OPEN_FN(_c,_name,_mode){                    \
    int _ret;                                                \
    _ret = MPI_File_open ( c.comm, _name, _mode,      \
                           _c.info, &_c.fd);                 \
    if (_ret != MPI_SUCCESS ) MPI_Abort (MPI_COMM_WORLD, 1); \
}

#define LAT_FILE_SET_VIEW(_c){								\
    int* len = (int *) malloc((_c.numseg+1) * sizeof(int));				\
    MPI_Aint* disp = (MPI_Aint *) malloc((_c.numseg+1) * sizeof(MPI_Aint));		\
    MPI_Datatype* type = (MPI_Datatype *) malloc((_c.numseg+1) * sizeof(MPI_Datatype));	\
    int i = 0;										\
    int datsize = 0;									\
    MPI_Type_size(_c.dat, &datsize);							\
    for (i = 0; i < _c.numseg; ++i)							\
    {							    				\
	len[i] = _c.cnt/_c.numseg;							\
	disp[i] = ((i*_c.size + _c.rank)*(_c.cnt/_c.numseg))*datsize;			\
	type[i] = MPI_INT;								\
    } 											\
    len[_c.numseg] = 1;									\
    disp[_c.numseg] = ((_c.cnt*_c.size)+(_c.rank*_c.cnt/_c.numseg))*datsize;		\
    type[_c.numseg] = MPI_UB;								\
    MPI_Type_struct(_c.numseg + 1, len, disp, type, &_c.newtype);			\
    MPI_Type_commit(&_c.newtype);							\
    MPI_File_set_view (_c.fd,0,_c.dat,_c.newtype,"native",_c.info); 			\
    MPI_Type_free(&_c.newtype);								\
    free(disp);										\
    free(type);										\
    free(len);										\
    _c.offset = 0;									\
}								

#define LAT_FILE_CLOSE_FN(_c)  MPI_File_close(&_c.fd)
#define LAT_FILE_SYNC_FN(_c)  MPI_File_sync( _c.fd)

/* bandwidth measurement functions */
#define LAT_FILE_MEASUREMENT_INIT_FN(_c) MPI_File_read_at_all_begin(_c.fd,_c.offset,_c.buf,_c.cnt,_c.dat)

#define LAT_FILE_MEASUREMENT_FIN_FN(_c) {\
  MPI_Status _status;                    \
  MPI_File_read_at_all_end(_c.fd, _c.buf, &_status); \
  _c.offset += _c.cnt; \
}

#endif /* __LAT_MPI_READ_AT_ALL_BEGIN__ */
