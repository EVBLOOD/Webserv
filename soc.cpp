#include <sys/event.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>

int main() {
    int kq = kqueue();
    // int enable = 1;
    int fd;
    { std::cout << (fd = socket(PF_INET, SOCK_STREAM, 0)) << '\n'; }
    {
        std::cout << (fd = socket(PF_INET, SOCK_STREAM, 0)) << '\n';
        struct kevent evSet;
        bzero(&evSet, sizeof(struct kevent));
        EV_SET(&evSet, fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
        std::cout << evSet.ident << '\n';
        if (kevent(kq, &evSet, 1, NULL, 0, NULL) == -1) {
            std::cerr << "kevent is joking!\n";
            exit(1);
        }
        int enable = 1;
        if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable))) {
            std::cerr << "Setsockopt failed" << strerror(errno) << std::endl;
            exit(-1);
        }
        std::cout << evSet.ident << '\n';
    }
    {
        std::cout << (fd = socket(PF_INET, SOCK_STREAM, 0)) << '\n';
        struct kevent evSet;
        bzero(&evSet, sizeof(struct kevent));
        EV_SET(&evSet, fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
        std::cout << evSet.ident << '\n';
        if (kevent(kq, &evSet, 1, NULL, 0, NULL) == -1) {
            std::cerr << "kevent is joking!\n";
            exit(1);
        }
        int enable = 1;
        if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable))) {
            std::cerr << "Setsockopt failed" << strerror(errno) << std::endl;
            exit(-1);
        }
        std::cout << evSet.ident << '\n';
    }
    {
        std::cout << (fd = socket(PF_INET, SOCK_STREAM, 0)) << '\n';
        struct kevent evSet;
        bzero(&evSet, sizeof(struct kevent));
        EV_SET(&evSet, fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
        std::cout << evSet.ident << '\n';
        if (kevent(kq, &evSet, 1, NULL, 0, NULL) == -1) {
            std::cerr << "kevent is joking!\n";
            exit(1);
        }
        int enable = 1;
        if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable))) {
            std::cerr << "Setsockopt failed" << strerror(errno) << std::endl;
            exit(-1);
        }
        std::cout << evSet.ident << '\n';
    }
    std::cout << (fd = socket(PF_INET, SOCK_STREAM, 0)) << '\n';
    struct kevent evSet;
    bzero(&evSet, sizeof(struct kevent));
    EV_SET(&evSet, fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
    std::cout << evSet.ident << '\n';
    if (kevent(kq, &evSet, 1, NULL, 0, NULL) == -1) {
        std::cerr << "kevent is joking!\n";
        exit(1);
    }
    int enable = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable))) {
        std::cerr << "Setsockopt failed" << strerror(errno) << std::endl;
        exit(-1);
    }
    std::cout << evSet.ident << '\n';
    // std::cout << (fd = socket(PF_UNIX, SOCK_STREAM, 0)) << '\n';
    // struct kevent evSet;
    // bzero(&evSet, sizeof(struct kevent));
    // EV_SET(&evSet, fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
    // std::cout << evSet.ident << '\n';
    // if (kevent(kq, &evSet, 1, NULL, 0, NULL) == -1) {
    //     std::cerr << "kevent is joking!\n";
    //     exit(1);
    // }
    // int enable = 1;
    // if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable))) {
    //     std::cerr << "Setsockopt failed" << strerror(errno) << std::endl;
    //     exit(-1);
    // }
    // std::cout << evSet.ident << '\n';
    listen(fd, 5);
    std::cout << "pid " << getpid() << std::endl;
    while (1)
        ;
}
