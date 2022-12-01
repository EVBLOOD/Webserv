#pragma once
#include <unistd.h>
#include <array>
#include <cstring>
#include "server_interface.hpp"
#include "tcpListener.hpp"

class TcpListener;
class TcpStream : public IStreamer {
    const int _fd;
    const TcpListener& _owner;

   public:
    TcpStream(int fd, const TcpListener& owner);
    std::string get_port() const;
    std::string get_host() const;
    virtual ~TcpStream();
    int get_raw_fd() const;
    void shutdown();
    size_t read(char* buff, size_t size) const;
    size_t write(const char* const buff, size_t size) const;
};
