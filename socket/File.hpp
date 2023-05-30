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
    virtual ~File();
    int get_raw_fd() const;
};
