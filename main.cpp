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
#include <cstdlib>
#include <malloc/_malloc.h>
#include <netinet/in.h>
#include <ostream>
#include <string>
#include <sys/_types/_size_t.h>
#include <sys/_types/_socklen_t.h>
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
#include <vector>
#include <cstring>
typedef struct addrinfo infos;
typedef struct sockaddr_storage storage;
#include <sys/event.h>
#include <sys/time.h>

int create_socket_and_listen(std::string host, std::string port, int number)
{
    infos *addr;
    infos hints;

    memset(&hints, 0, sizeof hints);
    hints.ai_protocol = IPPROTO_TCP; // I think I can remove this one!
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if (getaddrinfo(host.c_str(), port.c_str(), &hints, &addr) == -1)
    {
        std::cerr << "get info is joking!\n";
        exit (1);
    }
    int s = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
    int enable = 1;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR,  &enable, sizeof(enable));
    if (bind(s, addr->ai_addr, addr->ai_addrlen) == -1)
    {
        std::cerr << "bind is joking!\n";
        exit (1);
    }
    if (listen(s, number) == -1)
    {
        std::cerr << "listen is joking!\n";
        exit (1);
    }
    freeaddrinfo(addr);
    return s;
}

int init_kqueue(int number, int *data)
{
    struct kevent evSet;
    int kq;
    kq = kqueue();
    if (kq == -1)
    {
        std::cerr << "kqueue is joking!\n";
        exit (1);
    }
    for (int i = 0; i < number; i++)
    {
        bzero(&evSet, sizeof(struct kevent));
        EV_SET(&evSet, data[i], EVFILT_READ, EV_ADD, 0, 0, NULL);
        if (kevent(kq, &evSet, 1, NULL, 0, NULL) == -1)
        {
            std::cerr << "kevent is joking!\n";
            exit (1);
        }
    }
    return (kq);
}
 
 // at the last moment : I found this and it does everything => https://nima101.github.io/io_multiplexing
void kJob(int kdata, int *data)
{
    struct kevent change, event;
    struct kevent evSet;
    while (true)
    {
        int fd = kevent(kdata, &change, 1, &event, 1, NULL);
        if (fd == -1)
        {
            std::cerr << " kevent before accept is joking!\n";
            exit (1);
        }
        int rwfd = accept(event.ident, NULL, NULL);
        if (rwfd == -1)
        {
            std::cerr << "accept is joking!\n";
            exit (1);
        }
                                                    //keep-alive = ok pair<>
        /*
            {
                bzero(&evSet, sizeof(struct kevent));
                EV_SET(&evSet, rwdf, EVFILT_READ, EV_ADD, 0, 0, NULL);
                if (kevent(kq, &evSet, 1, NULL, 0, NULL) == -1)
                {
                    std::cerr << "kevent is joking!\n";
                    exit (1);
                }
        */
        /*
             else if (isListeningSocket(webserv))
            {
                struct sockaddr_storage addr;
                socklen_t socklen = sizeof(addr);
                int connfd = accept(webserv.changelist.ident, (struct sockaddr *)&addr, &socklen);
                webserv.peer= new peer_socket_t();
                bzero(webserv.peer, sizeof(peer_socket_t));
                webserv.peer->fd = connfd;
                bzero(&(webserv.changelist), sizeof(webserv.changelist));
                EV_SET(&webserv.changelist, connfd, EVFILT_READ, EV_ADD, 0, 0, (void*)webserv.peer);
                if (kevent(webserv.kq, &webserv.changelist, 1, NULL, 0, 0) == -1)
                    handel_error("kevent() failed");
            }
        */
        // 
        // std::cout << "Read?\n";
        // write(rwfd, "200 1.1 OK\r\nLocation: / \r\nContent-Type text/html\r\n\r\n<h1>hello, world</h1>\r\n",strlen("200 1.1 OK\r\nLocation: / \r\nContent-Type text/html\r\n\r\n<h1>hello, world</h1>\r\n"));
        // close(rwfd);
        // }
        // // exit (0);
        // EV_SET(&evSet, rwfd, EVFILT_READ, EV_ADD, 0, 0, NULL);
        // if (kevent(kdata, &evSet, 1, NULL, 0, NULL) == -1)
        // {
        //     std::cerr << "kevent is joking!\n";
        //     exit (1);
        // }
        // char buffer[1025];
        // // std::string request_text = std::string();
        // int rec;
        // // while (1)
        // // {
        // rec = recv(rwfd, &buffer, 1024, 0);
        /*
                   write("200 1.1 OK\r\nLocation: /\r\nContent-Type text/html\r\n<h1>hello, world</1>");
        */
        // send(int, const void *, size_t, int)
        // if (rec == -1)
        // {
        //     std::cerr << "recv is joking!\n";
        //     exit (1);
        // }
        // buffer[rec] = '\0';
        // std::cout << buffer << "\n";
        // // EV_SET(evSet, sckfd, EVFILT_READ, EV_DELETE, 0, 0, 0); when needs to remove
    }
}

