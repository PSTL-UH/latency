#include "latency.h"
#include "latency_internal.h"

int LAT_NUM_SEG=1;

static int is_init=0;
static int file=0;

#ifdef USE_MPIIO

static MPI_Comm tcomm=MPI_COMM_NULL;
static MPI_Comm fcomm=MPI_COMM_NULL;
static MPI_File LAT_mfp=MPI_FILE_NULL;
static char tmpstring[250];
static MPI_Status status;
#endif
static FILE* LAT_fp=NULL;


void LAT_print_init ( char *filename, int partner, MPI_Comm comm, int sender )
{
  if ( filename != NULL ) {
    if ( !is_init ) {
      is_init = 1;
      file = 1;
#ifdef USE_MPIIO
      /* Create the communicator used for the IO operations first */
      MPI_Intercomm_create ( MPI_COMM_SELF, 0, comm, partner, 1100,
			     &tcomm );
      MPI_Intercomm_merge ( tcomm, sender, &fcomm );

      /* Open the file now */
      MPI_File_open ( fcomm, filename, (MPI_MODE_WRONLY|MPI_MODE_CREATE|MPI_MODE_APPEND), 
		      MPI_INFO_NULL, &LAT_mfp);
      printf("Using MPI I/O\n");
#else
      LAT_fp = fopen ( filename, "a");
#endif
      printf("Output will be written to :%s\n", filename );
    }
  }
}
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void LAT_print (char *form, ... )
{
  va_list out;

  va_start( out, form);
  if ( file ) {
#ifdef USE_MPIIO
    vsprintf (tmpstring, form, out );
    MPI_File_write_shared ( LAT_mfp, tmpstring, strlen(tmpstring), MPI_CHAR, &status);
#else
    vfprintf( LAT_fp, form, out );
    fflush(LAT_fp);
#endif
  }
  else {
    vprintf(form, out );
    fflush(stdout);
  }

  va_end ( out );
}
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void LAT_print_finalize ()
{
  if ( is_init ) {
    is_init = 0;
    if ( file ) {
#ifdef USE_MPIIO
      MPI_File_close ( &LAT_mfp);
      MPI_Comm_free ( &fcomm );
      MPI_Comm_free ( &tcomm );
#else
      fclose ( LAT_fp );
#endif
      file = 0;
    }
  }
}
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void LAT_print_status (void)
{ 
  time_t     zeit;
  struct utsname uts;

  zeit = time(NULL);

  LAT_print ( "# %s", ctime(&zeit));
  uname(&uts);
  LAT_print ("# %s %s %s %s %s\n", uts.sysname, uts.nodename, uts.release, 
             uts.version, uts.machine);
  LAT_print ( "#\n");
}
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void LAT_print_hostname (int sender)
{
  char hostname[1024];
  int len=1024;

#ifdef SR8K
  pid_t mypid;
  node_name_t nodename;
  int ret;
  
  mypid = getpid();
  hmpp_pid_to_node ( mypid, &nodename);
  
  if ( ret != -1 )
  {
    if (sender ) {
      LAT_print(  "#Sender is at %d %d %d\n", nodename.x, nodename.y, 
		 nodename.z);
    }
    else {
      LAT_print(  "#Receiver is at %d %d %d\n", nodename.x, nodename.y, nodename.z);
    }
  }
#else
  gethostname (hostname, len);
  if ( sender ) {    
     LAT_print("#Sender is %s\n", hostname );
  }
  else {
    LAT_print("#Receiver is %s\n", hostname );
  }
#endif
}
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void LAT_print_description (char * msg, char *comstr, MPI_Info info)
{ 
  int numseg=1;
  int overlap=0;

  char *text1[] = {
  "# This program tries to measure the bandwidth and latency of this",
  "# system using the MPI library.",
  "# All timings are in milli seconds, bandwidth in MBytes per second",
   NULL };
  
  int x;
  
  for (x=0; text1[x]!=NULL; x++)
     LAT_print ( "%s\n", text1[x]);

  LAT_print("#\n#\n");
  LAT_print("# Paramters for the short message tests \n");
  LAT_print("# ===================================== \n");
  LAT_print("# Number of measurements for latency  : %d\n",LATENCY_TESTS);
  LAT_print("# Number of messages per measurement  : %d\n",NUM_TESTS);
  LAT_print("# Number of measurements for bandwidth: %d\n",BAND_TESTS);
  if ( info != MPI_INFO_NULL ) {
      CHECK_INFO_FOR_NUMSEG (info, numseg);
      CHECK_LAT_NUM_SEG(numseg);
      CHECK_INFO_FOR_OVERLAP(info, overlap);
  }
  LAT_print("# Number of segments per message      : %d\n",numseg);
  LAT_print("#\n#\n");
  LAT_print("# Paramters for long message tests \n");
  LAT_print("# ================================ \n");
  LAT_print("# Number of messages per measurement  : %d\n",NUM_TESTS_LONG);
  LAT_print("# Number of measurements for bandwidth: %d\n",BAND_TESTS_LONG);
  LAT_print("# Number of segments per message      : %d\n",LAT_NUM_SEG );
  LAT_print("#\n#\n");
  LAT_print("# Communication primitive: %s\n", comstr);
  if ( overlap ) {
      LAT_print("# Overlapping communication and computation\n");
  }
  LAT_print("# %s\n", msg );


}
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
char* LAT_alloc_memory ( int count, MPI_Datatype datatype )
{
  char *tbuf;
  MPI_Aint extent;

  MPI_Type_extent ( datatype, &extent );

#ifdef USE_MPI_ALLOC_MEM
  MPI_Alloc_mem ( count*extent, MPI_INFO_NULL, (void *)&tbuf );

  LAT_print("#Memory allocated using MPI_Alloc_mem\n");
#else
  tbuf = ( char *) malloc ( count * extent );
  if ( tbuf == NULL ) {
    printf("alloc_memory: could not allocate %ld bytes of memory\n", (count * extent));
    MPI_Abort ( MPI_COMM_WORLD, 1 );
  }
#endif

  return ( tbuf );
}
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void LAT_free_memory ( char *buf )
{

#ifdef USE_MPI_ALLOC_MEM
  MPI_Free_mem ( buf );
#else
  if ( !buf ) 
    free ( buf );
#endif
}
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void LAT_print_lat (double sum, double max, double min, double E)
{      
  double av_time;
  av_time=sum/(LATENCY_TESTS*NUM_TESTS);

  LAT_print ("#\n#Latency:\n#\n#");
  LAT_print ("#  average: %9.5f     min: %9.5f     max: %9.5f   +/- %9.5f%%\n",
	   (1000*av_time),
	   (min*1000.0)/(NUM_TESTS),
	   (max*1000.0)/(NUM_TESTS),
	   100*E);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void LAT_print_bandinit ()
{

  LAT_print ( "#\n");
  LAT_print ( "#Bandwidth:\n");
  LAT_print (
	     "#\n#msg len     average        min           max       avr.-time 1 msg    min. time 1 msg  deviation\n"
	     "#   bytes      mbyte/s      mbyte/s       mbyte/s        milli sec          milli sec\n"
	     "# ----------------------------------------------------------------------------------------------------\n");
  
}  
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void LAT_print_band (long len, double sum, double max, double min, double E, int calclen)
{ 
  double n_mb;

  n_mb = ((double)len)/((double)(1024L*1024L));

  if ( calclen <= 0 ) {
      LAT_print ("  %8ld     %9.5f     %9.5f     %9.5f    %.5f      %.5f     +/-%.5f%%\n", len,
		 n_mb/sum,
		 n_mb/max,
		 n_mb /min,
		 1000*sum,
		 1000*min, 100.0*E);
  }
  else { 
      LAT_print ("  %8ld     %9.5f     %9.5f     %9.5f    %.5f      %.5f     %d    +/-%.5f%%\n", len,
		 n_mb/sum,
		 n_mb/max,
		 n_mb /min,
		 1000*sum,
		 1000*min, 
		 calclen, 
		 100.0*E);
  }
      
}
