/* Public interface */
#define LAT_OBJTYPE      MPI_Comm
#define LAT_TMP_OBJTYPE  MPI_Comm

#define LAT_COMMUNICATION_METHODOLOGY_STRING "MPI_Isend/MPI_Recv"
#define LAT_COMMUNICATION_METHODOLOGY        LAT_isend_recv

/* you want zero byte message ping-pong ? */
#define LAT_METHODOLOGY_DO_LATENCY_MSR       1

/* set for methods doing simplex data transfer to 1, 
   set for methods doing duplex data transfer  to 2 */
#define LAT_FACTOR       2

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
#define LAT_LATENCY_MEASUREMENT_SEND         LAT_isend
#define LAT_LATENCY_MEASUREMENT_RECV         LAT_recv
#define LAT_BANDWIDTH_MEASUREMENT_SEND       LAT_band_isend
#define LAT_BANDWIDTH_MEASUREMENT_RECV       LAT_band_recv

/* Implementation of methodology specific initialization functions */
#define LAT_COMMUNICATION_METHODOLOGY_INIT_FN(_c,_c1,_c2,_p,_dat,_mcnt,_inf,_buf,_s){_c1=_c;_c2=_c;}
#define LAT_COMMUNICATION_METHODOLOGY_FIN_FN(_c,_c1, _c2) {}

/* latency measurement functions */
#define LAT_LATENCY_MEASUREMENT_SEND_FN(_c) {                             \
   MPI_Request _req;                                                      \
   MPI_Status _stat;                                                      \
   MPI_Isend (_c.sbuf, _c.cnt, _c.dat, _c.partner, 4712, _c.obj, &_req);  \
   MPI_Recv  (_c.rbuf, _c.cnt, _c.dat, _c.partner, 4712, _c.obj, &_stat); \
   MPI_Wait  (&_req, &_stat);                                             \
}

#define LAT_LATENCY_MEASUREMENT_RECV_FN(_c) {                             \
   MPI_Request _req;                                                      \
   MPI_Status _stat;                                                      \
   MPI_Isend (_c.sbuf, _c.cnt, _c.dat, _c.partner, 4712, _c.obj, &_req);  \
   MPI_Recv  (_c.rbuf, _c.cnt, _c.dat, _c.partner, 4712, _c.obj, &_stat); \
   MPI_Wait  (&_req, &_stat);                                             \
}

/* bandwidth measurement functions */
#define LAT_BANDWIDTH_MEASUREMENT_SEND_INIT_FN(_c) {                  \
   MPI_Status _stats[_c.numseg];                                      \
   int _i;                                                            \
   for (_i=0; _i<_c.numseg ; _i++)                                    \
      MPI_Isend (&(_c.sbuf[_i*_c.bufstep]), _c.cnt/_c.numseg, _c.dat, \
                 _c.partner, 4715, _c.obj, &(_c.req[_i]));            \
   for (_i=0; _i<_c.numseg ; _i++)                                    \
      MPI_Recv (&(_c.rbuf[_i*_c.bufstep]), _c.cnt/_c.numseg, _c.dat,  \
                _c.partner, 4715, _c.obj, &(_stats[_i]));             \
}

#define LAT_BANDWIDTH_MEASUREMENT_SEND_FIN_FN(_c) {\
   MPI_Status _stats[_c.numseg];                   \
   MPI_Waitall (_c.numseg, _c.req, _stats);        \
}

#define LAT_BANDWIDTH_MEASUREMENT_RECV_INIT_FN(_c) {                  \
   MPI_Status _stats[_c.numseg];                                      \
   int _i;                                                            \
   for (_i=0; _i<_c.numseg ; _i++)                                    \
      MPI_Isend (&(_c.sbuf[_i*_c.bufstep]), _c.cnt/_c.numseg, _c.dat, \
                 _c.partner, 4715, _c.obj, &(_c.req[_i]));            \
   for (_i=0; _i<_c.numseg ; _i++)                                    \
      MPI_Recv (&(_c.rbuf[_i*_c.bufstep]), _c.cnt/_c.numseg, _c.dat,  \
                _c.partner, 4715, _c.obj, &(_stats[_i]));             \
}

#define LAT_BANDWIDTH_MEASUREMENT_RECV_FIN_FN(_c) {\
   MPI_Status _stats[_c.numseg];                   \
   MPI_Waitall (_c.numseg, _c.req, _stats);        \
}