int main()
{
    int number = 5;
    int data[number];
    std::string y[5] = {"8081", "1037", "1125", "1319", "1331"}; // Number to change

    for (int i = 0; i < number; i++)
        data[i] = create_socket_and_listen("localhost", y[i], number);
    int kdata;
    kdata = init_kqueue(number, data);
    kJob(kdata, data);
    // ---------------------------------------------------------------------------------- TODO: <kqueue() code/> Done [ ] ---------------------------------------------------
   
}
    // int pollre;
    // int i;
    // ---------------------------------------------------------------------------------- TODO: <Poll() code/> Done [√] ---------------------------------------------------
    // storage x;
    // int rwfd;
    // socklen_t len = sizeof(storage);
    // int clnt = 5;
    // int crl = 0;
    // while (1)
    // {
    //     std::cout << crl << '\n';
    //     crl++;
    //     pollre = poll(data, clnt, -1);
    //     // clnt++;
    //     if (pollre == -1)
    //     {
    //         std::cerr << "pool is joking!\n";
    //         exit (1);
    //     }
    //     else
    //     {
    //         for (i = 0; i < 5 ; i++){
    //             if (data[i].revents == POLLIN || data[i].revents == POLLOUT)
    //             {
    //                 break;
    //             }
    //         }
    //         if (i > 5)
    //         {
    //             std::cerr << "pool return is joking!\n";
    //             exit (1);
    //         }
    //         rwfd = accept(data[i].fd, NULL, NULL);
    //         if (rwfd == -1)
    //         {
    //             std::cerr << "accept is joking!\n";
    //             exit (1);
    //         }
    //         char buffer[1025];
    //         // std::string request_text = std::string();
    //         int rec;
    //         // while (1)
    //         // {
    //         rec = recv(rwfd, &buffer, 1024, 0);
    //         if (rec == -1)
    //         {
    //             std::cerr << "recv is joking!\n";
    //             exit (1);
    //         }
    //         buffer[rec] = '\0';
    //         std::cout << "\n------------------------------------------------------------------------------------" << std::endl;
    //         std::cout << buffer;
    //         std::cout << "\n------------------------------------------------------------------------------------" << std::endl;
    //         std::vector<std::string> requestlines;
    //         std::string s = buffer;

    //         size_t pos = 0;
    //         std::string token;
    //         while ((pos = s.find("\r\n")) != std::string::npos) {
    //             token = s.substr(0, pos);
    //             requestlines.push_back(token);
    //             s.erase(0, pos + 2);
    //         }
    //         std::cout << "Done : " << "\n";
    //         std::vector<std::string>::iterator b = requestlines.begin();
    //         std::vector<std::string>::iterator e = requestlines.end();
    //         std::vector<std::vector<std::string> > jokes;
    //         while (b != e)
    //         {
    //             std::vector<std::string> xx;
    //             std::cout << "{{{{" << *b << "}}}}}}" << std::endl;
    //             s = *b;
    //             size_t pos = 0;
    //             std::string token;
    //             while ((pos = s.find(" ")) != std::string::npos || (pos = s.find("\r\n")) != std::string::npos) {
    //                 token = s.substr(0, pos);
    //                 xx.push_back(token);
    //                 std::cout << "|" << token << "|";
    //                 s.erase(0, pos + 1);
    //             }
    //             std::cout << "\n";
    //             jokes.push_back(xx);
    //             b++;
    //         }

            // exit (1);
            // while (1) // dealing with request;
            // {
            //     std::string word = strtok(buffer, " ");
            //     if (word == "GET")
            //     {
            //         std::cout << "GET";
            //         std::string PATH(strtok((char *)word.c_str(), " "));
            //         std::cout << "PATH => " << PATH << "\n";
            //         exit (0);

            //     }
            // }
            // else if (rec == 0)
            //     close(rwfd);
            // rec = recv(rwfd, buffer, 1, 0);
            // if (rec <= 0)
            //     break;
            // request_text += buffer;
            // }
        // }
    // }
// 