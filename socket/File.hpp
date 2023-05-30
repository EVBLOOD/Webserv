#pragma once
#include <unistd.h>
#include <array>
#include <cstring>
#include "listener_interface.hpp"
#include "tcpListener.hpp"

class File : public IListener {
    const int _fd;
    const std::string _path;
    Kevent _event;

   public:
    File(std::string path);
    std::string get_path();
    std::string get_port() const;
    std::string get_host() const;
    virtual ~File();
    int get_raw_fd() const;
    Kevent get_kevent() const;
    void set_kevent(Kevent kv);
};
