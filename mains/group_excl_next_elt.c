/* This benchmark excludes every odd process in the current communicator
   and create a new communicator from the new group and executes
   a ping pong test between the first two processes.
   This is meant to test the effect of creating sparse process groups
   on latency between processes
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"
#include "latency.h"

int main( int argc, char *argv[] )
{
    MPI_Comm comm;
    int root;
    int rank2, size2, global_send, local_send,*old_ranks,m;
    int *ranks, n, rank, size,i,j,k,num_groups;
    MPI_Group *group;
    MPI_Comm *commList;
    char *commName, *message; 

    MPI_Init ( &argc, &argv );
    comm = MPI_COMM_WORLD;
    root = 0;
    
    MPI_Comm_rank ( comm, &rank );
    MPI_Comm_size ( comm, &size );
    
    k = size; 
    num_groups=0;
    n=0;

    /* calculate how many levels we have */
    while(k >= 2) 
    { 
	num_groups++;
	if(k%2 == 0)
	{
	    k = k/2;
	}
	else
	{ 
	    k = k/2 + 1;
	}
    }

    group = (MPI_Group *)malloc(num_groups * (sizeof(MPI_Group)) );
    if (NULL == group)
    {
	MPI_Abort (MPI_COMM_WORLD, 1);
    }
    commList = (MPI_Comm *)malloc(num_groups * (sizeof(MPI_Comm)) );
    if (NULL == commList)
    {
	MPI_Abort (MPI_COMM_WORLD, 1);
    }
    old_ranks = (int *)malloc(num_groups* (sizeof(int)));
    if (NULL == old_ranks)
    {
	MPI_Abort (MPI_COMM_WORLD, 1);
    }
    commName = (char *)malloc(8*sizeof(char));
    if (NULL == commName)
    {
	MPI_Abort (MPI_COMM_WORLD, 1);
    }
    message = (char *)malloc(32*sizeof(char));
    if (NULL == message)
    {
	MPI_Abort (MPI_COMM_WORLD, 1);
    }

    sprintf(commName,"%s%d","Comm",0);
    sprintf(message,"%s%s",commName,", datatype MPI_Byte");
    m=0;
    commList[0] = MPI_COMM_WORLD;
    MPI_Comm_group ( comm, &group[0] );

    /* ping pong on MPI_COMM_WORLD */
    if ( rank == 0 ) 
    {
	LAT_send_recv ( commList[0],    /* communicator */
			MPI_BYTE,       /* datatype */
			MAX_LEN,        /* max. count number */
			1,              /* communication partner */
			1,              /* sender flag (yes/no) */
			message, 
			commName,       /* filename, NULL=stdout */
			MPI_INFO_NULL); /* options/hints */
    }
    
    if ( rank == 1 ) 
    {
	LAT_send_recv ( commList[0],    /* communicator */
			MPI_BYTE,       /* datatype */
			MAX_LEN,        /* max. count number */
			0,              /* communication partner */
			0,              /* sender flag (yes/no) */
			message, 
			commName,       /* filename, NULL=stdout */
			MPI_INFO_NULL); /* options/hints */
    }
    if (NULL != commName)
    {
	free(commName);
    }
    if (NULL != message)
    {
	free(message);
    }
    
    MPI_Barrier ( commList[0] );
    
    /* Ping Pong on every level */
    for (j=0 ; j<num_groups-1 ; j++)
    {
	if (j != 0) 
	{
	    MPI_Comm_group (commList[j], &group[j]);
	    MPI_Comm_size (commList[j], &size);
	    MPI_Comm_rank (commList[j], &rank);
	}
	
	old_ranks[m] = rank;
	m++;
	
	k = 0;
	
	for(i=0; i<size ; i++) 
	{
	    if( i%2 != 0 ) 
	    { 
		k++;
	    }
	}
	ranks = (int *)malloc(k* (sizeof(int)) );
	k = 0;
	for(i=0; i<size ; i++) 
	{
	    if( i%2 != 0 ) 
	    { 
		ranks[k] = i;
		k++;
	    }
	}
	n = k;
	
	MPI_Group_excl ( group[j], n, ranks, &group[j+1] );
	MPI_Comm_create ( commList[j], group[j+1], &commList[j+1] );
	if (NULL != ranks)
	{
	    free(ranks);
	}

	rank2=size2=MPI_UNDEFINED;
	if ( commList[j+1] != MPI_COMM_NULL ) 
	{
	    MPI_Comm_rank ( commList[j+1], &rank2);
	    MPI_Comm_size ( commList[j+1], &size2);

	    if ( size2 < 2 ) 
	    {
		break;
	    }
	    commName = (char *)malloc( (4+ (j%10 + 1) )*sizeof(char));
	    message = (char *)malloc( (24+ (j%10 + 1) )*sizeof(char));

	    sprintf(commName,"%s%d","Comm",(j+1));
	    sprintf(message,"%s%s",commName,", datatype MPI_Byte");

	    if (rank2 == 0)
	    {
		LAT_send_recv ( commList[j+1],    /* communicator */
				MPI_BYTE,       /* datatype */
				MAX_LEN,        /* max. count number */
				1,              /* communication partner */
				1,              /* sender flag (yes/no) */
				message, 
				commName,       /* filename, NULL=stdout */
				MPI_INFO_NULL); /* options/hints */
	    }
	    if (rank2 == 1)
	    {
		LAT_send_recv ( commList[j+1],    /* communicator */
				MPI_BYTE,       /* datatype */
				MAX_LEN,        /* max. count number */
				0,              /* communication partner */
				0,              /* sender flag (yes/no) */
				message, 
				commName,       /* filename, NULL=stdout */
				MPI_INFO_NULL); /* options/hints */
	    }
	    if (NULL != commName)
	    {
		free(commName);
	    }
	    if (NULL != message)
	    {
		free(message);
	    }
	  	
	    MPI_Barrier ( commList[j+1]);
	}	  
	else 
	{
	    break;
	}
    }

    if (NULL != old_ranks)
    {
	free (old_ranks);
    }
    if (NULL != commList)
    {
	free (commList);
    }
    if (NULL != group)
    {
	free (group);
    }

    MPI_Finalize ();

    return ( 0 );
}


