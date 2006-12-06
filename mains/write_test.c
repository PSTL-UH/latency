/* Example main file executing a basic file-write 
   test using integer datatypes
*/

#include "latency.h"
#include <string.h>

int main ( int argc, char **argv)
{
  int mynode, numnode;
  int j, mode=1;
  char *path=NULL;
  char *filename=NULL;
  int numseg = 0;

  MPI_Init ( &argc, &argv );
  MPI_Comm_size ( MPI_COMM_WORLD, &numnode );
  MPI_Comm_rank ( MPI_COMM_WORLD, &mynode );

   if ( argc == 1 ) {
        printf("Usage: mpirun -np 1 ./write_test -m <mode> -f <filename> "
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
	printf("              12 : MPI_File_write_all -numseg <val>\n");
	printf("              13 : MPI_File_write_at_all -numseg <val>\n");
	printf("              14 : MPI_File_write_at_all -numseg <val>\n");
	printf("              15 : MPI_File_write_all_begin -numseg <val>\n");

        printf("    -f <filename>: name of resulting file (default: "
	       "outfile.txt) \n");
        printf("    -p <path>    : path where to write file <filename> "
	       "(default: cwd) \n");
        exit(1);
    }


    for(j=1;j<argc;j++)  {
        if ( !strcmp ( argv[j], "-m") ) {
            mode = atoi (argv[++j]);
            continue;
        }
        if ( !strcmp ( argv[j], "-numseg") ) {
            numseg = atoi (argv[++j]);
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

    MPI_Info info;
    if ( (12 <= mode) && (15 >= mode) )
    {
	if (0 == numseg)
	{
		printf("With MPI_File_write_all numseg can't be 0.\n");
		MPI_Finalize();
		return 0;
	}
	if ((MAX_LEN/4)%numseg)
	{
		printf("Numseg should be a multiple of data packet: %d\n", MAX_LEN/4);
		MPI_Finalize();
		return 0;
	}
	char key[] = "lat_info_numseg";
	char value[6] = {0};
	sprintf(value, "%d", numseg);
	MPI_Info_create(&info);
	MPI_Info_set(info, key, value);
    }	
    if (  path == NULL ) {
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
			    break;
	} //case 12

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
			    break;
	} //case 13

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
			    break;
	} //case 14

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
			    break;
	} //case 15

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
