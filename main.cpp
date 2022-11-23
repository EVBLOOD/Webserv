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
int main()
{
    infos *ret;
    infos hints;
    ret = NULL;
    bzero(&hints, sizeof(infos));
    hints.ai_family = AF_UNSPEC;//AF_INET;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    if (getaddrinfo("localhost", "8080", &hints, &ret) == -1)
    {
        std::cerr << "get address info joking!\n";
        return 1;
    }
    int file_ds = socket(ret->ai_family, ret->ai_socktype ,ret->ai_protocol);
    if (file_ds == -1)
    {
        std::cerr << "socket creating error!\n";
        return 1;
    }
    fcntl(file_ds, F_SETFL, O_NONBLOCK);
    // if (connect(file_ds, ret->ai_addr, ret->ai_addrlen) == -1) // just for client code !
    // {
    //     std::cerr << "connect error!\n";
    //     return 1;
    // }
    if (bind(file_ds, ret->ai_addr, ret->ai_addrlen) == -1)
    {
        std::cerr << "binding error!\n";
        return 1;
    }
    if (listen(file_ds, 10) == -1)
    {
        std::cerr << "listning ? !\n";
        return 1;
    }
    int new_fileds;
    storage getinfos;
    socklen_t getinfos_size = sizeof(storage);
    new_fileds = accept(file_ds, (struct sockaddr *)&getinfos, &getinfos_size);
    if ( new_fileds == -1)
    {
        std::cerr << "accept ? !\n";
        return 1;
    }
    char req[100000];
    while (1)
    {
        int rd = recv(new_fileds, &req, 100000, 0);
        if (rd == 0)
        {
            std::cerr << "connection end\n";
            return 1;
        }
        if (rd == -1)
        {
            std::cerr << "connection error !\n";
            return 1;
        }
        req[rd] = '\0';
        std::cout << req << "\n";
        // send(new_fileds, "Hello, world!", 13, 0);
        std::string x = "content-type: text/html;\r\n<h1>helooe</h1>\r\n\r\n";
        write(new_fileds, x.c_str(), 66);
    }
    // while (1);
    freeaddrinfo(ret);
    close(file_ds);
}