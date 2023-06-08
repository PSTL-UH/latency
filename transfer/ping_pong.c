
/*******************************************************************************
*        								       *
* Filename	: lateny.c  $Id: ping_pong.c,v 1.3 2005/01/14 12:47:20 hpceg Exp $						       *
*									       *
* Problem       : changing message between two processors, whereat Latency,    *
*		  Bandwidth and the standard-deviation are being calculated    *
*									       *

*******************************************************************************/


#include "latency.h"
#include "latency_internal.h"
#include "eddhr.h"

#include "lat_transfer.h"

/*--------------------------------- Functions --------------------------------*/

static void LAT_LATENCY_MEASUREMENT_SEND   (LAT_OBJTYPE obj, LAT_TMP_OBJTYPE tobj, 
					    MPI_Datatype dat, int partner);
static void LAT_LATENCY_MEASUREMENT_RECV   (LAT_OBJTYPE obj, LAT_TMP_OBJTYPE tobj, 
					    MPI_Datatype dat, int partner);
static void LAT_BANDWIDTH_MEASUREMENT_SEND (LAT_OBJTYPE obj, LAT_TMP_OBJTYPE tobj, 
					    MPI_Datatype dat, int maxcount, int partner,  
					    MPI_Info info);
static void LAT_BANDWIDTH_MEASUREMENT_RECV (LAT_OBJTYPE obj, LAT_TMP_OBJTYPE tobj, 
					    MPI_Datatype dat, int maxcount, int partner,  
					    MPI_Info info);

static char *buf;
static char *buf2;
MPI_Comm lat_user_comm;

struct LAT_comm_object {
    LAT_OBJTYPE       obj;
    LAT_TMP_OBJTYPE  tobj;
    char            *sbuf;
    char            *rbuf;
    int               cnt;
    int           partner;
    int            numseg;
    int           bufstep;
    int              vlen;
    MPI_Aint      defdisp;
    MPI_Datatype      dat;
    MPI_Request      *req;
}; 

int LAT_COMMUNICATION_METHODOLOGY (MPI_Comm comm, MPI_Datatype dat, int maxcount, 
				   int partner, int sender, char *msg, char *filename, 
				   MPI_Info info )
{
    LAT_OBJTYPE obj;
    LAT_TMP_OBJTYPE tobj;

    lat_user_comm = comm;
    LAT_print_init ( filename, partner, comm, sender );
    LAT_print_hostname ( sender );
    buf  =  LAT_alloc_memory ( LAT_BUF_FACTOR * maxcount, dat );
    if ( LAT_NEED_SECOND_BUF) 
      buf2 =  LAT_alloc_memory ( maxcount, dat );

    if (sender) { 
        LAT_print_status();
        LAT_print_description(msg, LAT_COMMUNICATION_METHODOLOGY_STRING, info);
    }

    LAT_COMMUNICATION_METHODOLOGY_INIT_FN(comm,obj,tobj,partner,dat,maxcount,info,buf,sender);

    if (sender) { 
        if ( LAT_METHODOLOGY_DO_LATENCY_MSR ) {
            LAT_LATENCY_MEASUREMENT_SEND  (obj, tobj, dat, partner);
        }
        LAT_print_bandinit ();
        LAT_BANDWIDTH_MEASUREMENT_SEND (obj, tobj, dat, maxcount, partner, info);
    }
    else  { 
        if ( LAT_METHODOLOGY_DO_LATENCY_MSR ) {
            LAT_LATENCY_MEASUREMENT_RECV  (obj, tobj, dat, partner);
        }
        LAT_BANDWIDTH_MEASUREMENT_RECV (obj, tobj, dat, maxcount, partner, info);
    }

  
    LAT_COMMUNICATION_METHODOLOGY_FIN_FN(comm, obj, tobj);
    
    LAT_free_memory ( buf  );
    if ( LAT_NEED_SECOND_BUF) 
       LAT_free_memory ( buf2 );
    LAT_print_finalize ();

    return(MPI_SUCCESS);
}

