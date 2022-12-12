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
    std::string _request_buffer;
    std::string _response_buffer;

   public:
    TcpStream(int fd, const TcpListener& owner);
    std::string get_port() const;
    std::string get_host() const;
    virtual ~TcpStream();
    int get_raw_fd() const;
    Kevent get_kevent() const;
    void set_kevent(Kevent kv);
    void shutdown();
    std::string get_buffer_request() const;
    size_t read(char* buff, size_t size) const;
    size_t write(const char* const buff, size_t size) const;
    void add_to_request_buffer(std::string tail);
    void set_reponse_buffer(std::string tail);
    bool is_response_not_finished();
    std::string get_response_buffer();
    void clear_buffer();
};
