/* Example main file executing a basic file-read
   test using integer datatypes
*/

#include "latency.h"
#include <string.h>

static void check_input_file (char *path, char *filename, MPI_Datatype dat,
			      int maxlen)


int main ( int argc, char **argv)
{
  int mynode, numnode;
  int j, mode=1;
  char *path=NULL;
  char *filename=NULL;

  MPI_Init ( &argc, &argv );
  MPI_Comm_size ( MPI_COMM_WORLD, &numnode );
  MPI_Comm_rank ( MPI_COMM_WORLD, &mynode );

   if ( argc == 1 ) {
        printf("Usage: mpirun -np 1 ./read_test -m <mode> -f <filename> "
	       "-p <path> \n");
        printf("\n");
        printf("   with: \n");
        printf("    -m <mode>    : read using a certain mode, mode being "
	       "(default: 1) \n");
	printf("               1 : seq_read\n");
	printf("               2 : seq_fread\n");
	printf("               3 : seq_readv\n");
	printf("               4 : seq_pread\n");
	printf("               5 : aio_read\n");
	printf("               6 : MPI_File_read\n");
	printf("               7 : MPI_File_read_at \n");
	printf("               8 : MPI_File_read_shared \n");
	printf("               9 : MPI_File_iread\n");
	printf("              10 : MPI_File_iread_at\n");
	printf("              11 : MPI_File_iread_shared \n");

        printf("    -f <filename>: name of resulting file (default: "
	       "outfile.txt) \n");
        printf("    -p <path>    : path where to read file <filename> "
	       "(default: cwd) \n");
        exit(1);
    }


    for(j=1;j<argc;j++)  {
        if ( !strcmp ( argv[j], "-m") ) {
            mode = atoi (argv[++j]);
            continue;
        }
        else if( !strcmp ( argv[j], "-f" ) ) {
            filename = strdup (argv[++j]);
            continue;
        }
        else if( !strcmp ( argv[j], "-p")) {
            path = strdup (argv[++j]);
            continue;
	}
	else {
            printf("Unknow flag %s\n", argv[j]);
	    MPI_Abort ( MPI_COMM_WORLD, 1 );
        }
    }
	
    if (  path == NULL ) {
	path = (char *) malloc ( 128 );
	getcwd ( path, 128);
    }
    if ( filename == NULL ) {
	filename = strdup ("outfile.txt");
    }

    check_input_file (path, filename, MPI_INT, MAX_LEN/4);

    switch ( mode ) 
    { 
	case 1: 
	    LAT_seq_read ( MPI_COMM_WORLD, /* communicator */
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
	    LAT_seq_fread ( MPI_COMM_WORLD, /* communicator */
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
	    LAT_seq_readv ( MPI_COMM_WORLD, /* communicator */
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
	    LAT_seq_pread ( MPI_COMM_WORLD, /* communicator */
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
	    LAT_aio_read ( MPI_COMM_WORLD, /* communicator */
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
	    LAT_mpi_read ( MPI_COMM_WORLD, /* communicator */
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
	    LAT_mpi_read_at ( MPI_COMM_WORLD, /* communicator */
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
	    LAT_mpi_read_shared ( MPI_COMM_WORLD, /* communicator */
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
	    LAT_mpi_iread ( MPI_COMM_WORLD, /* communicator */
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
	    LAT_mpi_iread_at ( MPI_COMM_WORLD, /* communicator */
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
	    LAT_mpi_iread_shared ( MPI_COMM_WORLD, /* communicator */
			     MPI_INT,        /* datatype */
			     MAX_LEN/4,      /* max. count number */
			     !(mynode),      /* active process (yes/no) */
			     "sequential, datatype MPI_INT", 
			     NULL,           /* filename, NULL=stdout */
			     path,           /* path for the resulting file */
			     filename,       /* name for the resulting file */ 
			     MPI_INFO_NULL); /* options/hints */
	    break;

	default:
	    printf("Unknown file read mode\n");
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




static void check_input_file (char *path, char *filename, MPI_Datatype dat,
			      int maxlen)
{
  int fd;
  int rank;
  char *realname;

  MPI_Comm_rank ( MPI_COMM_WORLD, &rank );
  asprintf(&realname, "%s/%s/", path, filename);

  fd = open ( "/data/outfile.txt", O_RDONLY );
  if ( fd == -1 ) {
      /* We need to write a file first to make sure that we have something to 
	 read! */
      LAT_seq_write ( MPI_COMM_WORLD, /* communicator */
		      dat,            /* datatype */
		      maxlen,         /* max. count number */
		      !(rank),        /* active process (yes/no) */
		      "sequential", 
		      "/tmp/result.out", /* filename, NULL=stdout */
		      path,           /* path for the resulting file */
		      filename,       /* name for the resulting file */ 
		      MPI_INFO_NULL); /* options/hints */
  }
  else {
      close (fd);
  }

  fsync(fd);
  free (realname);

  return;
}
