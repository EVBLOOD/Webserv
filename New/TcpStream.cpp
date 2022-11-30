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

TcpStream::TcpStream(int fd) : _fd(fd){};

void TcpStream::shutdown() {
    close(_fd);
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

TcpStream::~TcpStream() {}
