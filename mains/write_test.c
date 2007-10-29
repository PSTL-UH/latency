/* Example main file executing a basic file-write 
   test using integer datatypes
*/

#include "latency.h"
#include <getopt.h>
#include <string.h>

static void print_usage();

int main ( int argc, char **argv)
{
  int mynode, numnode;
  int i, j, mode=1;
  char *path=NULL;
  char *filename=NULL;
  int numseg = 0;
  int atomicity = 0; 
  int seg_size = 0;

  static struct option long_opts[] = 
  {
      { "dir",             required_argument, NULL, 'p' },
      { "segment_size",    required_argument, NULL, 's' },
      { "mode",            required_argument, NULL, 'm' },
      { "filename",        required_argument, NULL, 'f' },
      { "atomicity",       required_argument, NULL, 'a' },
      { NULL, 0, NULL, 0 }
  };

  while (-1 != (i = getopt_long (argc, argv, "p:m:f:s:a:", long_opts, NULL))) 
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
          if ( 1 > mode || 17 < mode)
          {
              printf ("Invalid Mode\n");
              print_usage();
              exit (1);
          }
          break;
      case 'a':
          atomicity = atoi (optarg);
          if (optarg[0] != '0' && atoi (optarg) == 0) 
          {
              printf ("Invalid Atomicity value\n");
              exit (1);
          }
          break;
      case 's':
          seg_size = atoi (optarg);
          if (optarg[0] != '0' && atoi (optarg) == 0) 
          {
              printf ("Invalid Segment size value\n");
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

  MPI_Info info;
  if ( (12 <= mode) && (17 >= mode) )
  {
        char key1[] = "lat_info_numseg";
	char key2[] = "lat_info_atomicity";
        char *info_value;

        if (0 == seg_size)
        {
            printf("With MPI_File_write_all seg_size can't be 0");
            MPI_Finalize();
            return 0;
        }
        numseg = MAX_LEN/seg_size;
	if ((MAX_LEN/4)%numseg)
	{
		printf("Number of segments should be a multiple of data packet: %d\n", 
                       MAX_LEN/4);
		MPI_Finalize();
		return 0;
	}	
        
        asprintf(&info_value, "%d", numseg);
        
        MPI_Info_create(&info);
        MPI_Info_set(info, key1, info_value);
        if (NULL != info_value)
        {
            free (info_value);
        }
	
	if (atomicity)
	{
            info_value = strdup("true");
	}
	else
	{
            info_value = strdup("false");
	}
	MPI_Info_set(info, key2, info_value);
        if (NULL != info_value)
        {
            free (info_value);
        }
    }	
    if (path == NULL) {
	path = (char *) malloc ( 128 );
	getcwd ( path, 128);
    }
    if ( filename == NULL ) {
	filename = strdup ("outfile.txt");
    }


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
			    MPI_INFO_NULL); /* options/hints */
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
			     MPI_INFO_NULL); /* options/hints */
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
			     MPI_INFO_NULL); /* options/hints */
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
			     MPI_INFO_NULL); /* options/hints */
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
			    MPI_INFO_NULL); /* options/hints */
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
			    MPI_INFO_NULL); /* options/hints */
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
			       MPI_INFO_NULL); /* options/hints */
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
			       MPI_INFO_NULL); /* options/hints */
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
			     MPI_INFO_NULL); /* options/hints */
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
			     MPI_INFO_NULL); /* options/hints */
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
			     MPI_INFO_NULL); /* options/hints */
	    break;
	case 12:
	{
			    LAT_mpi_write_all ( MPI_COMM_WORLD, /* communicator */
			    MPI_INT,        /* datatype */
			    MAX_LEN/4,      /* max. count number */
			    !(mynode),      /* active process (yes/no) */
			    "sequential, datatype MPI_INT", 
			    NULL,           /* filename, NULL=stdout */
			    path,           /* path for the resulting file */
			    filename,       /* name for the resulting file */
			    info); /* options/hints */
	} //case 12
	break;

	case 13:
	{
			    LAT_mpi_write_at_all ( MPI_COMM_WORLD, /* communicator */
			    MPI_INT,        /* datatype */
			    MAX_LEN/4,      /* max. count number */
			    !(mynode),      /* active process (yes/no) */
			    "sequential, datatype MPI_INT", 
			    NULL,           /* filename, NULL=stdout */
			    path,           /* path for the resulting file */
			    filename,       /* name for the resulting file */
			    info); /* options/hints */
	} //case 13
	break;

	case 14:
	{
			    LAT_mpi_write_ordered ( MPI_COMM_WORLD, /* communicator */
			    MPI_INT,        /* datatype */
			    MAX_LEN/4,      /* max. count number */
			    !(mynode),      /* active process (yes/no) */
			    "sequential, datatype MPI_INT", 
			    NULL,           /* filename, NULL=stdout */
			    path,           /* path for the resulting file */
			    filename,       /* name for the resulting file */
			    info); /* options/hints */
	} //case 14
	break;

	case 15:
	{
			    LAT_mpi_write_all_begin( MPI_COMM_WORLD, /* communicator */
			    MPI_INT,        /* datatype */
			    MAX_LEN/4,      /* max. count number */
			    !(mynode),      /* active process (yes/no) */
			    "sequential, datatype MPI_INT", 
			    NULL,           /* filename, NULL=stdout */
			    path,           /* path for the resulting file */
			    filename,       /* name for the resulting file */
			    info); /* options/hints */
	} //case 15
	break;

	case 16:
	{
			    LAT_mpi_write_at_all_begin( MPI_COMM_WORLD, /* communicator */
			    MPI_INT,        /* datatype */
			    MAX_LEN/4,      /* max. count number */
			    !(mynode),      /* active process (yes/no) */
			    "sequential, datatype MPI_INT", 
			    NULL,           /* filename, NULL=stdout */
			    path,           /* path for the resulting file */
			    filename,       /* name for the resulting file */
			    info); /* options/hints */
	} //case 16
	break;

	case 17:
	{
			    LAT_mpi_write_ordered_begin( MPI_COMM_WORLD, /* communicator */
			    MPI_INT,        /* datatype */
			    MAX_LEN/4,      /* max. count number */
			    !(mynode),      /* active process (yes/no) */
			    "sequential, datatype MPI_INT", 
			    NULL,           /* filename, NULL=stdout */
			    path,           /* path for the resulting file */
			    filename,       /* name for the resulting file */
			    info); /* options/hints */
	} //case 17
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

static void print_usage ()
{
    printf("Usage: mpirun -np 1 ./write_test -m <mode> -f <filename> "
           "-p <path> -s <segment_size> -a <atomicity>/n");
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
    printf("              12 : MPI_File_write_all -numseg <val>\n");
    printf("              13 : MPI_File_write_at_all -numseg <val>\n");
    printf("              14 : MPI_File_write_ordered -numseg <val>\n");
    printf("              15 : MPI_File_write_all_begin -numseg <val>\n");
    printf("              16 : MPI_File_write_at_all_begin -numseg <val>\n");
    printf("              17 : MPI_File_write_ordered_begin -numseg <val>\n");
    
    printf("    For mode's from 12 to 17, -a <0/1>: for atomicity. default is 0. \n");
    printf("    -f <filename>: name of resulting file (default: "
           "outfile.txt) \n");
    printf("    -p <path>    : path where to write file <filename> "
           "(default: cwd) \n");
}
