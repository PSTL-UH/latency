#include <stdio.h>
#include <stdlib.h>
#include "eddhr.h"
#include "eddhr_internal.h"
/*
** Basic idea behind this implementation:
** 1. unroll the datatype into a linked list. This is done in a while loop
**    until all combiner-values in the linked list are set to "predefined"
** 2. Loop over the number of arguments when printing the data 
*/

#define static 

static EDDHR_struct* EDDHR_nextderdatatype ( EDDHR_struct* listhead );
static EDDHR_head *EDDHR_decode_datatype ( MPI_Datatype dat );
static void EDDHR_compactlist ( EDDHR_head *head );
static void EDDHR_freelist ( EDDHR_head *head );
static void EDDHR_dumplist ( EDDHR_head *head );
static int EDDHR_dumpcontent ( void *buf, int count, MPI_Datatype dat, 
                                EDDHR_head *head );
static int EDDHR_setdata ( void *buf, int count, MPI_Datatype dat, 
                           EDDHR_head *head );
static int EDDHR_testdata (void *buf, int count, MPI_Datatype dat,
                           EDDHR_head *head );


int EDDHR_Type_printf ( void *buf, int count, MPI_Datatype dat )
{
    EDDHR_head *head=NULL;

    /*
    ** First step: decode the datatype. The first
    ** routine generates a linked list, where each 
    ** node of the linked list contains exactly one
    ** datatype element. The second routine
    **than tries to agglomerate this description.
    */
    head = EDDHR_decode_datatype ( dat );
    EDDHR_compactlist ( head);

    EDDHR_dumplist ( head );
    /* 
    ** Second step: the linked list is now set-up completly,
    ** we can now print the real data 
    */
    EDDHR_dumpcontent ( buf, count, dat, head);
    EDDHR_freelist ( head );

    return ( MPI_SUCCESS );
}

int EDDHR_Type_description ( MPI_Datatype dat )
{
    EDDHR_head *head=NULL;

    head = EDDHR_decode_datatype ( dat );
    EDDHR_compactlist ( head );
    EDDHR_dumplist ( head );

    EDDHR_freelist ( head );

    return ( MPI_SUCCESS );
}


int EDDHR_Type_set_testdata ( void *buf, int count, MPI_Datatype dat )
{
    EDDHR_head *head=NULL;

    head = EDDHR_decode_datatype ( dat );
    EDDHR_compactlist ( head );

    EDDHR_setdata ( buf, count, dat, head );
    EDDHR_freelist ( head );

    return ( MPI_SUCCESS );
}

int EDDHR_Type_check_testdata ( void *buf, int count, MPI_Datatype dat, int* flag )
{
    EDDHR_head *head=NULL;
    int ret;

    head = EDDHR_decode_datatype ( dat );
    EDDHR_compactlist ( head );

    ret = EDDHR_testdata ( buf, count, dat, head );
    EDDHR_freelist ( head );

    *flag = ret;
    return ( MPI_SUCCESS );
}
/* 
** Routines, which use a cached description 
*/

int EDDHR_cached_get_description ( MPI_Datatype dat, EDDHR_head **eddhr)
{
    EDDHR_head *head=NULL;

    head = EDDHR_decode_datatype ( dat );
    EDDHR_compactlist ( head );

    *eddhr = head;
    return ( MPI_SUCCESS );
}

int EDDHR_cached_print_description ( EDDHR_head *eddhr )
{
    EDDHR_dumplist ( eddhr );
    return ( MPI_SUCCESS );
}

int EDDHR_cached_print_content ( void *buf, int count, MPI_Datatype dat, 
                               EDDHR_head *eddhr )
{
    EDDHR_dumpcontent (buf, count, dat, eddhr );
    return ( MPI_SUCCESS );
}

