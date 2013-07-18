#ifndef COLL_ADCL_IBCAST_H
#define COLL_ADCL_IBCAST_H

#define LAT_COLL_METHODOLOGY LAT_coll_adcl_ibcast
#define LAT_COLL_METHODOLOGY_STRING "ADCL_Ibcast"
#define LAT_COLL_MEASUREMENT LAT_coll_adcl_ibcast_msment
#define OVERLAP_NBC 0

/* define LAT_BUF_FACTOR to 1, if you would like to 
   allocate maxcount * extent(datatype) buffer length.
   Some methods need this size * 2 */
#define LAT_BUF_FACTOR     1

/* define LAT_TIME_FACTOR to 2 for ping-pong tests
   (you are dividing the measured time by two)
   and to 1 for duplex tests */
#define LAT_TIME_FACTOR     1

/* set for methods doing simplex data transfer to 1, 
   set for methods doing duplex data transfer  to 2 */
#define LAT_FACTOR       1

#define LAT_COLL_MEASUREMENT_INIT_FN(_c) { \
  ADCL_Request_init ( _c.request);        \
}

#define LAT_COLL_MEASUREMENT_PREP_FN(_c) {	\
						\
  int _dim_cnt, _dim, _err;			\
  int _cdims=0;					\
  int _periods=0;				\
						\
						\
  int _root = 0;				\
  _dim = _c.cnt;				\
  _dim_cnt = 1;					\
									\
/*Creating the ADCL Topology*/						\
MPI_Dims_create ( _c.size, 1, &_cdims );					\
MPI_Cart_create ( _c.comm, 1, &_cdims, &_periods, 0, &_cart_comm);	\
_err = ADCL_Topology_create ( _cart_comm, &_topo );			\
if ( ADCL_SUCCESS != _err) printf("Error n. %d\n",_err);		\
/*Creating the ADCL Vector*/						\
_err = ADCL_Vmap_all_allocate( &_svmap );				\
if ( ADCL_SUCCESS != _err) printf("Error n. %d\n",_err);		\
_err = ADCL_Vmap_all_allocate( &_rvmap );				\
if ( ADCL_SUCCESS != _err) printf("Error n. %d\n",_err);		\
									\
_err = ADCL_Vector_allocate_generic ( _dim_cnt, &_dim, 0, _svmap, _c.dat, &_c.sbuf, &_svec ); \
if ( ADCL_SUCCESS != _err) printf("Error n. %d\n",_err);		\
_err = ADCL_Vector_allocate_generic ( _dim_cnt, &_dim, 0, _rvmap, _c.dat, &_c.rbuf, &_rvec ); \
if ( ADCL_SUCCESS != _err) printf("Error n. %d\n",_err);		\
									\
/* Creating the ADCL Request*/						\
_err = ADCL_Request_create_generic_rooted ( _svec, _rvec, _topo, ADCL_FNCTSET_IBCAST, _root, &_c.request ); \
if ( ADCL_SUCCESS != _err) printf("Error n. %d\n",_err);		\
									\
_err = ADCL_Timer_create ( 1, &_c.request, &_c.timer );			\
}

#define LAT_COLL_MEASUREMENT_FIN_FN(_c) {\
  ADCL_Request_wait ( _c.request);       \
}


#endif
