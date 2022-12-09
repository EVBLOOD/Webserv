#include "TcpStream.hpp"
#include <sys/socket.h>

#define MSG_CONNTERM 0x80
TcpStream::TcpStream(int fd, const TcpListener& owner)
    : _fd(fd), _owner(owner) {
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


std::string TcpStream::get_buffer() const {
    return _buffer;
};

void TcpStream::add_to_buffer(std::string tail) {
    _buffer = _buffer + tail;
};

void TcpStream::clear_buffer() {
    _buffer = std::string("");
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