/*---------------------------------- LATENCY ---------------------------------*/
static void LAT_LATENCY_MEASUREMENT_SEND ( LAT_OBJTYPE obj, LAT_TMP_OBJTYPE tobj, 
					   MPI_Datatype dat,  int partner)
{ 
    double starttime, endtime, min, max, s_time, z, m, E, a;
    long cnt, x;
    double sum2=0, sum=0;
    struct LAT_comm_object c;
    
    c.obj     = obj;
    c.tobj    = tobj;
    c.sbuf    = buf;
    c.rbuf    = buf2;
    c.cnt     = 0;
    c.partner = partner;
    c.numseg  = 1;
    c.bufstep = 0;
    c.defdisp = 0;
    c.dat     = dat;
    c.req     = NULL;

    s_time = max = 0.0;
    min = 99999000.0;
    
    for ( x=0; x<LATENCY_TESTS; x++) { 
        LAT_LATENCY_MEASUREMENT_SEND_FN (c);
        
        starttime = MPI_Wtime();
        for ( cnt=0; cnt<NUM_TESTS; cnt++)  { 
            LAT_LATENCY_MEASUREMENT_SEND_FN (c);
        }
        endtime = MPI_Wtime();
        
        z = endtime-starttime;
        a = z/2;

        s_time += a;
        sum    += a;
        sum2   += a*a;
        
        if (a>max) max = a;
        if (a<min) min = a;
    }
    
    m = sum/(LATENCY_TESTS);
    E = sqrt(sum2/(LATENCY_TESTS)-m*m);
    LAT_print_lat(s_time, max, min, E/m);
}


static void LAT_LATENCY_MEASUREMENT_RECV (LAT_OBJTYPE obj, LAT_TMP_OBJTYPE tobj, 
					  MPI_Datatype dat,   int partner)
{ 
    long cnt, x;
    struct LAT_comm_object c;
    
    c.obj     = obj;
    c.tobj    = tobj;
    c.sbuf    = buf;
    c.rbuf    = buf2;
    c.cnt     = 0;
    c.partner = partner;
    c.numseg  = 1;
    c.bufstep = 0;
    c.defdisp = 0;
    c.dat     = dat;
    c.req     = NULL;
    
    for ( x=0; x<LATENCY_TESTS; x++) { 
        LAT_LATENCY_MEASUREMENT_RECV_FN (c);
        
        for ( cnt=0; cnt<NUM_TESTS; cnt++) {
            LAT_LATENCY_MEASUREMENT_RECV_FN (c);
        }
    }
}

