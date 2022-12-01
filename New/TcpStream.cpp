#include "TcpStream.hpp"
#include <sys/socket.h>

// class TcpStream {
//     int fd;

//    public:
//     TcpStream(int fd);
//     void shutdown();
//     size_t read(char* buff, size_t size);
//     size_t write(char* buff, size_t size);
// };
//

TcpStream::TcpStream(int fd, const TcpListener& owner)
    : _fd(fd), _owner(owner){};

void TcpStream::shutdown() {
    _owner.delete_client(this);
};

size_t TcpStream::read(char* buff, size_t size) const {
    return recv(_fd, buff, size, 0);
};

size_t TcpStream::write(const char* const buff, size_t size) const {
    return send(_fd, buff, size, 0);
};

int TcpStream::get_raw_fd() const {
    return _fd;
}

TcpStream::~TcpStream() {
    close(_fd);
}

std::string TcpStream::get_port() const {
    return _owner.get_port();
};
std::string TcpStream::get_host() const {
    return _owner.get_host();
};
