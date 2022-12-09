#pragma once
#include <unistd.h>
#include <array>
#include <cstring>
#include "listener_interface.hpp"
#include "tcpListener.hpp"

class TcpListener;
class TcpStream : public IListener {
    const int _fd;
    const TcpListener& _owner;
    Kevent _event;
    std::string _buffer;

   public:
    TcpStream(int fd, const TcpListener& owner);
    std::string get_port() const;
    std::string get_host() const;
    virtual ~TcpStream();
    int get_raw_fd() const;
    Kevent get_kevent() const;
    void set_kevent(Kevent kv);
    void shutdown();
    std::string get_buffer() const;
    size_t read(char* buff, size_t size) const;
    size_t write(const char* const buff, size_t size) const;
    void add_to_buffer(std::string tail);
    void clear_buffer();
};
