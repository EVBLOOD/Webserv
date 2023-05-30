#include "File.hpp"
#include <fcntl.h>
#include <sys/socket.h>

File::File(std::string path) : _fd(open(path.data(), O_RDONLY)), _path(path) {
    memset(&_event, 0, sizeof(Kevent));
};

int File::get_raw_fd() const {
    return _fd;
}

Kevent File::get_kevent() const {
    return _event;
};

void File::set_kevent(Kevent kv) {
    memset(&_event, 0, sizeof(Kevent));
    _event = kv;
}

static void shutdown_helper(int fd) {
    shutdown(fd, SHUT_RDWR);
    close(fd);
}

std::string File::get_path() {
    return _path;
};

File::~File() {
    shutdown_helper(_fd);
}
