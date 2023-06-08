/* Public interface */
#define LAT_OBJTYPE      MPI_Win
#define LAT_TMP_OBJTYPE  MPI_Group

#define LAT_COMMUNICATION_METHODOLOGY_STRING "MPI_Get/MPI_Win_fence using duplex mode"
#define LAT_COMMUNICATION_METHODOLOGY        LAT_get_winfence_duplex

/* you want zero byte message ping-pong ? */
#define LAT_METHODOLOGY_DO_LATENCY_MSR       0

/* set for methods doing simplex data transfer to 1, 
   set for methods doing duplex data transfer  to 2 */
#define LAT_FACTOR       2

/* define whether you need a second buffer:
   set to 0 if not, to 1 if you need */
#define LAT_NEED_SECOND_BUF  0

/* define LAT_BUF_FACTOR to 1, if you would like to 
   allocate maxcount * extent(datatype) buffer length.
   Some methods need this size * 2 */
#define LAT_BUF_FACTOR     2

/* define LAT_TIME_FACTOR to 2 for ping-pong tests
   (you are dividing the measured time by two)
   and to 1 for duplex tests */
#define LAT_TIME_FACTOR     1

/* internal interfaces */
#define LAT_LATENCY_MEASUREMENT_SEND         LAT_noop
#define LAT_LATENCY_MEASUREMENT_RECV         LAT_noop2
#define LAT_BANDWIDTH_MEASUREMENT_SEND       LAT_band_sget_winfence_duplex
#define LAT_BANDWIDTH_MEASUREMENT_RECV       LAT_band_rget_winfence_duplex

/* Implementation of methodology specific initialization functions */
#define LAT_COMMUNICATION_METHODOLOGY_INIT_FN(_c,_c1,_c2,_p,_dat,_mcnt,_inf,_buf,_s) {\
  int _size;                                                                       \
  MPI_Comm _tcomm, _wcomm, _ucomm;                                                 \
  MPI_Aint _extent, _lb;							   \
  MPI_Comm_size ( _c, &_size );                                                    \
  if ( _size > 2 ) {                                                               \
    /* Create the communicator used for the window */                              \
    MPI_Intercomm_create ( MPI_COMM_SELF, 0, _c, _p, 1100, &_tcomm );              \
    MPI_Intercomm_merge  ( _tcomm, _s, &_wcomm );                                  \
    _ucomm = _wcomm;                                                               \
    LAT_print ("# ATTENTION: since the size of the communicator is NOT 2 \n"       \
	       "#            another communicator consisting of the two \n"        \
	       "#            processes participating in the ping pong is used\n"); \
    MPI_Comm_free ( &_tcomm );                                                     \
  }                                                                                \
  else                                                                             \
    _ucomm = _c;                                                                   \
                                                                                   \
  MPI_Type_get_extent ( _dat, &_lb, &_extent );				           \
  MPI_Win_create  ( _buf, 2*_extent*_mcnt, 1, _inf, _ucomm, &_c1 );                \
  if ( _size > 2 )  MPI_Comm_free (&_wcomm);                                       \
}

#define LAT_COMMUNICATION_METHODOLOGY_FIN_FN(_c,_c1, _c2) {                    \
  MPI_Win_free (&_c1);                                                         \
}

/* latency measurement functions */
#define LAT_LATENCY_MEASUREMENT_SEND_FN(_c) {                                  \
  /* no op, should not be executed, since LAT_METHODOLOGY_DO_LATENCY_MSR is*/  \
  /* set to zero  */                                                           \
}

#define LAT_LATENCY_MEASUREMENT_RECV_FN(_c) {                                  \
  /* no op, should not be executed, since LAT_METHODOLOGY_DO_LATENCY_MSR is*/  \
  /* set to zero  */                                                           \
}

/* bandwidth measurement functions */
#define LAT_BANDWIDTH_MEASUREMENT_SEND_INIT_FN(_c) {                           \
   int _i;                                                                     \
   MPI_Win_fence (0,_c.obj);                                                   \
   for (_i=0; _i<_c.numseg ; _i++)                                             \
      MPI_Get (&(_c.sbuf[_i*_c.bufstep]), _c.cnt/_c.numseg, _c.dat, _c.partner,\
               _c.defdisp+_i*_c.bufstep, _c.cnt/_c.numseg, _c.dat, _c.obj);    \
}

#define LAT_BANDWIDTH_MEASUREMENT_SEND_FIN_FN(_c) {                            \
   MPI_Win_fence (0,_c.obj);                                                   \
}

#define LAT_BANDWIDTH_MEASUREMENT_RECV_INIT_FN(_c) {                           \
   int _i;                                                                     \
   MPI_Win_fence (0,_c.obj);                                                   \
   for (_i=0; _i<_c.numseg ; _i++)                                             \
      MPI_Get (&(_c.sbuf[_i*_c.bufstep]), _c.cnt/_c.numseg, _c.dat, _c.partner,\
               _c.defdisp+_i*_c.bufstep, _c.cnt/_c.numseg, _c.dat, _c.obj);    \
}

#define LAT_BANDWIDTH_MEASUREMENT_RECV_FIN_FN(_c) {                             \
   MPI_Win_fence (0,_c.obj);                                                    \
}