/*-------------------------------- BANDWIDTH ---------------------------------*/
static void LAT_BANDWIDTH_MEASUREMENT_SEND (LAT_OBJTYPE obj, LAT_TMP_OBJTYPE tobj, 
					    MPI_Datatype dat, int maxcount, int p, 
					    MPI_Info info)
{ 
    double starttime, endtime, min, max, s_time, E, m, s_m, sum=0., sum2=0.0;
    double t1, t2, ttime, tvtime=0.0;
    double a[BAND_TESTS];
    int cnt, i, size, flag, calclen=0;
    int band_limit, num_limit;
    MPI_Aint extent, lb;
    EDDHR_head *eddhr_desc=NULL;
    struct LAT_comm_object c;

    /* Options for the current run */
    int testresult=0;       /* default: no */
    int numseg=1;           /* default: one */
    int overlap=0;          /* default: no */
    int overlap_method=0;   /* adapt the problem size to transfer time */

    MPI_Type_size ( dat, &size );
    MPI_Type_get_extent ( dat, &lb, &extent );

    if ( info != MPI_INFO_NULL ) {
        CHECK_INFO_FOR_TESTRESULT(info, testresult);
        CHECK_INFO_FOR_NUMSEG(info, numseg);
	CHECK_INFO_FOR_OVERLAP(info, overlap)
    }

    c.obj     = obj;
    c.tobj    = tobj;
    c.sbuf    = buf;
    c.rbuf    = buf2;
    c.partner = p;
    c.numseg  = numseg;
    c.defdisp = maxcount*extent;
    c.dat     = dat;
    c.req = (MPI_Request *) malloc ( 2 * numseg * sizeof(MPI_Request));
    if ( c.req == NULL ) {
        MPI_Abort ( MPI_COMM_WORLD, 1 );
    }

    if ( testresult ) {
        EDDHR_cached_get_description (dat, &eddhr_desc);
    }

    for (cnt=numseg; cnt<=maxcount; cnt =_lat_next_msglen(maxcount,size,numseg,cnt)) { 
        long x;
        double z;
        
        s_time = max = 0.0;
        sum = sum2 =0.;
        tvtime = ttime = 0.0;
        min = 99999000.0;
        
        c.bufstep  = extent * cnt/numseg;
        c.cnt      = cnt;
        band_limit = (cnt < MAX_SHORT_LEN ? BAND_TESTS : BAND_TESTS_LONG );
        num_limit  = (cnt < MAX_SHORT_LEN ? NUM_TESTS  : NUM_TESTS_LONG );
        
        if ( testresult ) {
            EDDHR_cached_set_testdata (buf, cnt, dat, eddhr_desc );
            EDDHR_cached_set_testdata (buf2, cnt, dat, eddhr_desc );
            tvtime = 99999000.0;
	}
	
	if ( testresult || overlap ) {
            t1 =  MPI_Wtime();
            for ( i=0; i <num_limit; i++ ) {
                LAT_BANDWIDTH_MEASUREMENT_SEND_INIT_FN (c);
                LAT_BANDWIDTH_MEASUREMENT_SEND_FIN_FN (c);
            }
            t2 = MPI_Wtime();
            ttime  = t2 - t1;
        }                           

	if ( overlap ) {
	    calclen = lat_calc_get_size( cnt, (ttime/(num_limit*2)), p, overlap_method,0);
	}

        for (x=0; x<band_limit; x++) { 
            LAT_BANDWIDTH_MEASUREMENT_SEND_INIT_FN (c);
            LAT_BANDWIDTH_MEASUREMENT_SEND_FIN_FN (c);
            
            starttime = MPI_Wtime();
	    for (i=0; i<num_limit; i++) {
                if ( testresult ) {
                    EDDHR_cached_set_testdata (buf, cnt, dat, eddhr_desc);
                }

                LAT_BANDWIDTH_MEASUREMENT_SEND_INIT_FN (c);
		if ( overlap ) {
		    lat_calc_exec ( calclen );
		}
                LAT_BANDWIDTH_MEASUREMENT_SEND_FIN_FN (c);

                if ( testresult ) {
                    EDDHR_cached_check_testdata(buf2,cnt,dat,&flag,eddhr_desc);
                    if ( flag != MPI_IDENT ) printf("cnt=%d data wrong\n", cnt);
                }
            }
            endtime = MPI_Wtime();

	    z  = endtime-starttime;
            if ( testresult ) {
                if ((z - tvtime)<ttime) tvtime  = z - ttime;
            }

            a[x] = z/LAT_TIME_FACTOR;
        }

        for (x=0; x< band_limit; x++) {
            sum    += a[x];
            a[x]   -= tvtime/2;
            s_time += a[x];
            sum2   += a[x]*a[x];
            
            if (a[x]>max) max = a[x];
            if (a[x]<min) min = a[x];
        }
        
        m   = s_time/(band_limit);
        s_m = sum/(band_limit);
        E=sqrt(sum2/(band_limit)-m*m);
        s_time = s_time/(band_limit*num_limit);
        LAT_print_band(LAT_FACTOR*cnt*size, s_time,max/num_limit, min/num_limit, E/s_m, 
		       calclen );
    }

    if ( testresult ) {
        EDDHR_cached_free_description(&eddhr_desc);
    }

    if ( c.req != NULL ) {
        free ( c.req );
    }

    return;
}

