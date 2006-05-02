#include "latency.h"

#ifdef ISEND_IRECV
#include "isend_irecv.h"
#endif

#ifdef SEND_RECV
#include "send_recv.h"
#endif

#ifdef ISEND_RECV
#include "isend_recv.h"
#endif

#ifdef SEND_IRECV
#include "send_irecv.h"
#endif

#ifdef SENDRECV
#include "sendrecv.h"
#endif

#ifdef PUT_WINFENCE
#include "put_winfence.h"
#endif

#ifdef GET_WINFENCE
#include "get_winfence.h"
#endif

#ifdef PUT_WINFENCE_DUPLEX
#include "put_winfence_duplex.h"
#endif

#ifdef GET_WINFENCE_DUPLEX
#include "get_winfence_duplex.h"
#endif

#ifdef PUT_STARTPOST
#include "put_startpost.h"
#endif

#ifdef GET_STARTPOST
#include "get_startpost.h"
#endif

#ifdef PUT_STARTPOST_DUPLEX
#include "put_startpost_duplex.h"
#endif

#ifdef GET_STARTPOST_DUPLEX
#include "get_startpost_duplex.h"
#endif