int EDDHR_cached_set_testdata ( void *buf, int count, MPI_Datatype dat, EDDHR_head *eddhr )
{
    EDDHR_setdata ( buf, count, dat, eddhr );

    return ( MPI_SUCCESS );
}

int EDDHR_cached_check_testdata ( void *buf, int count, MPI_Datatype dat, int* flag,
                                  EDDHR_head *eddhr)
{
    int ret;

    ret = EDDHR_testdata ( buf, count, dat, eddhr);

    *flag = ret;
    return ( MPI_SUCCESS );
}

int EDDHR_cached_free_description ( EDDHR_head **eddhr )
{
    EDDHR_freelist ( *eddhr );
    *eddhr = NULL;
    return ( MPI_SUCCESS );
}
/**********************************************************************/
/**********************************************************************/
/**********************************************************************/
/*                 Internal routines                                  */
/**********************************************************************/

static EDDHR_head* EDDHR_decode_datatype ( MPI_Datatype dat )
{
    EDDHR_struct *start=NULL, *current=NULL, *newelement=NULL;
    EDDHR_head *head;
    int done=0, i, j;
    int numints, numaddr, numdat, combiner;
    int *intarr;
    MPI_Aint *addrarr, prevdisp, extent;
    MPI_Datatype *datarr;

    /*
    ** First step: unroll the datatype description into a linked list
    */
    head = (EDDHR_head *)malloc ( sizeof(EDDHR_head) );
    GETFIRSTELEM ( start, dat );
    head->dat = dat;
    MPI_Type_extent ( dat, &(head->extent));
    MPI_Type_get_envelope ( dat, &numints, &numaddr, &numdat, 
                            &(head->combiner));
    head->next = start;


    while (!done ) {
        /* Get the next dadatype which has to be potentially decomposed */
        current = EDDHR_nextderdatatype ( start );
        if ( current == NULL ) {
            done = 1;
            break;
        }

        /* ok, here is a datatype to analyze */
        MPI_Type_get_envelope ( current->dat, &numints, &numaddr, &numdat, 
                                &combiner);
        
        /* Allocate temporary arrays required for MPI_Type_get_contents */
        if ( combiner != MPI_COMBINER_NAMED ) {
            if ( numints > 0 )
                intarr  = (int *) malloc ( sizeof(int ) * numints );
            else
                intarr = NULL;
            if ( numaddr > 0 )
                addrarr = (MPI_Aint*) malloc ( sizeof(MPI_Aint) * numaddr);
            else
                addrarr = NULL;
            if ( numdat > 0 )
                datarr  = (MPI_Datatype*) malloc (sizeof(MPI_Datatype)*numdat);
            else
                datarr = NULL;

            MPI_Type_get_contents ( current->dat, numints, numaddr, numdat,
                                    intarr, addrarr, datarr );
        }
        else {
            intarr  = NULL;
            addrarr = NULL;
            datarr  = NULL;
        }

        switch ( combiner ) {
        case MPI_COMBINER_NAMED:
            current->combiner = MPI_COMBINER_NAMED;            
            SETDISP(current, current->disp+0);
            break;
#ifdef HAVE_COMBINER_DUP
        case MPI_COMBINER_DUP:
            current->combiner = MPI_COMBINER_DUP;            
            SETDAT   ( current, datarr[0] );
            SETDISP  ( current, current->disp0 );
            break;
#endif
        case MPI_COMBINER_CONTIGUOUS:
            MPI_Type_extent ( datarr[0], &extent );
            for ( i = 0; i < intarr[0]; i++ ){
                current->combiner = MPI_COMBINER_CONTIGUOUS;
                SETDAT(current, datarr[0] );
                if ( i != 0 ) SETDISP(current, current->disp+extent)
                
                if ( i!= (intarr[0] - 1)) {
                    GETNEWELEM ( current, newelement);
                    current = newelement;
                }
            }
            break;
        case MPI_COMBINER_VECTOR:
            MPI_Type_extent ( datarr[0], &extent );
            prevdisp = current->disp;
            for ( i = 0; i < intarr[0]; i++ ){
                for ( j = 0; j < intarr[1]; j++ ) {
                    current->combiner = MPI_COMBINER_VECTOR;
                    SETDAT   ( current, datarr[0] );
                    if ( j == 0 ) {
                        if ( i!= 0 ) {
                            prevdisp += intarr[2]*extent;
                            SETDISP  ( current, prevdisp );
                        }
                    }
                    else {
                        SETDISP ( current, current->disp+extent );
                    }

                    if ( !((i== (intarr[0]-1)) && 
                           (j== (intarr[1]-1))) ) {
                        GETNEWELEM ( current, newelement );
                        current = newelement;
                    }
                }
            }
            break;
        case MPI_COMBINER_HVECTOR:
#ifdef HAVE_MPI_COMBINER_HVECTOR_INTEGER
        case MPI_COMBINER_HVECTOR_INTEGER:
#endif
            MPI_Type_extent ( datarr[0], &extent );
            prevdisp = current->disp;
            for ( i = 0; i < intarr[0]; i++ ){
                for ( j = 0; j < intarr[1]; j++ ) {
                    current->combiner = MPI_COMBINER_HVECTOR;
                    SETDAT   ( current, datarr[0] );
                    if ( j == 0 ) {
                        if ( i!= 0 ) {
                            prevdisp += addrarr[0];
                            SETDISP(current, prevdisp);
                        }
                    }
                    else {
                        SETDISP(current, current->disp+extent);
                    }
                   
                    if ( !(( i == (intarr[0]-1)) && 
                           ( j == (intarr[1]-1)) ) ) {
                        GETNEWELEM ( current, newelement );
                        current = newelement;
                    }
                }
            }
            break;
        case MPI_COMBINER_INDEXED:
            MPI_Type_extent ( datarr[0] , &extent );
            for ( i = 0; i < intarr[0]; i++ ) {
                for ( j = 0; j < intarr[i+1]; j++ ){
                    current->combiner = MPI_COMBINER_INDEXED;
                    SETDAT   ( current, datarr[0] );
                    if ( j == 0 ) {
                        if ( i != 0 ) {
                            GETPREVDISP ( current, prevdisp );                   
                            SETDISP  ( current, (intarr[(intarr[0]+i+1)]*extent
                                                 - prevdisp+current->disp) );
                        }
                    }
                    else {
                        SETDISP ( current, current->disp+extent );
                    }

                    if ( !((j == (intarr[i+1]-1)) && 
                           (i == (intarr[0] - 1))) ){
                        GETNEWELEM ( current, newelement );
                        current = newelement;
                    }
                }
            }
            break;
        case MPI_COMBINER_HINDEXED:
#ifdef HAVE_MPI_COMBINER_HINDEXED_INTEGER
        case MPI_COMBINER_HINDEXED_INTEGER:
#endif
            MPI_Type_extent ( datarr[0] , &extent );
            for ( i = 0; i < intarr[0]; i++ ) {
                for ( j = 0; j < intarr[i+1]; j++ ){                    
                    current->combiner = MPI_COMBINER_HINDEXED;
                    SETDAT   ( current, datarr[0] );
                    if ( j == 0 ) {
                        if ( i!= 0 ) {
                            GETPREVDISP ( current, prevdisp);
                            SETDISP(current, current->disp + addrarr[i] - prevdisp);
                        }
                    }
                    else {
                        SETDISP(current, current->disp+extent );
                    }
                    
                    if ( !((j == (intarr[i+1]-1)) && 
                           (i == (intarr[0] - 1))) ){
                        GETNEWELEM ( current, newelement );
                        current = newelement;
                    }
                }
            }
            break;
#ifdef HAVE_MPI_COMBINER_INDEXED_BLOCK
        case MPI_COMBINER_INDEXED_BLOCK:
            /* to be done */
            break;
#endif

        case MPI_COMBINER_STRUCT:
#ifdef HAVE_MPI_COMBINER_STRUCT_INTEGER
        case MPI_COMBINER_STRUCT_INTEGER:
#endif
            for ( i = 0; i < intarr[0] ; i++ ) {
                MPI_Type_extent ( datarr[i], &extent );
                for ( j = 0; j < intarr[i+1]; j++ ) {
                    current->combiner = MPI_COMBINER_STRUCT;
                    SETDAT   ( current, datarr[i] );
                    if ( j == 0 ) {
                        if ( i!= 0 ) {
                            GETPREVDISP(current, prevdisp);
                            SETDISP  ( current, current->disp+addrarr[i]-prevdisp );
                        }
                    }
                    else {
                        SETDISP ( current, current->disp+extent );
                    }

                    if ( !((j == (intarr[i+1]-1)) &&
                           (i == (intarr[0] - 1))) ) {
                        GETNEWELEM ( current, newelement ) ;
                        current = newelement;
                    }
                }
            }
            break;
#ifdef HAVE_MPI_COMBINER_SUBARRAY
        case MPI_COMBINER_SUBARRAY:
            /* to be done */
            break;
#endif
#ifdef HAVE_MPI_COMBINER_DARRAY
        case MPI_COMBINER_DARRAY:
            /* to be done */
            break;
#endif
#ifdef HAVE_MPI_COMBINER_RESIZED
        case MPI_COMBINER_RESIZED:
            /* to be done */
            break;
#endif
        default:
            printf("EDDHR_decode_datatype: unknown/unimplemented datatype "
                   "constructor. Combiner = %d\n", combiner);
            EDDHR_freelist ( head );
            start = NULL;
            done = 1;
            break;
        }

        /* Free the temporary arrays used for Type_get_contents */
        if ( intarr != NULL ) 
            free ( intarr );
        if ( addrarr != NULL )
            free ( addrarr );
        if ( datarr != NULL )
            free ( datarr );
    }

    return ( head );
}
/**********************************************************************/
/**********************************************************************/
/**********************************************************************/
static int EDDHR_dumpcontent ( void *buf, int count, MPI_Datatype dat, 
                               EDDHR_head *head )
{
    char *bufp, *cbufp;
    short *sbufp;
    int *ibufp;
    long *lbufp;
    unsigned short *usbufp;
    unsigned int *uibufp;
    unsigned long *ulbufp;
    float *fbufp;
    double *dbufp;
#ifdef HAVE_LONG_DOUBLE
    long double *ldbufp;
#endif

    int i, j;
    EDDHR_struct *current;
    
    for ( i = 0; i < count; i++ ) {
        bufp = (char *)buf + i * head->extent;
        current = head->next;

        while ( current != NULL ) {
            if ( (current->dat==MPI_CHAR) || (current->dat==MPI_BYTE)) {
                cbufp = (char *) ((char*)bufp+current->disp);
                printf("%s", &cbufp[0]);
                printf(" ");
            }
            else if ( current->dat ==  MPI_SHORT ){
                sbufp = (short *) ((char *)bufp+current->disp);
                for ( j = 0; j < current->count; j++)
                    printf("%d ", (short)sbufp[j]);
            }
            else if ( current->dat ==  MPI_UNSIGNED_SHORT) {
                usbufp = (unsigned short *) ((char *)bufp+current->disp);
                for ( j = 0; j < current->count; j++)
                    printf("%u ", (unsigned short)usbufp[j]);
            }
            else if ( current->dat ==  MPI_INT) {
                ibufp = (int *) ((char*)bufp+current->disp);
                for ( j = 0; j < current->count; j++)
                    printf("%d ", (int)ibufp[j]);
            }
            else if ( current->dat ==  MPI_UNSIGNED) {
                uibufp = (unsigned *) ((char*)bufp+current->disp);
                for ( j = 0; j < current->count; j++)
                    printf("%u ", (unsigned)uibufp[j]);
            }
            else if ( current->dat ==  MPI_LONG) {
                lbufp = (long *) ((char*)bufp+current->disp);
                for ( j = 0; j < current->count; j++)
                    printf("%ld ", (long)lbufp[j]);
            }
            else if ( current->dat ==  MPI_UNSIGNED_LONG) {
                ulbufp = (unsigned long *) ((char*)bufp+current->disp);
                for ( j = 0; j < current->count; j++)
                    printf("%lu ", (unsigned long)ulbufp[j]);
            }
            else if ( current->dat ==  MPI_FLOAT) {
                fbufp = (float *) ((char*)bufp+current->disp);
                for ( j = 0; j < current->count; j++)
                    printf("%f ", (float)fbufp[j]);
            }
            else if ( current->dat ==  MPI_DOUBLE) {
                dbufp = (double *) ((char*)bufp+current->disp);
                for ( j = 0; j < current->count; j++)
                    printf("%lf ", (double)dbufp[j]);
            }
#ifdef HAVE_LONG_DOUBLE
            else if ( current->dat ==  MPI_LONG_DOUBLE ) {
                ldbufp = (long double *) ((char*)bufp+current->disp);
                for ( j = 0; j < current->count; j++)
                    printf("%lf ", (long double)ldbufp[j]);
            }
#endif
            else {
                printf("EDDHR_dumpcontent: unknown/unimplemented basic "
                       "datatype\n");
                return ( MPI_ERR_TYPE );
            }
            current = current->next;
        }
        printf("\n");
    }
    return ( MPI_SUCCESS );
}
/**********************************************************************/
/**********************************************************************/
/**********************************************************************/
static int EDDHR_testdata ( void *buf, int count, MPI_Datatype dat, 
                           EDDHR_head *head )
{
    char *bufp, *cbufp, *fromp;
    short *sbufp, stmp;
    int *ibufp, itmp;
    long *lbufp, ltmp;
    unsigned short *usbufp, ustmp;
    unsigned int *uibufp, uitmp;
    unsigned long *ulbufp, ultmp;
    float *fbufp, ftmp;
    double *dbufp, dtmp;
#ifdef HAVE_LONG_DOUBLE
    long double *ldbufp, ldtmp;
#endif
    int i, j, size, rsize, ret;
    int result=MPI_IDENT, rank;
    EDDHR_struct *current;
    

    RESETCOUNTERS(0);
    
    MPI_Comm_rank ( MPI_COMM_WORLD, &rank );
    for ( i = 0; i < count; i++ ) {
        bufp = (char *)buf + i * head->extent;
        current = head->next;

        while ( current != NULL ) {
            if ( (current->dat==MPI_CHAR) || (current->dat==MPI_BYTE)){
                cbufp = (char *) ((char*)bufp+current->disp);
                size = current->count;
                while (size>0){
                    NEXTCHAR(fromp, size, rsize);
                    ret = strncmp(cbufp, fromp, rsize);
                    if ( size == rsize ) {
                        ret = strncmp(cbufp, fromp, rsize-1);
                        size -= rsize;                        
                    }
                    else {
                        ret = strncmp(cbufp, fromp, rsize-1);
                        size-=rsize-1;
                    }
                    if ( ret != 0 ) {
                        result = MPI_UNEQUAL;
                        printf("[%d]: count=%d, ret= %d datatype: MPI_CHAR differ "
                        "Found: %s\n", rank, i, ret, cbufp); 
                    }
                    cbufp+=rsize-1;
                }
            }
            else if ( current->dat ==  MPI_SHORT ){
                sbufp = (short *) ((char *)bufp+current->disp);
                for ( j = 0; j < current->count; j++) {
                    NEXTSHORT(stmp);
                    if ( sbufp[j] !=  stmp ) {
                        result = MPI_UNEQUAL;
                        printf("[%d]: count=%d, datatype: MPI_SHORT block: %d "
                               " element:%d differ. Found:%d Expected: %d\n",
                               rank, i, current->blockid, j, sbufp[j], stmp );
                    }
                }
            }
            else if ( current->dat ==  MPI_UNSIGNED_SHORT) {
                usbufp = (unsigned short *) ((char *)bufp+current->disp);
                for ( j = 0; j < current->count; j++) {
                    NEXTUSHORT(ustmp);
                    if ( usbufp[j] != ustmp ) {
                        result = MPI_UNEQUAL;
                        printf("[%d]: count=%d, datatype: MPI_UNISGNED_SHORT "
                               "block: %d element:%d differ. Found:%u Expected: %u\n",
                               rank, i, current->blockid, j, usbufp[j], ustmp );
                    }
                }
            }
            else if ( current->dat ==  MPI_INT) {
                ibufp = (int *) ((char*)bufp+current->disp);
                for ( j = 0; j < current->count; j++) {
                    NEXTINT(itmp);
                    if ( ibufp[j] != itmp ) {
                        result = MPI_UNEQUAL;
                        printf("[%d]: count=%d, datatype: MPI_INT block: %d "
                               " element:%d differ. Found:%d Expected: %d\n",
                               rank, i, current->blockid, j, ibufp[j], itmp );
                    }
                }
            }
            else if ( current->dat ==  MPI_UNSIGNED) {
                uibufp = (unsigned *) ((char*)bufp+current->disp);
                for ( j = 0; j < current->count; j++){
                    NEXTUINT(uitmp);
                    if ( uibufp[j] !=  uitmp) {
                        result = MPI_UNEQUAL;
                        printf("[%d]: count=%d, datatype: MPI_UNSIGNED block: %d "
                               " element:%d differ. Found:%u Expected: %u\n",
                               rank, i, current->blockid, j, uibufp[j], uitmp );
                    }
                }
            }
            else if ( current->dat ==  MPI_LONG) {
                lbufp = (long *) ((char*)bufp+current->disp);
                for ( j = 0; j < current->count; j++) {
                    NEXTLONG(ltmp);
                    if ( lbufp[j] != ltmp ) {
                        result = MPI_UNEQUAL;
                        printf("[%d]: count=%d, datatype: MPI_LONG block: %d "
                               " element:%d differ. Found:%ld Expected: %ld\n",
                               rank, i, current->blockid, j, lbufp[j], ltmp );
                    }
                }
            }
            else if ( current->dat ==  MPI_UNSIGNED_LONG) {
                ulbufp = (unsigned long *) ((char*)bufp+current->disp);
                for ( j = 0; j < current->count; j++) {
                    NEXTULONG(ultmp);
                    if ( ulbufp[j] != ultmp ) {
                        result = MPI_UNEQUAL;
                        printf("[%d]: count=%d, datatype: MPI_UNSIGNED_LONG "
                               "block: %d element:%d differ. Found:%lu "
                               "Expected: %lu\n", rank, i, current->blockid, j, 
                               ulbufp[j], ultmp );
                    }
                }
            }
            else if ( current->dat ==  MPI_FLOAT) {
                fbufp = (float *) ((char*)bufp+current->disp);
                for ( j = 0; j < current->count; j++) {
                    NEXTFLOAT(ftmp);
                    if ( fbufp[j] != ftmp ) {                        
                        result = MPI_UNEQUAL;
                        printf("[%d]: count=%d, datatype: MPI_FLOAT block: %d "
                               " element:%d differ. Found:%f Expected: %f\n",
                               rank, i, current->blockid, j, fbufp[j], ftmp );
                    }
                }
            }
            else if ( current->dat ==  MPI_DOUBLE) {
                dbufp = (double *) ((char*)bufp+current->disp);
                for ( j = 0; j < current->count; j++) {
                    NEXTDOUBLE(dtmp);
                    if ( dbufp[j] != dtmp ) {
                        result = MPI_UNEQUAL;
                        printf("[%d]: count=%d, datatype: MPI_DOUBLE block: %d "
                               " element:%d differ. Found:%lf Expected: %lf\n",
                               rank, i, current->blockid, j, dbufp[j], dtmp );
                    }
                }
            }
#ifdef HAVE_LONG_DOUBLE
            else if ( current->dat ==  MPI_LONG_DOUBLE ) {
                ldbufp = (long double *) ((char*)bufp+current->disp);
                for ( j = 0; j < current->count; j++) {
                    NEXTLONGDOUBLE(ldtmp);
                    if ( ldbufp[j] != ldtmp ) {
                        result = MPI_UNEQUAL;
                        printf("[%d]: count=%d, datatype: MPI_LONG_DOUBLE block: %d "
                               " element:%d differ. Found:%lf Expected: %lf\n",
                               rank, i, current->blockid, j, ldbufp[j], ldtmp );
                    }
                }
            }
#endif
            else {
                printf("EDDHR_testdata: unknown/unimplemented basic "
                       "datatype\n");
                return ( MPI_ERR_TYPE );
            }
            current = current->next;
        }
    }

    return ( result);
}
/**********************************************************************/
/**********************************************************************/
/**********************************************************************/
static int EDDHR_setdata ( void *buf, int count, MPI_Datatype dat, 
                           EDDHR_head* head )
{
    char *bufp, *cbufp, *fromp;
    short *sbufp;
    int *ibufp;
    long *lbufp;
    unsigned short *usbufp;
    unsigned int *uibufp;
    unsigned long *ulbufp;
    float *fbufp;
    double *dbufp;
#ifdef HAVE_LONG_DOUBLE
    long double *ldbufp;
#endif

    int i, j, size, rsize;
    EDDHR_struct *current;
    
    RESETCOUNTERS(0);

    for ( i = 0; i < count; i++ ) {
        bufp = (char *)buf + i * head->extent;
        current = head->next;

        while ( current != NULL ) {
            if ( (current->dat==MPI_CHAR) || (current->dat==MPI_BYTE)){
                cbufp = (char *) ((char*)bufp+current->disp);
                size = current->count;
                while (size>0){
                    NEXTCHAR(fromp, size, rsize);
                    if ( size == rsize ) {
                        strncpy(cbufp, fromp, rsize-1 );
                        size -= rsize;
                    }
                    else {
                        strncpy(cbufp, fromp, rsize-1);
                        size-=rsize-1;
                        cbufp+=rsize-1;
                    }
                }
            }
            else if ( current->dat ==  MPI_SHORT ){
                sbufp = (short *) ((char *)bufp+current->disp);
                for ( j = 0; j < current->count; j++)
                    NEXTSHORT(sbufp[j]);
            }
            else if ( current->dat ==  MPI_UNSIGNED_SHORT) {
                usbufp = (unsigned short *) ((char *)bufp+current->disp);
                for ( j = 0; j < current->count; j++)
                    NEXTUSHORT(usbufp[j]);
            }
            else if ( current->dat ==  MPI_INT) {
                ibufp = (int *) ((char*)bufp+current->disp);
                for ( j = 0; j < current->count; j++)
                    NEXTINT(ibufp[j]);
            }
            else if ( current->dat ==  MPI_UNSIGNED) {
                uibufp = (unsigned *) ((char*)bufp+current->disp);
                for ( j = 0; j < current->count; j++)
                    NEXTUINT(uibufp[j]);
            }
            else if ( current->dat ==  MPI_LONG) {
                lbufp = (long *) ((char*)bufp+current->disp);
                for ( j = 0; j < current->count; j++)
                    NEXTLONG(lbufp[j]);
            }
            else if ( current->dat ==  MPI_UNSIGNED_LONG) {
                ulbufp = (unsigned long *) ((char*)bufp+current->disp);
                for ( j = 0; j < current->count; j++)
                    NEXTULONG(ulbufp[j]);
            }
            else if ( current->dat ==  MPI_FLOAT) {
                fbufp = (float *) ((char*)bufp+current->disp);
                for ( j = 0; j < current->count; j++)
                    NEXTFLOAT(fbufp[j]);
            }
            else if ( current->dat ==  MPI_DOUBLE) {
                dbufp = (double *) ((char*)bufp+current->disp);
                for ( j = 0; j < current->count; j++)
                    NEXTDOUBLE(dbufp[j]);
            }
#ifdef HAVE_LONG_DOUBLE
            else if ( current->dat ==  MPI_LONG_DOUBLE ) {
                ldbufp = (long double *) ((char*)bufp+current->disp);
                for ( j = 0; j < current->count; j++)
                    NEXTLONGDOUBLE(ldbufp[j]);
            }
#endif
            else {
                printf("EDDHR_setdata: unknown/unimplemented basic "
                       "datatype\n");
                return ( MPI_ERR_TYPE );
            }
            current = current->next;
        }
    }
    return ( MPI_SUCCESS );
}
/**********************************************************************/
/**********************************************************************/
/**********************************************************************/
static void EDDHR_dumplist ( EDDHR_head *head )
{
    EDDHR_struct *current;
    
    /* Print head */
    printf("Description of datatype %ld, extent=%ld combiner = %d\n",
           (long)head->dat, (long)head->extent, head->combiner );

    current = head->next;
    while ( current != NULL ) {
        printf("Block %d: Datatype=%ld, disp=%ld, count=%d, \n",
               current->blockid, (long)current->dat, (long)current->disp, current->count);
        current = current->next;
    }

    return;
}
/**********************************************************************/
/**********************************************************************/
/**********************************************************************/
static EDDHR_struct * EDDHR_nextderdatatype ( EDDHR_struct * start )
{
    EDDHR_struct *current=NULL;
    int found;

    current = start;
    found = 0;
    while ( !found ){
        /* check for first element */
        if ( current->combiner == MPI_UNDEFINED ) {
            found = 1;
            break;
        }
        if ( current->combiner == MPI_COMBINER_NAMED ) {
            if ( current->next == NULL ) {
                /* we are done, there is no more element behind this point */
                found = 1;
                current = NULL;
                break;
            }
        }
        else {
            /* hey, we found a derived datatype */
            found = 1;
            break;
        }
        current = current->next;
    }

    return current;
}
/**********************************************************************/
/**********************************************************************/
/**********************************************************************/
static void EDDHR_compactlist ( EDDHR_head* head )
{
    EDDHR_struct *current, *next;
    int done=0;
    MPI_Aint extent;
    
    current = head->next;
    while (current != NULL ) {
        done = 0;
        while ( !done ) {
            next = current->next;
            if ( next == NULL ) {
                /* nothing to do in this round*/
                break; 
            }

            MPI_Type_extent ( current->dat, &extent );
            if ( (next->disp   == (current->count*extent+current->disp))   && 
                 (current->dat == next->dat)  ) {
                /* These two blocks can be merged */
                current->count += next->count;
                REMOVEELEM ( next );
            }
            else {
                done = 1;
                break;
            }
        }
        current = current->next;
    }


    /* If everything is done, we run through again to set the block-id
       correctly */
    done = 0;
    current = head->next;
    while ( current != NULL ) {
        current->blockid = done;
        done++;
        current=current->next;
    }
        
    return;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/
static void EDDHR_freelist ( EDDHR_head *head )
{
    EDDHR_struct *current, *next;

    current = head->next;
    while (current != NULL ) {
        next = current->next;
        free ( current );
        current = next;
    }

    free ( head );
    return;
}
