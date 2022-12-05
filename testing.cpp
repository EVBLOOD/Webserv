#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string>
#include <unistd.h>
#include <iostream>
// #include "/Users/orahmoun/webserv/nacl-newlib/newlib/libc/sys/linux/net/getaddrinfo.c"
int main(void) {
  {
    struct addrinfo* addr;
    int result = getaddrinfo("0.0.0.0", "8080", NULL, &addr);
    if (result != 0) {
      printf("Error from getaddrinfo: %s\n", gai_strerror(result));
      return 1;
    }
    freeaddrinfo(addr);
}
// close (4);
  std::string full = "lsof -p" + std::to_string(getpid());
  system(full.c_str());
std::cout << "----------------------\n";
{
  struct addrinfo* addr;
  int result = getaddrinfo("0.0.0.0", NULL, NULL, &addr);
  if (result != 0) {
    printf("Error from getaddrinfo: %s\n", gai_strerror(result));
    return 1;
  }
  freeaddrinfo(addr);

}
  // struct sockaddr_in* internet_addr = (struct sockaddr_in*) addr->ai_addr;
  // printf("google.com is at: %s\n", inet_ntoa(internet_addr->sin_addr));
  system(full.c_str());
  // return 0;
}