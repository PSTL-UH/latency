
/*******************************************************************************
*        								       *
* Filename	: lateny.c  $Id: single_op.c                                   *
*									       *
* Problem       : 						               *
*******************************************************************************/

#include "latency.h"
#include "latency_internal.h"
#include "eddhr.h"

#include "lat_file.h"
MPI_Comm lat_user_comm;
/*--------------------------------- Functions --------------------------------*/

static void LAT_FILE_MEASUREMENT (MPI_Datatype dat, int maxcount, MPI_Info info,
				  char *path, char *testfile );

static char *buf;

struct LAT_file_object {
    LAT_FD             fd;
    char             *buf;
    int               cnt;
    int               len;
    off_t          offset;
    MPI_Request       req;
    MPI_Datatype      dat;
    MPI_Info         info;
}; 

int LAT_FILE_METHODOLOGY (MPI_Comm comm, MPI_Datatype dat, int maxcount, 
			  int active, char *msg, char *filename, 
			  char *path, char *testfile,  MPI_Info info )
{
    LAT_print_init ( filename, -1, comm, active );
    LAT_print_hostname ( active );

    if (active) { 
	buf  =  LAT_alloc_memory ( LAT_BUF_FACTOR * maxcount, dat );

        LAT_print_status();
        LAT_print_description(msg, LAT_FILE_METHODOLOGY_STRING, info);
        LAT_print_bandinit ();
        LAT_FILE_MEASUREMENT ( dat, maxcount, info, path, testfile );

	LAT_free_memory ( buf  );
    }
    
    LAT_print_finalize ();

    return(MPI_SUCCESS);
}

/*-------------------------------- BANDWIDTH ---------------------------------*/
static void LAT_FILE_MEASUREMENT ( MPI_Datatype dat, int maxcount, MPI_Info info, 
				   char *path, char *testfile )

{ 
    double total_stime, total_etime;
    long totallength=0;
    double starttime, endtime, min, max, s_time, E, m, s_m, sum=0., sum2=0.0;
    double t1, t2, ttime, tvtime=0.0;
    double a[BAND_TESTS];
    int cnt, i, size, flag, calclen=0;
    int band_limit, num_limit;
    int p = MPI_UNDEFINED;
    MPI_Aint extent;
    EDDHR_head *eddhr_desc=NULL;
    struct LAT_file_object c;
    char *realname;


    /* Options for the current run */
    int testresult=0;       /* default: no */
    int overlap=0;          /* default: no */
    int overlap_method=0;   /* adapt the problem size to transfer time */

    c.info = info;
    asprintf(&realname,"%s/%s",path,testfile);
    LAT_FILE_OPEN_FN(c, realname, LAT_FILE_MODE);
    free (realname);

    MPI_Type_size ( dat, &size );
    MPI_Type_extent ( dat, &extent );

    if ( info != MPI_INFO_NULL ) {
        CHECK_INFO_FOR_TESTRESULT(info, testresult);
	CHECK_INFO_FOR_OVERLAP(info, overlap)
    }


    c.buf = buf;
    c.dat = dat;

    if ( testresult ) {
        EDDHR_cached_get_description (dat, &eddhr_desc);
    }

    total_stime = MPI_Wtime ();
    for (cnt=1; cnt<=maxcount; cnt =_lat_file_next_msglen(maxcount,size,1,cnt)) { 
        long x;
        double z;
        
        s_time = max = 0.0;
        sum = sum2 =0.;
        tvtime = ttime = 0.0;
        min = 99999000.0;
        
        c.cnt      = cnt;
	c.len      = cnt * size;

        band_limit = (cnt < MAX_SHORT_LEN ? BAND_FILE_TESTS : BAND_FILE_TESTS_LONG );
        num_limit  = (cnt < MAX_SHORT_LEN ? NUM_FILE_TESTS  : NUM_FILE_TESTS_LONG );
        
        if ( testresult ) {
            EDDHR_cached_set_testdata (buf, cnt, dat, eddhr_desc );
            tvtime = 99999000.0;
	}
	
	if ( testresult || overlap ) {
	    totallength +=c.len*num_limit/1024;
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

	totallength +=c.len/1024*(num_limit*band_limit);
        for (x=0; x<band_limit; x++) { 
            
            starttime = MPI_Wtime();
	    for (i=0; i<num_limit; i++) {
                if ( LAT_WRITE && testresult ) {
                    EDDHR_cached_set_testdata (buf, cnt, dat, eddhr_desc);
                }

                LAT_FILE_MEASUREMENT_INIT_FN(c);
		if ( overlap ) {
		    lat_calc_exec ( calclen );
		}
                LAT_FILE_MEASUREMENT_FIN_FN(c);


                if ( !LAT_WRITE && testresult ) {
                    EDDHR_cached_check_testdata(buf,cnt,dat,&flag,eddhr_desc);
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
    LAT_FILE_SYNC_FN(c);
    total_etime = MPI_Wtime();

    /* Convert kb into MB */
    totallength /= 1024;

    /* Output total summary */
    LAT_print("\n\n");
    LAT_print("Total amount of data %ld, total time %lf, avg. bandwidth %lf\n", 
	      totallength, (total_etime-total_stime), totallength/(total_etime-total_stime));

    if ( testresult ) {
        EDDHR_cached_free_description(&eddhr_desc);
    }

     LAT_FILE_CLOSE_FN(c);

    return;
}

