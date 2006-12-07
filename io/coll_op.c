
/*******************************************************************************
*        								       *
* Filename	: lateny.c  $Id: coll_op.c                                   *
*									       *
* Problem       : 						               *
*******************************************************************************/

#include "latency.h"
#include "latency_internal.h"
#include "eddhr.h"

#include "lat_file.h"
MPI_Comm lat_user_comm;
/*--------------------------------- Functions --------------------------------*/

static void LAT_FILE_MEASUREMENT ( MPI_Datatype dat, int maxcount, MPI_Info info, 
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
    int		   numseg;
    int	  	     size;
    int		     rank;
    MPI_Datatype  newtype;
}; 

int LAT_FILE_METHODOLOGY (MPI_Comm comm, MPI_Datatype dat, int maxcount, 
			  int active, char *msg, char *filename, 
			  char *path, char *testfile, MPI_Info info )
{

    LAT_print_init ( filename, -1, comm, active );
    LAT_print_hostname ( active );

    buf  =  LAT_alloc_memory ( LAT_BUF_FACTOR * maxcount, dat );
   
    int rank = -1; 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    if (!rank)
    {
	LAT_print_status();
	LAT_print_description(msg, LAT_FILE_METHODOLOGY_STRING, info);
	LAT_print_bandinit ();
    }
    LAT_FILE_MEASUREMENT ( dat, maxcount, info, path, testfile );
    
    LAT_free_memory ( buf  );
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
    c.buf = buf;
    c.dat = dat;
   
    MPI_Comm_size(MPI_COMM_WORLD, &c.size); 
    MPI_Comm_rank(MPI_COMM_WORLD, &c.rank); 

    asprintf(&realname,"%s/%s",path,testfile);
    LAT_FILE_OPEN_FN(c, realname, LAT_FILE_MODE);
    free (realname);

    MPI_Type_size ( dat, &size );
    MPI_Type_extent ( dat, &extent );

    if ( info != MPI_INFO_NULL ) {
        CHECK_INFO_FOR_NUMSEG(info, c.numseg);
        CHECK_INFO_FOR_TESTRESULT(info, testresult);
	CHECK_INFO_FOR_OVERLAP(info, overlap)
    }

    total_stime = MPI_Wtime ();
    cnt = maxcount;
    { 
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
        
	LAT_FILE_SET_VIEW(c);

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
	if (!c.rank)
	{
	    LAT_print_band(LAT_FACTOR*cnt*size, s_time,max/num_limit, min/num_limit, E/s_m, 
			   calclen );
	} //if
    }
    LAT_FILE_SYNC_FN(c);
    total_etime = MPI_Wtime();
totallength = totallength * c.size;

    /* Convert kb into MB */
    totallength /= 1024;

    /* Output total summary */

     LAT_FILE_CLOSE_FN(c);
     //MPI_Barrier(MPI_COMM_WORLD);
     double total_Stime = 0;
     MPI_Reduce(&total_stime, &total_Stime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD); 
     //MPI_Barrier(MPI_COMM_WORLD);
     double total_Etime = 0;
     MPI_Reduce(&total_etime, &total_Etime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD); 
     if (!c.rank)
     {
	 LAT_print("\n\n");
	 LAT_print("Total amount of data %ld, total time %lf, avg. bandwidth %lf\n", 
		   totallength, (total_Etime-total_Stime), totallength/(total_Etime-total_Stime));
     } //if

    return;
}

