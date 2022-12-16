#include "TcpStream.hpp"
#include <sys/socket.h>
#include <cassert>

TcpStream::TcpStream(int fd, const TcpListener& owner)
    : _fd(fd),
      _owner(owner),
      _request_buffer(),
      _response_buffer(),
      chunked(-1),
      len_chunked(0) {
    memset(&_event, 0, sizeof(Kevent));
};

size_t TcpStream::read(char* buff, size_t size) const {
    assert(_fd != -1);
    // MSG_TRUNC | MSG_DONTWAIT
    return recv(_fd, buff, size, 0);
};

size_t TcpStream::write(const char* const buff, size_t size) const {
    assert(_fd != -1);
    return send(_fd, buff, size, 0);
};

int TcpStream::get_raw_fd() const {
    return _fd;
}

Kevent TcpStream::get_kevent() const {
    return _event;
};

void TcpStream::set_kevent(Kevent kv) {
    memset(&_event, 0, sizeof(Kevent));
    _event = kv;
}

std::string TcpStream::get_buffer_request() const {
    return _request_buffer;
};

void TcpStream::set_reponse_buffer(std::string buffer) {
    _response_buffer = buffer;
};
bool TcpStream::is_response_not_finished() {
    return _response_buffer.size() != 0;
};

void TcpStream::add_to_request_buffer(std::string tail) {
    if (chunked == -1) {
        std::cout << "[" << tail << "]";
        if (tail.find("Transfer-Encoding: chunked") != std::string::npos) {
            tail = tools::dealwithchuncked_buff(tail, len_chunked, true);
            chunked = 1;
        }
        if (tail.find("Transfer-Encoding: chunked") == std::string::npos &&
            tail.find("\r\n\r\n") != std::string::npos)
            chunked = 0;
    } else if (chunked == 1) {
        // assert(false);
        tail = tools::dealwithchuncked_buff(tail, len_chunked);
    }
    _request_buffer = _request_buffer + tail;
};

std::string TcpStream::get_response_buffer() {
    return _response_buffer;
};

void TcpStream::clear_buffer() {
    chunked = -1;
    len_chunked = 0;
    _request_buffer = std::string("");
};

static void shutdown_helper(int fd) {
    shutdown(fd, SHUT_RDWR);
    close(fd);
}
TcpStream::~TcpStream() {
    shutdown_helper(_fd);
}

std::string TcpStream::get_port() const {
    return _owner.get_port();
};
std::string TcpStream::get_host() const {
    return _owner.get_host();
};