static void LAT_BANDWIDTH_MEASUREMENT_RECV (LAT_OBJTYPE obj, LAT_TMP_OBJTYPE tobj, 
					    MPI_Datatype dat, int maxcount,  int p,  
					    MPI_Info info)
{ 
    int cnt, i, size, flag, calclen=0;
    int band_limit, num_limit;
    double t1=0.0, t2=0.0, ttime=0.0;

    MPI_Aint extent, lb;
    EDDHR_head *eddhr_desc=NULL;
    struct LAT_comm_object c;

    /* Options for the current run */
    int testresult=0;       /* default: no */
    int numseg=1;           /* default: one */
    int overlap=0;          /* default: no */
    int overlap_method=0;   /* adapt problem size to transfer time */
    
    MPI_Type_size ( dat, &size );
    MPI_Type_get_extent (dat, &lb, &extent);
    
    if ( info != MPI_INFO_NULL ) {
        CHECK_INFO_FOR_TESTRESULT(info, testresult);
        CHECK_INFO_FOR_NUMSEG(info, numseg);
	CHECK_INFO_FOR_OVERLAP(info, overlap);
    }
    
    if ( testresult ) {
        EDDHR_cached_get_description (dat, &eddhr_desc);
    }

    c.obj     = obj;
    c.tobj    = tobj;
    c.sbuf    = buf;
    c.rbuf    = buf2;
    c.partner = p;
    c.numseg  = numseg;
    c.defdisp = maxcount*extent;
    c.dat     = dat;
    c.req = (MPI_Request *) malloc ( 2 * numseg * sizeof(MPI_Request));
    if ( c.req == NULL ) {
        MPI_Abort ( MPI_COMM_WORLD, 1 );
    }

    for (cnt=numseg; cnt<=maxcount; cnt =_lat_next_msglen(maxcount,size,numseg,cnt)) { 
        long x;
        
        c.bufstep  = extent * cnt / numseg;
        c.cnt      = cnt;
        band_limit = (cnt < MAX_SHORT_LEN ? BAND_TESTS : BAND_TESTS_LONG );
        num_limit  = (cnt < MAX_SHORT_LEN ? NUM_TESTS  : NUM_TESTS_LONG );
        
        if ( testresult ) {
            EDDHR_cached_set_testdata (buf, cnt, dat, eddhr_desc );
            EDDHR_cached_set_testdata (buf2, cnt, dat, eddhr_desc );
	}

        if ( testresult || overlap ) {
            t1 = MPI_Wtime();
            for ( i=0; i <num_limit; i++ ) {
                LAT_BANDWIDTH_MEASUREMENT_RECV_INIT_FN(c);
                LAT_BANDWIDTH_MEASUREMENT_RECV_FIN_FN(c);
            }
	    t2 = MPI_Wtime();
	    ttime = t2-t1;
        }   
	
	if ( overlap ) {
	    calclen = lat_calc_get_size ( cnt, (ttime/(num_limit*2)), p, overlap_method, 0);
	}
	                        
        for (x=0; x<band_limit; x++){ 
            LAT_BANDWIDTH_MEASUREMENT_RECV_INIT_FN(c);
            LAT_BANDWIDTH_MEASUREMENT_RECV_FIN_FN(c);
            
            for (i=0; i<num_limit; i++) {
                if ( testresult ) {
                    EDDHR_cached_set_testdata (buf, cnt, dat, eddhr_desc);
                }

                LAT_BANDWIDTH_MEASUREMENT_RECV_INIT_FN(c);
		if ( overlap ) {
		    lat_calc_exec ( calclen );
		}
                LAT_BANDWIDTH_MEASUREMENT_RECV_FIN_FN(c);

                if ( testresult ) {
                    EDDHR_cached_check_testdata(buf2,cnt,dat,&flag,eddhr_desc);
                    if ( flag != MPI_IDENT ) printf("cnt=%d data wrong\n", cnt);
                }
                
            }
        }
    }

    if ( testresult ) {
        EDDHR_cached_free_description(&eddhr_desc);
    }

    if ( c.req != NULL ) {
        free ( c.req );
    }
    
    return;
}

 

    

