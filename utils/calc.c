#include "latency.h"
#include "latency_internal.h"
#include "eddhr.h"

static int lat_psize=0;
static double *a=NULL;
static double *b=NULL;
static double *c=NULL;

static int lat_calc_init=0;
static EDDHR_head *eddhr_desc=NULL;
extern MPI_Comm lat_user_comm;
static double newtime=0.0, prevtime=0.0;
static int newsize=256, prevsize=0;
static int overlap_method=-1;


static double calc_func1 ( int psize );
static double calc_func2 ( int psize );

int lat_calc_get_size (int msglen, double ttime, int partner, int method, int verbose)
{
    double t1, t2;
    double m, c1;
    int rank, oldpsize=0.0;
    int r_psize;
    MPI_Status status;

    MPI_Comm_rank (MPI_COMM_WORLD, &rank);
    
    if ( lat_calc_init == 0 ) {
	EDDHR_cached_get_description(MPI_DOUBLE, &eddhr_desc);
	lat_calc_init = 1;
    }

    if ( method == LATENCY_OVERLAP_ADAPT_PSIZE ) {
	overlap_method = LATENCY_OVERLAP_ADAPT_PSIZE;

	if ( ttime > newtime ) {
	    do {
		prevtime = newtime;
		prevsize = newsize;
		
		/* New message length */
		newsize *= 2;
		if ( a != NULL ) free ( a );
		if ( b != NULL ) free ( b );
		if ( c != NULL ) free ( c );
		
		a = (double *)calloc (1, newsize * sizeof(double));
		b = (double *)calloc (1, newsize * sizeof(double));
		c = (double *)calloc (1, newsize * sizeof(double));
		if ( a == NULL || b == NULL | c == NULL ) {
		    return (MPI_ERR_INTERN);
		}
		
		t1 = MPI_Wtime();
		lat_calc_exec (newsize);
		t2 = MPI_Wtime();
		
		newtime = t2-t1;	
	    } while (newtime < ttime );
	}
	else {
	    do {
		prevtime = newtime;
		prevsize = newsize;
		
		/* New message length */
		newsize /= 2;
		if ( a != NULL ) free ( a );
		if ( b != NULL ) free ( b );
		if ( c != NULL ) free ( c );
		
		a = (double *)calloc (1, newsize * sizeof(double));
		b = (double *)calloc (1, newsize * sizeof(double));
		c = (double *)calloc (1, newsize * sizeof(double));
		if ( a == NULL || b == NULL | c == NULL ) {
		    return (MPI_ERR_INTERN);
		}
		
		t1 = MPI_Wtime();
		lat_calc_exec (newsize);
		t2 = MPI_Wtime();
		
		newtime = t2-t1;	
	    } while (newtime > ttime );
	}
	
	/* Linear extrapolation for the real vector length */
	m  = (1000*newtime - 1000*prevtime)/(newsize - prevsize);
	c1 = 1000*newtime - m*newsize;
	oldpsize = lat_psize;
	lat_psize = (int)((1000*ttime - c1)/m);
	if ( lat_psize < 0 ) {
	    lat_psize = oldpsize;
	}
	
	MPI_Sendrecv(&lat_psize, 1, MPI_INT, partner, 0, 
		     &r_psize, 1, MPI_INT, partner, 0, lat_user_comm, 
		     &status);
	
	/* Take the smaller value. The tendency seems to be, that one process
	   has a much too high value for the calculation. */
	if (r_psize < lat_psize ) {
	    lat_psize = r_psize;
	}
	
	if (verbose ) {
	    printf("%d: newsize=%d prevsize=%d newtime =%lf prevtime=%lf ttime=%lf "
		   "lat_psize=%d r_psize=%d\n", rank, newsize, prevsize, newtime, 
		   prevtime,  ttime, lat_psize, r_psize);
	}
    }
    else {
	double tt;
	/* emulate a 3D regular mesh. The mesh is of order O(3)
	   while the messages sent between the processes is of
	   order O(2).
	*/
	
	overlap_method = LATENCY_OVERLAP_3DMESH;

	tt = sqrt((double)msglen);
	if ( tt < 1.0 ) tt = 1.0;

	lat_psize = (int) (tt*tt*tt);

	if ( verbose ) {
	    printf("%d: msglen=%d tt=%lf lat_psize=%d\n", rank, msglen, tt, lat_psize);
	}
    }

    if ( a != NULL ) free ( a );
    if ( b != NULL ) free ( b );
    if ( c != NULL ) free ( c );
    
    a = (double *)calloc (1, lat_psize * sizeof(double));
    b = (double *)calloc (1, lat_psize * sizeof(double));
    c = (double *)calloc (1, lat_psize * sizeof(double));
    if ( a == NULL || b == NULL | c == NULL ) {
	return (MPI_ERR_INTERN);
    }

    EDDHR_cached_set_testdata (a, lat_psize, MPI_DOUBLE, eddhr_desc);
    EDDHR_cached_set_testdata (b, lat_psize, MPI_DOUBLE, eddhr_desc);

    return lat_psize;
}

/* The goal of this routine is to emulate some work 
   while doing communication. The routine performance
   (psize-1)*3 double precision operations and 
   psize*2 setting of testdata. The setting of the testdata
   can not precisly be measured in flops/ops. 
   But the main goal of the routine is anyway to 
   spend as much time as possible in a moderatly sized
   vector length (that's why we are trying to break any
   vectorizing effects by introducing a recursion.
*/
double lat_calc_exec (int psize ) 
{
    double res;

    if ( overlap_method == LATENCY_OVERLAP_ADAPT_PSIZE ) 
	res = calc_func1 (psize);
    else
	res = calc_func2 (psize);

    return res;
}


static double calc_func1 ( int psize )
{
    int i;
    double res=0;
    
    EDDHR_cached_set_testdata (a, psize, MPI_DOUBLE, eddhr_desc);
    EDDHR_cached_set_testdata (b, psize, MPI_DOUBLE, eddhr_desc);
    memset (c, 0, psize*sizeof(double));
    /* Using c[i-1] should destroy vectorization 
       and too heavy optimization by the compiler
    */
    c[0] = 0;
    for (i=1; i<psize; i++) {
	c[i] = c[i-1] + a[i]*b[i];
	res += c[i];
    }
    
    /* returning res should prevent, that the compiler removes
       the whole loop above */
    return res;
}

static double calc_func2 ( int psize )
{
    int i;
    double res=0;
    
    for (i=0; i<psize; i++) {
	c[i] = c[i] + a[i]*b[i];
	res += c[i];
    }
    
    /* returning res should prevent, that the compiler removes
       the whole loop above */
    return res;
}
