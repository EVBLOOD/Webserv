#pragma once
#include <cstring>
#include "listener_interface.hpp"
#include "server_interface.hpp"
class IStreamer : public IListener {
   public:
    virtual int get_raw_fd() const = 0;
    virtual size_t read(char* buff, size_t size) const = 0;
    virtual size_t write(const char* const buff, size_t size) const = 0;
    virtual void shutdown() = 0;
};
