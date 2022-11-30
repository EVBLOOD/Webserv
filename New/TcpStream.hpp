#pragma once
#include <unistd.h>
#include <array>
#include <cstring>
#include "streamer_interface.hpp"

class TcpStream : public IStreamer {
    int _fd;

   public:
    TcpStream(int fd);
    ~TcpStream();
    int get_raw_fd() const;
    void shutdown();
    size_t read(char* buff, size_t size) const;
    size_t write(const char* const buff, size_t size) const;
};
