/*
** Enhanced derived datatype handling routines.
**
** A set of MPI-like routines meant to easy the life
** of application developers when using derived datatypes.
**
** Author:
**   Edgar Gabriel <egabriel@cs.utk.edu>
*/

#ifndef _EDDR_H_
#define _EDDR_H_

#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


typedef struct _datstruct{
    int      blockid; /* block no. in this datatype */
    MPI_Aint    disp; /* displacement from the start of the buffer provided by user */
    MPI_Datatype dat; /* basic datatype */
    int     combiner; /* combiner returned by MPI_Type_get_contents */
    int        count; /* number of elements */
    struct _datstruct *next;
    struct _datstruct *prev;
} EDDHR_struct;

/* This datatype is the head of the list */
typedef struct _dathead{
    MPI_Datatype        dat; /* original datathead */
    MPI_Aint         extent; /* extent of datatype */
    int            combiner; /* how was it created */
    struct _datstruct *next; /* pointer to the description */
} EDDHR_head;


/* publicly available routines */
int EDDHR_Type_printf ( void *buf, int count, MPI_Datatype datatype );
int EDDHR_Type_description ( MPI_Datatype dat );
int EDDHR_Type_set_testdata ( void *buf, int count, MPI_Datatype datatype );

/*
** flag can be :
** @MPI_IDENT: the buffer contains correct values for the test data 
** @MPI_UNEQUAL: the buffer differs from test data 
*/
int EDDHR_Type_check_testdata ( void *buf, int count, MPI_Datatype datatype, 
                                int *flag );


/* Second set of interface routines working on a cached description */
int EDDHR_cached_get_description ( MPI_Datatype dat, EDDHR_head **eddhr);
int EDDHR_cached_print_description ( EDDHR_head *eddhr );
int EDDHR_cached_print_content ( void *buf, int count, MPI_Datatype dat, 
                               EDDHR_head *eddhr );
int EDDHR_cached_set_testdata ( void *buf, int count, MPI_Datatype dat, EDDHR_head *eddhr );
int EDDHR_cached_check_testdata ( void *buf, int count, MPI_Datatype dat, int* flag,
                                  EDDHR_head *eddhr);
int EDDHR_cached_free_description ( EDDHR_head **eddhr );


#endif
