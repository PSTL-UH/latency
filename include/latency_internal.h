
#ifndef __LATENCY_INTERNAL_H__
#define __LATENCY_INTERNAL_H__

#include "mpi.h"
#include <string.h>


#define LATENCY_OVERLAP_ADAPT_PSIZE 0
#define LATENCY_OVERLAP_3DMESH      1

#define CHECK_LAT_NUM_SEG(_numseg) {if (_numseg < 1 ) {\
                              printf(" Invalid number of segments. " \
				     "Cannot split a message into %d segments\n", \
                                     _numseg);\
                              MPI_Abort ( MPI_COMM_WORLD, 1); \
                            } }

/* Macros checking for various info-values */
#define CHECK_INFO_FOR_NUMSEG(_info,_numseg) {                           \
            int _flag; char _key[20], _value[10];                        \
            strncpy(_key,"lat_info_numseg", strlen("lat_info_numseg")+1); \
            MPI_Info_get(_info, _key, 10, _value, &_flag);               \
            if ( _flag )                                                 \
                _numseg = atoi(_value);                                  \
            else                                                         \
                _numseg = 1; }

#define CHECK_INFO_FOR_TESTRESULT(_info,_testresult) {                   \
            int _flag; char _key[20], _value[10];                        \
            _testresult = 0;                                             \
            strncpy(_key,"lat_info_testresult", strlen("lat_info_testresult")+1); \
            MPI_Info_get(_info, _key, 10, _value, &_flag);               \
            if ( _flag )                                                 \
               if ( !strncmp(_value,"true",strlen("true")+1) )           \
                  _testresult = 1;  }

#define CHECK_INFO_FOR_OVERLAP(_info,_overlap) {                         \
            int _flag; char _key[20], _value[10];                        \
            _overlap = 0;                                                \
            strncpy(_key,"lat_info_overlap", strlen("lat_info_overlap")+1); \
            MPI_Info_get(_info, _key, 10, _value, &_flag);               \
            if ( _flag )                                                 \
               if ( !strncmp(_value,"true",strlen("true")+1) )           \
                  _overlap = 1;  }
 

#define CHECK_INFO_FOR_ATOMICITY(_info,_atomicity) {                         	\
            int _flag; char _key[20], _value[10];                       	\
            _atomicity = 0;                                                	\
            strncpy(_key,"lat_info_atomicity", strlen("lat_info_atomicity")+1); \
            MPI_Info_get(_info, _key, 10, _value, &_flag);               	\
            if ( _flag )                                                 	\
               if ( !strncmp(_value,"true",strlen("true")+1) )           	\
                  _atomicity = 1;  }


static int _lat_next_msglen ( int maxcount, int dsize, int numseg, int clen )
{
    int tmpmax;
    int ret;
    int tmpstep;

    if ( (maxcount*dsize) > MAX_SHORT_LEN )
        tmpmax = MAX_SHORT_LEN/dsize;
    else
        tmpmax = (maxcount * dsize );

    tmpstep = (int) (floor(LARGE_STEP/(dsize * numseg))) * numseg;

    if ( clen < tmpmax )
        ret = clen * 2;
    else
        ret = clen + tmpstep;

    return ret;
}

static int _lat_file_next_msglen ( int maxcount, int dsize, int numseg, int clen )
{
    int tmpmax;
    int ret;
    int tmpstep;

    if ( (maxcount*dsize) > MAX_SHORT_LEN )
        tmpmax = MAX_SHORT_LEN/dsize;
    else
        tmpmax = (maxcount * dsize );

    tmpstep = (int) (floor(FILE_LARGE_STEP/(dsize * numseg))) * numseg;

    if ( clen < tmpmax )
        ret = clen * 2;
    else
        ret = clen + tmpstep;

    return ret;
}




#endif /* __LATENCY_INTERNAL_H__ */
