#include "TcpStream.hpp"
#include <sys/socket.h>

TcpStream::TcpStream(int fd, const TcpListener& owner)
    : _fd(fd), _owner(owner){};

void shutdown_helper(int fd) {
    shutdown(fd, SHUT_RDWR);
    close(fd);
}

size_t TcpStream::read(char* buff, size_t size) const {
    assert(_fd != -1);
    return recv(_fd, buff, size, 0);
};

size_t TcpStream::write(const char* const buff, size_t size) const {
    assert(_fd != -1);
    return send(_fd, buff, size, 0);
};

int TcpStream::get_raw_fd() const {
    return _fd;
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
