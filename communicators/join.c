/* This routine creates an inter-communicator
 * between two processes, which are connected
 * by a socket connection. 
 */

#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include "latency.h"

#define LAT_PORT_NUM 10000
#define RECONN_MAX   20

MPI_Comm LAT_comm_join_brother ( MPI_Info info )
{
    int sock, fd, ret, rank, i;
    int portnum=LAT_PORT_NUM;
    char hostname[MAXHOSTNAMELEN];
    /*    char domainname[MAXHOSTNAMELEN]; */
    struct sockaddr_in lserver;
    struct sockaddr Address;
    socklen_t  AddressLength;

    MPI_Comm icomm=MPI_COMM_NULL;

    MPI_Comm_rank ( MPI_COMM_WORLD, &rank );

    /* Tell the other side who we are */
    gethostname(hostname, MAXHOSTNAMELEN);
    /*    getdomainname(domainname, MAXHOSTNAMELEN); */
    

    /* establish first a socket connection */
    if ((sock = socket (AF_INET, SOCK_STREAM, 0)) == -1) {
        fprintf (stderr,"socket: socket call failed, error:%s",
                 strerror(errno));
    }
    
    for (i=0; i<RECONN_MAX; i++, portnum++) {
        lserver.sin_family      = AF_INET;
        lserver.sin_addr.s_addr = INADDR_ANY;
        lserver.sin_port        = htons (portnum);
        if (bind (sock, (struct sockaddr*) &lserver, sizeof (lserver))){
            fprintf (stderr,"bind: failed to bind to port:%d Error:%s",
                     portnum, strerror(errno));
        }
        else {
            break;
        }
    }
    printf("Contact info for socket: %s %d\n", hostname, portnum);

    if( listen (sock, 5) < 0 ){
        fprintf (stderr, "listen: Failed to listen to sock:%d "
                 "error:%s", sock, strerror(errno));
    }

    AddressLength = sizeof(Address);
    do {
        fd = accept ( sock, &Address, &AddressLength );
        
    } while( (fd==-1) && (errno==EINTR) );
    
    if ( fd < 0 ){
        fprintf (stderr, "listen: Failed to listen at socket %d "
                 "error:%s", sock, strerror(errno));
    }     

    /* Call Comm_join */
    ret = MPI_Comm_join (fd, &icomm);
    if ( ( ret != MPI_SUCCESS) || ( icomm == MPI_COMM_NULL)) {
        printf("Error connecting to service, ret = %d\n", ret );
        MPI_Abort ( MPI_COMM_WORLD, ret );
    }

    /* Close the socket */
    close (fd);

    return (icomm);
}

MPI_Comm LAT_comm_join_sister ( MPI_Info info, int argc, char **argv )
{
    int reconn=0, conn_ret=-1;
    int port, ret, fd;
    int rank;
    char hostname[MAXHOSTNAMELEN];

    struct sockaddr_in server;
    struct hostent     *host;

    MPI_Comm icomm=MPI_COMM_NULL;

    MPI_Comm_rank ( MPI_COMM_WORLD, &rank );
    if ( argc < 3 ) {
        printf("usage: join-connect <host_name> <port_number>\n");
        MPI_Abort ( MPI_COMM_WORLD, 1);
    }
    strcpy (hostname, argv[1]);
    port = atoi(argv[2]);

    if ( (host = gethostbyname ( hostname ) ) == NULL ) {
        fprintf(stderr, "gethostbyname: failed to get hostname:%s," 
                "syserr:%s", hostname, strerror(h_errno) );
    }
    memcpy (&server.sin_addr, host->h_addr, host->h_length );
    server.sin_family = AF_INET;
    server.sin_port   = htons ( port );


    /* establish first a socket connection */
    do {
        printf("socket\n");
        if ((fd = socket (AF_INET, SOCK_STREAM, 0)) == -1) {
            fprintf (stderr,"socket: socket call failed, error:%s",
                     strerror(errno));
        }
        do { 
            printf("connect\n");
            conn_ret = connect(fd,(struct sockaddr *)&server, sizeof(server));
            if (conn_ret < 0) sleep (1);        
        } while( (conn_ret < 0) && (errno==EINTR));
        
        if ( conn_ret < 0 ) {
            fprintf(stderr, "Connect %d failed to host %s on port %d \n", 
                    reconn, hostname, server.sin_port);
            close(fd);
            reconn++;
            sleep (1);
	}      
    } while ( (conn_ret < 0) && (reconn < RECONN_MAX ) ) ;

    /* Establish connection */
    ret = MPI_Comm_join ( fd, &icomm );
    if ( ( ret != MPI_SUCCESS) || ( icomm == MPI_COMM_NULL)) {
        printf("%d: Error connecting to service, ret = %d\n", rank, ret );
        MPI_Abort ( MPI_COMM_WORLD, ret );
    }
    
    /* Close socket */
    close (fd);

    return (icomm);
    
}
