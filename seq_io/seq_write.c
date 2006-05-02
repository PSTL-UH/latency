/* Public interface */
#define LAT_OBJTYPE      FILE*


#define LAT_FILE_METHODOLOGY_STRING "sequential write()"
#define LAT_FILE_METHODOLOGY        LAT_seq_write


/* set for methods doing simplex data transfer to 1, 
   set for methods doing duplex data transfer  to 2 */
#define LAT_FACTOR       1

/* define whether you need a second buffer:
   set to 0 if not, to 1 if you need */
#define LAT_NEED_SECOND_BUF  1

/* define LAT_BUF_FACTOR to 1, if you would like to 
   allocate maxcount * extent(datatype) buffer length.
   Some methods need this size * 2 */
#define LAT_BUF_FACTOR     1

/* define LAT_TIME_FACTOR to 2 for ping-pong tests
   (you are dividing the measured time by two)
   and to 1 for duplex tests */
#define LAT_TIME_FACTOR     1

/* internal interfaces */
#define LAT_FILE_MEASUREMENT       LAT_seq_write

/* Implementation of methodology specific initialization functions */
#define LAT_FILE_METHODOLOGY_INIT_FN(_c,_c1,_c2,_p,_dat,_mcnt,_inf,_buf,_s){
    /* open file */
      _c1=_c;_c2=_c;
}
#define LAT_FILE_METHODOLOGY_FIN_FN(_c,_c1, _c2) {}


/* bandwidth measurement functions */
#define LAT_FILE_MEASUREMENT_INIT_FN(_c) {                               \
   MPI_Status _stat;                                                               \
   int _i;                                                                         \
   for (_i=0; _i<_c.numseg ; _i++)                                                 \
     MPI_Send (&(_c.sbuf[_i*_c.bufstep]), _c.cnt/_c.numseg, _c.dat, _c.partner,    \
               4715, _c.obj);                                                      \
   for (_i=0; _i<_c.numseg ; _i++)                                                 \
      MPI_Recv (&(_c.rbuf[_i*_c.bufstep]), _c.cnt/_c.numseg, _c.dat, _c.partner,   \
                4715, _c.obj, &_stat);                                             \

  long actual_num = 0L;
  long a;
  char *c_ptr = _c.sbuf;
  size_t s_num = _c.cnt * ;

  s_num = num;

  do {
    a = write ( handle, c_ptr, s_num );
    if ( a == -1 )
      {
        if ( errno == EINTR )
          {
            /*
            ** Catch EINTR on the SP2
            */
            continue;

           continue;
          }

}   s_num      -= a;
    num        -= a;
    c_ptr      += a;
    actual_num += a;
  }  while ( num > 0 );


#define LAT_FILE_MEASUREMENT__FIN_FN(_c) {}

