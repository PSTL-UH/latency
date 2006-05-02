/*
** internal eddhr-header. Should not be available to the end-user.
*/

#ifndef _EDDHR_INTERNAL_H_
#define _EDDHR_INTERNAL_H_

#include <string.h>

/* some macros easing my life */
#define GETFIRSTELEM(first, d) {\
        first = (EDDHR_struct *) calloc(1, sizeof(EDDHR_struct)); \
        first->dat = d; \
        first->disp = 0; \
        first->count = 1; \
        first->combiner = MPI_UNDEFINED; }

       
#define GETNEWELEM(current, newelem) {\
        newelement = (EDDHR_struct *) calloc(1, sizeof(EDDHR_struct));\
        if ( current->next != NULL ) {\
            current->next->prev = newelem;\
            newelem->next       = current->next;\
        }\
        newelem->prev     = current; \
        current->next     = newelem; \
        newelem->dat      = MPI_DATATYPE_NULL;\
        newelem->disp     = current->disp;\
        newelem->count    = 1;\
        newelem->combiner = MPI_UNDEFINED; }

#define SETDISP(current, displacement) \
              current->disp = displacement;


#define SETDAT( current, datatype )\
            current->dat = datatype;

#define GETPREVDISP(current, p ) {\
            EDDHR_struct *_prev; \
            _prev = current->prev;\
            if ( _prev != NULL ) \
                p = _prev->disp;\
            else \
                p = 0;\
            }

#define REMOVEELEM(t) {\
            EDDHR_struct *__prev, *__next; \
            __prev = t->prev; \
            __next = t->next; \
            if ( __prev != NULL ) \
               __prev->next = __next; \
            if ( __next != NULL ) \
               __next->prev = __prev; \
            free (t);} 


/* some macros for managing pointers for the setting and testing 
   data */
//#ifdef NONSENSE
//char _eddhr_chararr[]="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ`1234567890-=~!@#$%^&*()_+,./<>?;':[]{}|";

//#define MAXCHAR 92
//#endif

char _eddhr_chararr[2750]="This one needs an introduction, so you won't be lost at the beginning. This man was in an accident at work, so he filled out an insurance claim. The insurance company contacted him and asked for more information. This was his response: I am writing in response to your request for additional information, for block number 3 of the accident reporting form. I put 'poor planning' as the cause of my accident. You said in your letter that I should explain more fully and I trust the following detail will be sufficient. I am an amateur radio operator and on the day of the accident, I was working alone on the top section of my new 80-foot tower. When I had completed my work, I discovered that I had, over the course of several trips up the tower, brought up about 300 pounds of tools and spare hardware. Rather than carry the now unneeded tools and material down by hand, I decided to lower the items down in a small barrel by using the pulley attached to the gin pole at the top of the tower. Securing the rope at ground level, I went to the top of the tower and loaded the tools and material into the barrel. Then I went back to the ground and untied the rope, holding it tightly to ensure a slow decent of the 300 pounds of tools. You will note in block number 11 of the accident reporting form that I weigh only 155 pounds. Due to my surprise of being jerked off the ground so suddenly, I lost my presence of mind and forgot to let go of the rope. Needless to say, I proceeded at a rather rapid rate of speed up the side of the tower. In the vicinity of the 40-foot level, I met the barrel coming down. This explains my fractured skull and broken collarbone. Slowed only slightly, I continued my rapid ascent, not stopping until the fingers of my right hand were two knuckles deep into the pulley. Fortunately, by this time, I had regained my presence of mind and was able to hold onto the rope in spite of my pain. At approximately the same time, however, the barrel of tools hit the ground and the bottom fell out of the barrel. Devoid of the weight of the tools, the barrel now weighed approximately 20 pounds. I refer you again to my weight in block number 11. As you might imagine, I began a rapid descent down the side of the tower. In the vicinity of the 40-foot level, I met the barrel coming up. This accounts for the two fractured ankles, and the lacerations of my legs and lower body. The encounter with the barrel slowed me enough to lessen my injuries when I fell onto the pile of tools and, fortunately, only three vertebrae were cracked. I am sorry to report, however, that as I lay there on the tools, in pain, unable to stand and watching the empty barrel 80 feet above me, I again lost my presence of mind. I let go of the rope.";
#define MAXCHAR 2749

int _eddhr_nextchar=0;
short _eddhr_nextshort=0;
unsigned short _eddhr_nextushort=0;
int _eddhr_nextint=0;
unsigned int _eddhr_nextuint=0;
long _eddhr_nextlong=0;
unsigned long _eddhr_nextulong=0;
float _eddhr_nextfloat=0.0;
double _eddhr_nextdouble=0.0;
long double _eddhr_nextlongdouble=0.0;

#define RESETCOUNTERS(_offset) {\
    _eddhr_nextchar=_offset; \
    _eddhr_nextshort=_offset; \
    _eddhr_nextushort=_offset; \
    _eddhr_nextint=_offset; \
    _eddhr_nextuint=_offset; \
    _eddhr_nextlong=_offset; \
    _eddhr_nextulong=_offset; \
    _eddhr_nextfloat=(float)_offset; \
    _eddhr_nextdouble=(double)_offset; \
    _eddhr_nextlongdouble=(long double)_offset;}

#define NEXTCHAR(_i, _size, _retsize ) { if( _eddhr_nextchar >= MAXCHAR ) \
                         _eddhr_nextchar=0;\
                      if ( _eddhr_nextchar + _size > MAXCHAR ){\
                          _retsize = MAXCHAR - _eddhr_nextchar;\
                          _i = &(_eddhr_chararr[_eddhr_nextchar]);\
                          _eddhr_nextchar = 0;\
                      }\
                      else {\
                          _retsize = _size; \
                          _i = &(_eddhr_chararr[_eddhr_nextchar]);\
                          _eddhr_nextchar+=_size;\
                      }\
                     }

#define NEXTSHORT(_i) {if ( _eddhr_nextshort == (SHRT_MAX - 1) )\
                        _eddhr_nextshort = 0;\
                     else _eddhr_nextshort++; \
                     _i=_eddhr_nextshort;}

#define NEXTUSHORT(_i) {if ( _eddhr_nextushort == (USHRT_MAX - 1) )\
                        _eddhr_nextushort = 0;\
                     else _eddhr_nextushort++; \
                     _i=_eddhr_nextushort;}

#define NEXTINT(_i) {if ( _eddhr_nextint == (INT_MAX - 1) )\
                        _eddhr_nextint = 0;\
                     else _eddhr_nextint++; \
                     _i=_eddhr_nextint;}

#define NEXTUINT(_i) {if ( _eddhr_nextuint == (UINT_MAX - 1) )\
                        _eddhr_nextuint = 0;\
                     else _eddhr_nextuint++; \
                     _i=_eddhr_nextuint;}

#define NEXTLONG(_i) {if ( _eddhr_nextlong == (LONG_MAX - 1) )\
                        _eddhr_nextlong = 0;\
                     else _eddhr_nextlong++; \
                     _i=_eddhr_nextlong;}

#define NEXTULONG(_i) {if ( _eddhr_nextulong == (ULONG_MAX - 1) )\
                        _eddhr_nextulong = 0;\
                     else _eddhr_nextulong++; \
                     _i=_eddhr_nextulong;}

#define NEXTFLOAT(_i) _i=_eddhr_nextfloat++;
#define NEXTDOUBLE(_i) _i=_eddhr_nextdouble++;
#define NEXTLONGDOUBLE(_i) _i=_eddhr_nextlongdouble++;


#endif
