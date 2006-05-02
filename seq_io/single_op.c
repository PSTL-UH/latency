
/*******************************************************************************
*        								       *
* Filename	: lateny.c  $Id: ping_pong.c,v 1.3 2005/01/14 12:47:20 hpceg Exp $						       *
*									       *
* Problem       : 						       *

*******************************************************************************/


#include "latency.h"
#include "latency_internal.h"
#include "eddhr.h"

#include "lat_transfer.h"

/*--------------------------------- Functions --------------------------------*/

static void LAT_FILE_MEASUREMENT (LAT_OBJTYPE obj, LAT_TMP_OBJTYPE tobj, 
				  MPI_Datatype dat, int maxcount, MPI_Info info);

static char *buf;
static char *buf2;
MPI_Comm lat_user_comm;

struct LAT_comm_object {
    LAT_OBJTYPE       obj;
    char            *sbuf;
    char            *rbuf;
    int               cnt;
    int            numseg;
    int           bufstep;
    int               len;
    MPI_Datatype      dat;
}; 

int LAT_FILE_METHODOLOGY (MPI_Comm comm, MPI_Datatype dat, int maxcount, 
			  int active, char *msg, char *filename, 
			  MPI_Info info )
{
    LAT_OBJTYPE obj;

    lat_user_comm = comm;
    LAT_print_init ( filename, -1, comm, active );
    LAT_print_hostname ( sender );
    buf  =  LAT_alloc_memory ( LAT_BUF_FACTOR * maxcount, dat );
    if ( LAT_NEED_SECOND_BUF) 
      buf2 =  LAT_alloc_memory ( maxcount, dat );

    if (active) { 
        LAT_print_status();
        LAT_print_description(msg, LAT_FILE_METHODOLOGY_STRING, info);
    }

    LAT_FILE_METHODOLOGY_INIT_FN(comm,obj,dat,maxcount,info,buf,active);

    if (sender) { 
        LAT_print_bandinit ();
        LAT_FILE_MEASUREMENT (obj, dat, maxcount, info);
    }

  
    LAT_FILE_METHODOLOGY_FIN_FN(comm, obj, tobj);
    
    LAT_free_memory ( buf  );
    if ( LAT_NEED_SECOND_BUF) 
       LAT_free_memory ( buf2 );
    LAT_print_finalize ();

    return(MPI_SUCCESS);
}

/*-------------------------------- BANDWIDTH ---------------------------------*/
static void LAT_FILE_MEASUREMENT (LAT_OBJTYPE obj, LAT_TMP_OBJTYPE tobj, 
				  MPI_Datatype dat, int maxcount, int p, 
				  MPI_Info info)
{ 
    double starttime, endtime, min, max, s_time, E, m, s_m, sum=0., sum2=0.0;
    double t1, t2, ttime, tvtime=0.0;
    double a[BAND_TESTS];
    int cnt, i, size, flag, calclen=0;
    int band_limit, num_limit;
    MPI_Aint extent;
    EDDHR_head *eddhr_desc=NULL;
    struct LAT_comm_object c;

    /* Options for the current run */
    int testresult=0;       /* default: no */
    int numseg=1;           /* default: one */
    int overlap=0;          /* default: no */
    int overlap_method=0;   /* adapt the problem size to transfer time */

    MPI_Type_size ( dat, &size );
    MPI_Type_extent ( dat, &extent );

    if ( info != MPI_INFO_NULL ) {
        CHECK_INFO_FOR_TESTRESULT(info, testresult);
        CHECK_INFO_FOR_NUMSEG(info, numseg);
	CHECK_INFO_FOR_OVERLAP(info, overlap)
    }

    c.obj     = obj;
    c.sbuf    = buf;
    c.rbuf    = buf2;
    c.defdisp = maxcount*extent;
    c.dat     = dat;

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
	c.len      = cnt * extent;

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
                LAT_FILE_MEASUREMENT_INIT_FN (c);
                LAT_FILE_MEASUREMENT_FIN_FN (c);
            }
            t2 = MPI_Wtime();
            ttime  = t2 - t1;
        }                           

	if ( overlap ) {
	    calclen = lat_calc_get_size( cnt, (ttime/(num_limit*2)), p, overlap_method,0);
	}

        for (x=0; x<band_limit; x++) { 
            LAT_FILE_MEASUREMENT_INIT_FN (c);
            LAT_FILE_MEASUREMENT_FIN_FN (c);
            
            starttime = MPI_Wtime();
	    for (i=0; i<num_limit; i++) {
                if ( testresult ) {
                    EDDHR_cached_set_testdata (buf, cnt, dat, eddhr_desc);
                }

                LAT_FILE_MEASUREMENT_INIT_FN (c);
		if ( overlap ) {
		    lat_calc_exec ( calclen );
		}
                LAT_FILE_MEASUREMENT_FIN_FN (c);

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


    return;
}

