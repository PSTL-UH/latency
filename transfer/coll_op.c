/*******************************************************************************
*        								       *
* Filename	: lateny.c  $Id: coll_op.c                                     *
*									       *
* Problem       : 						               *
*******************************************************************************/
#include "mpi.h"
#include "latency.h"
#include "latency_internal.h"
#include "eddhr.h"
#include "ADCL.h"
#include "nbc.h"

#include "lat_coll.h"

MPI_Comm lat_user_comm;
/*--------------------------------- Functions --------------------------------*/

static void LAT_COLL_MEASUREMENT ( MPI_Comm comm, MPI_Datatype dat, int maxcount, MPI_Info info);

static char *sbuf;
static char *rbuf;

struct LAT_coll_object {
    char             *sbuf; /* data to be written or read */
    char             *rbuf; /* data to be written or read */
    long long         cnt; /* number of elements to be written or read */
    long long         len; /* total number of elements */
    MPI_Request       req; 
    ADCL_Request  request;
    ADCL_Timer      timer;
    MPI_Datatype      dat; /* etype */
    MPI_Info         info;
    int		   numseg; 
    MPI_Comm	     comm;
    int	  	     size; /* num of procs */
    int		     rank;
    MPI_Datatype  newtype;
}; 

ADCL_Topology _topo;
ADCL_Vector _svec, _rvec;
ADCL_Vmap _svmap, _rvmap;
MPI_Comm _cart_comm;

int LAT_COLL_METHODOLOGY (MPI_Comm comm, MPI_Datatype dat, int maxcount, 
			  int active, char *msg, char *filename, MPI_Info info )
{

    LAT_print_init ( filename, -1, comm, active );
    LAT_print_hostname ( active );

    //sbuf  =  LAT_alloc_memory ( LAT_BUF_FACTOR * maxcount, dat );
    //rbuf  =  LAT_alloc_memory ( LAT_BUF_FACTOR * maxcount, dat );
   
    int rank = -1; 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    if (!rank)
    {
	LAT_print_status();
	LAT_print_description(msg, LAT_COLL_METHODOLOGY_STRING, info);
	LAT_print_bandinit ();
    }

    LAT_COLL_MEASUREMENT ( comm, dat, maxcount, info);
    
    LAT_free_memory ( sbuf  );
    LAT_free_memory ( rbuf  );
    LAT_print_finalize ();

    return(MPI_SUCCESS);
}

