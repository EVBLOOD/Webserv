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
int main()
{

}