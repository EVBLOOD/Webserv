// for understanding perposes:
/*
------------
gai_strerror : get error message string from EAI_xxx error code  --> error returned by : getaddrinfo and getnameinfo
includes needed:
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
 --> proto
const char * gai_strerror(int ecode);
errors =>
    EAI_AGAIN     temporary failure in name resolution
    EAI_BADFLAGS  invalid value for ai_flags
    EAI_BADHINTS  invalid value for hints
    EAI_FAIL      non-recoverable failure in name resolution
    EAI_FAMILY    ai_family not supported
    EAI_MEMORY    memory allocation failure
    EAI_NONAME    hostname or servname not provided, or not known
    EAI_OVERFLOW  argument buffer overflow
    EAI_PROTOCOL  resolved protocol is unknown
    EAI_SERVICE   servname not supported for ai_socktype
    EAI_SOCKTYPE  ai_socktype not supported
    EAI_SYSTEM    system error returned in errno
    ----> return a string contains a msg or NULL if the space is not enough for it + sets an "errno"
---------
htons, htonl, ntohs, ntohl : convert values between host and network byte order
---------

select, poll, epoll (epoll_create, epoll_ctl,
epoll_wait), kqueue (kqueue, kevent), socket,
accept, listen, send, recv, bind, connect,
getaddrinfo, freeaddrinfo, setsockopt, getsockname,
getprotobyname, fcntl
*/
/* ----> this is for socket function!
 - domain:
We use AF_ LOCAL as defined in the POSIX standard for communication between processes on the same host
For communicating between processes on different hosts connected by IPV4, we use AF_INET
For communicating between processes on different hosts connected by IPV6, we use AF_INET6
 - type:
SOCK_STREAM: TCP
SOCK_DGRAM: UDP
 - protocol:
we use 0 for the IP -check /etc/protocols-
This is the same number which appears on protocol field in the IP header of a packet
*/
/* --> this is for setsockopt function
This helps in manipulating options for the socket referred by the file descriptor sockfd. This is completely optional, but it helps in reuse of address and port. Prevents error such as: “address already in use”.
*/
/* ---> this is for bind function
 binds the socket to the address and port number specified in addr(custom data structure)
*/
// #include "socket.hpp"
#include <netinet/in.h>
#include <string>
#include <sys/_types/_size_t.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/syslimits.h>
#include <sys/types.h>
#include <iostream>
#include <netdb.h>
#include <strings.h>
#include <system_error>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
typedef struct addrinfo infos;
typedef struct sockaddr_storage storage;
typedef struct pollfd spool;

int main()
{
    infos info;
    infos *ret[10];
    spool data[10];
    info.ai_protocol = IPPROTO_TCP;
    info.ai_flags = AI_PASSIVE;
    info.ai_family = AF_INET;
    info.ai_socktype = SOCK_STREAM;
    std::string y[10] = {"8081", "1037", "1125", "1319", "1331"};
    for (int i = 0; i < 5; i++)
    {
        if (getaddrinfo("localhost", y[i].c_str(), &info, &(ret[i])) == -1)
        {
            std::cerr << "get info is joking!\n";
            exit (1);
        }
        if ((data[i].fd = socket(ret[i]->ai_family, ret[i]->ai_socktype, ret[i]->ai_protocol)) == -1)
        {
            std::cerr << "socket is joking!\n";
            exit (1);
        }
        // fcntl(data[i].fd, F_SETFL, O_NONBLOCK);
        data[i].events = POLLIN | POLLOUT;
        if (bind(data[i].fd, ret[i]->ai_addr, ret[i]->ai_addrlen) == -1)
        {
            std::cerr << "bind is joking!\n";
            exit (1);
        }
        if (listen(data[i].fd, 5) == -1)
        {
            std::cerr << "listen is joking!\n";
            exit (1);
        }
    }
    int pollre;
    int i;
    storage x;
    int rwfd;
    socklen_t len = sizeof(storage);
    int clnt = 5;
    int crl = 0;
    while (1)
    {
        std::cout << crl << '\n';
        crl++;
        pollre = poll(data, clnt, -1);
        if (pollre == -1)
        {
            std::cerr << "pool is joking!\n";
            exit (1);
        }
        else
        {
            for (i = 0; i < 5 ; i++){
                if (data[i].revents == POLLIN || data[i].revents == POLLOUT)
                {
                    break;
                }
            }
            if (i > 5)
            {
                std::cerr << "pool return is joking!\n";
                exit (1);
            }
            rwfd = accept(data[i].fd, NULL, NULL);
            if (rwfd == -1)
            {
                std::cerr << "accept is joking!\n";
                exit (1);
            }
            char buffer[1000];
            int rec = recv(rwfd, buffer, 1000, 0);
            if (rec == -1)
            {
                std::cerr << "recv is joking!\n";
                exit (1);
            }
            buffer[rec] = '\0';
            std::cout << "\n------------------------------------------------------------------------------------" << std::endl;
            std::cout << buffer;
            std::cout << "\n------------------------------------------------------------------------------------" << std::endl;
            close(rwfd);
        }

    }
}