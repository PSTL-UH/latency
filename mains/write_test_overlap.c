/* Example main file executing a basic file-write 
   test using integer datatypes, overlapping file write operations 
   and computation.
*/

#include "latency.h"
#include <string.h>
#include <unistd.h>
#include <getopt.h>

static void print_usage();

int main ( int argc, char **argv)
{
  int mynode, numnode;
  int j, mode=1;
  char *path=NULL;
  char *filename=NULL;
  char key[]={"lat_info_overlap"};
  char value[]={"true"};
  MPI_Info info;

  static struct option long_opts[] = 
  {
      { "dir",             required_argument, NULL, 'p' },
      { "segment_size",    required_argument, NULL, 's' },
      { "mode",            required_argument, NULL, 'm' },
      { NULL, 0, NULL, 0 }
  };

  while (-1 != (i = getopt_long (argc, argv, "p:m:f:", long_opts, NULL))) 
  {
      switch (i) 
      {
      case 'f':
          filename = strdup (optarg);
          break;
      case 'p':
          path = strdup (optarg);
          break;
      case 'm':
          mode = atoi (optarg);
          if ( 1 > mode || 11 < mode)
          {
              printf ("Invalid Mode\n");
              print_usage();
              exit (1);
          }
          break;
      default: 
          print_usage();
          exit (1);
      }
  }

  MPI_Init ( &argc, &argv );
  MPI_Comm_size ( MPI_COMM_WORLD, &numnode );
  MPI_Comm_rank ( MPI_COMM_WORLD, &mynode );

    if (  path == NULL ) {
	path = (char *) malloc ( 128 );
	getcwd ( path, 128);
    }
    if ( filename == NULL ) {
	filename = strdup ("outfile.txt");
    }


    MPI_Info_create (&info);
    MPI_Info_set(info, key, value);
    
    switch ( mode ) 
    { 
	case 1: 
	    LAT_seq_write ( MPI_COMM_WORLD, /* communicator */
			    MPI_INT,        /* datatype */
			    MAX_LEN/4,      /* max. count number */
			    !(mynode),      /* active process (yes/no) */
			    "sequential, datatype MPI_INT", 
			    NULL,           /* filename, NULL=stdout */
			    path,           /* path for the resulting file */
			    filename,       /* name for the resulting file */ 
			    info); /* options/hints */
	    break;
	case 2:
	    LAT_seq_fwrite ( MPI_COMM_WORLD, /* communicator */
			     MPI_INT,        /* datatype */
			     MAX_LEN/4,      /* max. count number */
			     !(mynode),      /* active process (yes/no) */
			     "sequential, datatype MPI_INT", 
			     NULL,           /* filename, NULL=stdout */
			     path,           /* path for the resulting file */
			     filename,       /* name for the resulting file */ 
			     info); /* options/hints */
	    break;
	case 3:
	    LAT_seq_writev ( MPI_COMM_WORLD, /* communicator */
			     MPI_INT,        /* datatype */
			     MAX_LEN/4,      /* max. count number */
			     !(mynode),      /* active process (yes/no) */
			     "sequential, datatype MPI_INT", 
			     NULL,           /* filename, NULL=stdout */
			     path,           /* path for the resulting file */
			     filename,       /* name for the resulting file */ 
			     info); /* options/hints */
	    break;
	case 4:
	    LAT_seq_pwrite ( MPI_COMM_WORLD, /* communicator */
			     MPI_INT,        /* datatype */
			     MAX_LEN/4,      /* max. count number */
			     !(mynode),      /* active process (yes/no) */
			     "sequential, datatype MPI_INT", 
			     NULL,           /* filename, NULL=stdout */
			     path,           /* path for the resulting file */
			     filename,       /* name for the resulting file */ 
			     info); /* options/hints */
	    break;
	case 5:
	    LAT_aio_write ( MPI_COMM_WORLD, /* communicator */
			    MPI_INT,        /* datatype */
			    MAX_LEN/4,      /* max. count number */
			    !(mynode),      /* active process (yes/no) */
			    "sequential, datatype MPI_INT", 
			    NULL,           /* filename, NULL=stdout */
			    path,           /* path for the resulting file */
			    filename,       /* name for the resulting file */ 
			    info); /* options/hints */
	    break;
	case 6:
	    LAT_mpi_write ( MPI_COMM_WORLD, /* communicator */
			    MPI_INT,        /* datatype */
			    MAX_LEN/4,      /* max. count number */
			    !(mynode),      /* active process (yes/no) */
			    "sequential, datatype MPI_INT", 
			    NULL,           /* filename, NULL=stdout */
			    path,           /* path for the resulting file */
			    filename,       /* name for the resulting file */ 
			    info); /* options/hints */
	    break;
	case 7:
	    LAT_mpi_write_at ( MPI_COMM_WORLD, /* communicator */
			       MPI_INT,        /* datatype */
			       MAX_LEN/4,      /* max. count number */
			       !(mynode),      /* active process (yes/no) */
			       "sequential, datatype MPI_INT", 
			       NULL,         /* filename, NULL=stdout */
			       path,         /* path for the resulting file */
			       filename,     /* name for the resulting file */ 
			       info); /* options/hints */
	    break;
	case 8:
	    LAT_mpi_write_shared ( MPI_COMM_WORLD, /* communicator */
			       MPI_INT,        /* datatype */
			       MAX_LEN/4,      /* max. count number */
			       !(mynode),      /* active process (yes/no) */
			       "sequential, datatype MPI_INT", 
			       NULL,         /* filename, NULL=stdout */
			       path,         /* path for the resulting file */
			       filename,     /* name for the resulting file */ 
			       info); /* options/hints */
	    break;
	case 9:
	    LAT_mpi_iwrite ( MPI_COMM_WORLD, /* communicator */
			     MPI_INT,        /* datatype */
			     MAX_LEN/4,      /* max. count number */
			     !(mynode),      /* active process (yes/no) */
			     "sequential, datatype MPI_INT", 
			     NULL,           /* filename, NULL=stdout */
			     path,           /* path for the resulting file */
			     filename,       /* name for the resulting file */ 
			     info); /* options/hints */
	    break;
	case 10:
	    LAT_mpi_iwrite_at ( MPI_COMM_WORLD, /* communicator */
			     MPI_INT,        /* datatype */
			     MAX_LEN/4,      /* max. count number */
			     !(mynode),      /* active process (yes/no) */
			     "sequential, datatype MPI_INT", 
			     NULL,           /* filename, NULL=stdout */
			     path,           /* path for the resulting file */
			     filename,       /* name for the resulting file */ 
			     info); /* options/hints */
	    break;
	case 11:
	    LAT_mpi_iwrite_shared ( MPI_COMM_WORLD, /* communicator */
			     MPI_INT,        /* datatype */
			     MAX_LEN/4,      /* max. count number */
			     !(mynode),      /* active process (yes/no) */
			     "sequential, datatype MPI_INT", 
			     NULL,           /* filename, NULL=stdout */
			     path,           /* path for the resulting file */
			     filename,       /* name for the resulting file */ 
			     info); /* options/hints */
	    break;

	default:
	    printf("Unknown file write mode\n");
	    MPI_Abort ( MPI_COMM_WORLD, 1 );
	    break;
    }

    if ( NULL == path ) {
	free ( path );
    }
    if ( NULL == filename ) {
	free ( filename );
    }

    MPI_Finalize ();
    return ( 0 ) ;
}

static void print_usage()
{
    printf("Usage: mpirun -np 1 ./write_test_overlap -m <mode> -f <filename> "
           "-p <path> \n");
    printf("\n");
    printf("   with: \n");
    printf("    -m <mode>    : write using a certain mode, mode being "
           "(default: 1) \n");
    printf("               1 : seq_write\n");
    printf("               2 : seq_fwrite\n");
    printf("               3 : seq_writev\n");
    printf("               4 : seq_pwrite\n");
    printf("               5 : aio_write\n");
    printf("               6 : MPI_File_write\n");
    printf("               7 : MPI_File_write_at \n");
    printf("               8 : MPI_File_write_shared \n");
    printf("               9 : MPI_File_iwrite\n");
    printf("              10 : MPI_File_iwrite_at\n");
    printf("              11 : MPI_File_iwrite_shared \n");
    printf("    -f <filename>: name of resulting file (default: outfile.txt) \n");
    printf("    -p <path>    : path where to write file <filename> (default: cwd) \n");
}
