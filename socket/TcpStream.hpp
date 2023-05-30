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
    int chunked;
    ssize_t len_chunked;

   public:
    TcpStream(int fd, const TcpListener& owner);
    const std::string& get_port() const;
    const std::string& get_host() const;
    virtual ~TcpStream();
    int get_raw_fd() const;
    Kevent get_kevent() const;
    void set_kevent(Kevent kv);
    const std::string& get_buffer_request() const;
    size_t read(char* buff, size_t size) const;
    size_t write(const char* const buff, size_t size) const;
    void add_to_request_buffer(const std::string& tail);
    void set_reponse_buffer(const std::string& tail);
    bool is_response_not_finished() const;
    const std::string& get_response_buffer() const;
    void clear_buffer();
};
