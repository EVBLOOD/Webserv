/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakllam <sakllam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/20 13:03:58 by sakllam           #+#    #+#             */
/*   Updated: 2022/11/21 16:41:34 by sakllam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
typedef struct sockaddr_in sa;

class socket
{
    int fd;

    public:
        socket(int domain, int type, int protocol);
        ~socket();
};

socket::socket(int domain, int type, int protocol) : fd(::socket(domain, type, protocol))
{
    if (fd == -1)
        exit (1);
    sa x;
    /*
        struct addrinfo hints, *res;

    struct addrinfo hints, *res;

// do the lookup
// [pretend we already filled out the "hints" struct]
getaddrinfo("www.example.com", "http", &hints, &res);

// again, you should do error-checking on getaddrinfo(), and walk
// the "res" linked list looking for valid entries instead of just
// assuming the first one is good (like many of these examples do).
// See the section on client/server for real examples.

s = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    // do the lookup
    // [pretend we already filled out the "hints" struct]
    getaddrinfo("www.example.com", "http", &hints, &res);

    // again, you should do error-checking on getaddrinfo(), and walk
    // the "res" linked list looking for valid entries instead of just
    // assuming the first one is good (like many of these examples do).
    // See the section on client/server for real examples.

    s = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    */
}
/*
int socket(int domain, int type, int protocol);
*/

/*
int bind(int socket, const struct sockaddr *address, socklen_t address_len);
*/
/*
 to create an IPv4/TCP socket, applications make the call:

      s = socket(AF_INET, SOCK_STREAM, 0);

   To create an IPv4/UDP socket, applications make the call:

      s = socket(AF_INET, SOCK_DGRAM, 0);
*/

/*
struct addrinfo {
    int              ai_flags;     // AI_PASSIVE, AI_CANONNAME, etc.
    int              ai_family;    // AF_INET, AF_INET6, AF_UNSPEC
    int              ai_socktype;  // SOCK_STREAM, SOCK_DGRAM
    int              ai_protocol;  // use 0 for "any"
    size_t           ai_addrlen;   // size of ai_addr in bytes
    struct sockaddr *ai_addr;      // struct sockaddr_in or _in6
    char            *ai_canonname; // full canonical hostname

    struct addrinfo *ai_next;      // linked list, next node
};
inet_pton() -- > to convert the IP to binary representations
gethostbyname() to do DNS lookups
*/