/*-------------------------------- BANDWIDTH ---------------------------------*/
static void LAT_COLL_MEASUREMENT ( MPI_Comm comm, MPI_Datatype dat, int maxcount, MPI_Info info)
{ 
    double total_stime, total_etime;
    long totallength=0;
    double starttime, endtime, min, max, s_time, E, m, s_m, sum=0., sum2=0.0;
    double t1, t2, ttime, tvtime=0.0, tempt=0.0;
    double overlap_time_s, overlap_time_e, total_time_s, total_time_e;
    double overlap_max = 0.0, time_overlap_max=0.0;
    double wait_time_s=0.0, wait_time_e=0.0, total_wait=0.0, wait_max=0.0;
    double a[BAND_TESTS];
    int cnt, i, size, flag, calclen=0;
    int band_limit, num_limit;
    int p = MPI_UNDEFINED;
    MPI_Aint extent, lb;
    EDDHR_head *eddhr_desc=NULL;
    struct LAT_coll_object c;
    // char *realname;
    int atomicity = 0;
    double btime=0, actual_time=0.0;
    double iterations=0.0;
    /* Options for the current run */
    int testresult=0;       /* default: no */
    int overlap=0;          /* default: no */
    int overlap_method=0;   /* adapt the problem size to transfer time */
    int cbs; 
    double *bp_cbs = (double *) malloc (sizeof(double));

    c.info = info;
    c.sbuf = sbuf;
    c.rbuf = rbuf;
    c.dat = dat;
    c.comm = comm;
   
    MPI_Comm_size(MPI_COMM_WORLD, &c.size); 
    MPI_Comm_rank(MPI_COMM_WORLD, &c.rank); 

    MPI_Type_size ( dat, &size );
    MPI_Type_get_extent ( dat, &lb, &extent );

    // asprintf(&realname,"%s/%s",path,testfile);
    // asprintf(&realname,"%s/%s","./",testfile);

    if ( info != MPI_INFO_NULL ) {
        CHECK_INFO_FOR_NUMSEG(info, c.numseg);
        CHECK_INFO_FOR_TESTRESULT(info, testresult);
	CHECK_INFO_FOR_OVERLAP(info, overlap);
	CHECK_INFO_FOR_BLOCKING_TIME(info, btime);
	CHECK_INFO_FOR_ATOMICITY(info, atomicity);
    }

    if (OVERLAP_NBC && overlap){
	if (btime == 0.0){
	    printf("Invalid blocking time value\n cannot overlap\n");
	    MPI_Abort(comm, 1);
	}
    }
    cbs =  4194304;

    iterations = btime;
    total_stime = MPI_Wtime ();

    // free (realname);
    c.cnt = maxcount; 
    c.len = c.cnt * size;

    LAT_COLL_MEASUREMENT_PREP_FN(c);

    cnt = maxcount;
    { 
        long x;
        double z;
        
        s_time = max = 0.0;
        sum = sum2 =0.;
        tvtime = ttime = 0.0;
        min = 99999000.0;
      
        band_limit = (cnt < MAX_SHORT_LEN ? BAND_FILE_TESTS : BAND_FILE_TESTS_LONG );
        num_limit  = (cnt < MAX_SHORT_LEN ? NUM_FILE_TESTS  : NUM_FILE_TESTS_LONG );

	totallength +=c.len/1024*(num_limit*band_limit);
        for (x=0; x<band_limit; x++) {  

	    double cycles = (cbs/1048576);
	    bp_cbs[0] = (48000/cycles);
	    
            starttime = MPI_Wtime();
	    for (i=0; i<num_limit; i++) {
			        
                if ( testresult ) {
                    EDDHR_cached_set_testdata (sbuf, cnt, dat, eddhr_desc);
                }
	
		if (overlap)
		    total_time_s = MPI_Wtime();
		
		ADCL_Timer_start( c.timer );

                LAT_COLL_MEASUREMENT_INIT_FN(c);
		if ( overlap ) {
		    if (OVERLAP_NBC){
			overlap_time_s = MPI_Wtime();
			// lat_calc_exec_nbc_modified (iterations, bp_cbs, &(c->request->r_handle));
			overlap_time_e = MPI_Wtime();
		    }
		    else
			lat_calc_exec ( 1228000 );
		}

		wait_time_s = MPI_Wtime();
                LAT_COLL_MEASUREMENT_FIN_FN(c);
		wait_time_e = MPI_Wtime();

		ADCL_Timer_stop( c.timer );
		
		if (overlap){
		    
		    total_time_e = MPI_Wtime();

		    if ( (total_time_e - total_time_s) > time_overlap_max )
			time_overlap_max = (total_time_e - total_time_s);
		    if ( (overlap_time_e - overlap_time_s) > overlap_max )
			overlap_max = (overlap_time_e - overlap_time_s) ;
		    if ( (wait_time_e - wait_time_s) > wait_max )
			wait_max = (wait_time_e - wait_time_s) ;
		}
		
/*		printf("t  %f .. O %f .. w %f .. \n",
		       time_overlap_max,
		       overlap_max,
		       wait_max);*/
                if ( testresult ) {
                    EDDHR_cached_check_testdata(rbuf,cnt,dat,&flag,eddhr_desc);
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


    total_etime = MPI_Wtime();
    totallength = totallength * c.size;

    /* Convert kb into MB */
    totallength /= 1024;

    /* Output total summary */

     double total_time = total_etime - total_stime;
     double Total_time = 0;
     double max_overlap_time=0, max_overlap = 0, max_ttime=0;     
    if (overlap){
	MPI_Reduce(&overlap_max, &max_overlap, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
	MPI_Reduce(&time_overlap_max, &max_overlap_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD); 
	MPI_Reduce(&wait_max, &total_wait, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    }
	 
	 
     MPI_Reduce(&total_time, &Total_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD); 
     if (!c.rank)
     {
	 LAT_print("\n\n");
	 LAT_print("Total amount of data %ld, total time %lf, avg. bandwidth %lf\n", totallength, Total_time, totallength/Total_time);
	 if(overlap){
	     LAT_print("max_overlap_time : %lf, max_time_with_overlap: %lf, max_wait_time: %lf, blocking_time: %lf\n", max_overlap, max_overlap_time, total_wait, btime); 
	 }
     } //if

    return;
}
