#pragma once
#include "listener_interface.hpp"
#include "streamer_interface.hpp"
class IServer : public IListener {
   public:
    virtual IStreamer& accept() const = 0;
    virtual int get_raw_fd() const = 0;
};
