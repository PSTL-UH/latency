/* Public interface */
#define LAT_OBJTYPE      MPI_Win
#define LAT_TMP_OBJTYPE  MPI_Group

#define LAT_COMMUNICATION_METHODOLOGY_STRING "MPI_Put/MPI_Win_start/post"
#define LAT_COMMUNICATION_METHODOLOGY        LAT_put_startpost

/* you want zero byte message ping-pong ? */
#define LAT_METHODOLOGY_DO_LATENCY_MSR       0

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
#define LAT_TIME_FACTOR     2

/* internal interfaces */
#define LAT_LATENCY_MEASUREMENT_SEND         LAT_noop
#define LAT_LATENCY_MEASUREMENT_RECV         LAT_noop2
#define LAT_BANDWIDTH_MEASUREMENT_SEND       LAT_band_sput_startpost
#define LAT_BANDWIDTH_MEASUREMENT_RECV       LAT_band_rput_startpost

/* Implementation of methodology specific initialization functions */
#define LAT_COMMUNICATION_METHODOLOGY_INIT_FN(_c,_c1,_c2,_p,_dat,_mcnt,_inf,_buf,_s) {\
  int _size;                                                                       \
  MPI_Comm _tcomm, _wcomm, _ucomm;                                                 \
  MPI_Aint _extent;                                                                \
  MPI_Comm_size ( _c, &_size );                                                    \
  if ( _size > 2 ) {                                                               \
    /* Create the communicator used for the window */                              \
    MPI_Intercomm_create ( MPI_COMM_SELF, 0, _c, _p, 1100, &_tcomm );              \
    MPI_Intercomm_merge ( _tcomm, _s, &_wcomm );                                   \
    _ucomm = _wcomm;                                                               \
    LAT_print ("# ATTENTION: since the size of the communicator is NOT 2 \n"       \
	       "#            another communicator consisting of the two \n"        \
	       "#            processes participating in the ping pong is used\n"); \
    MPI_Comm_free ( &_tcomm );                                                     \
  }                                                                                \
  else                                                                             \
    _ucomm = _c;                                                                   \
                                                                                   \
  MPI_Type_extent ( _dat, &_extent );                                              \
  MPI_Win_create ( _buf, _extent*_mcnt, 1, _inf, _ucomm, &_c1 );                   \
  MPI_Comm_group ( _ucomm, &_c2);                                                  \
  if ( _size > 2 )  MPI_Comm_free (&_wcomm);                                       \
}

#define LAT_COMMUNICATION_METHODOLOGY_FIN_FN(_c,_c1, _c2) {                    \
  MPI_Win_free (&_c1);                                                         \
  MPI_Group_free (&_c2);                                                       \
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
#define LAT_BANDWIDTH_MEASUREMENT_SEND_INIT_FN(_c) {                            \
   int _i;                                                                      \
   MPI_Win_post  (_c.tobj, 0, _c.obj);                                          \
   MPI_Win_start (_c.tobj, 0, _c.obj);                                          \
   for (_i=0; _i<_c.numseg ; _i++)                                              \
      MPI_Put (&(_c.sbuf[_i*_c.bufstep]), _c.cnt/_c.numseg, _c.dat, _c.partner, \
               _i*_c.bufstep, _c.cnt/_c.numseg, _c.dat, _c.obj);                \
   MPI_Win_complete (_c.obj);                                                   \
   MPI_Win_wait     (_c.obj);                                                   \
   MPI_Win_post  (_c.tobj, 0, _c.obj);                                          \
   MPI_Win_start (_c.tobj, 0, _c.obj);                                          \
}

#define LAT_BANDWIDTH_MEASUREMENT_SEND_FIN_FN(_c) {                             \
   MPI_Win_complete (_c.obj);                                                   \
   MPI_Win_wait     (_c.obj);                                                   \
}

#define LAT_BANDWIDTH_MEASUREMENT_RECV_INIT_FN(_c) {                             \
   MPI_Win_post  (_c.tobj, 0, _c.obj);                                           \
   MPI_Win_start (_c.tobj, 0, _c.obj);                                           \
}

#define LAT_BANDWIDTH_MEASUREMENT_RECV_FIN_FN(_c) {                              \
   int _i;                                                                       \
   MPI_Win_complete (_c.obj);                                                    \
   MPI_Win_wait     (_c.obj);                                                    \
   MPI_Win_post(_c.tobj, 0, _c.obj);                                             \
   MPI_Win_start(_c.tobj, 0, _c.obj);                                            \
   for (_i=0; _i<_c.numseg ; _i++)                                               \
      MPI_Put (&(_c.sbuf[_i*_c.bufstep]), _c.cnt/_c.numseg, _c.dat, _c.partner,  \
               _i*_c.bufstep, _c.cnt/_c.numseg, _c.dat, _c.obj);                 \
   MPI_Win_complete (_c.obj);                                                      \
   MPI_Win_wait     (_c.obj);                                                      \
}
