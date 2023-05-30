#pragma once
#include <sys/event.h>
#include <sys/time.h>
#include <sys/types.h>
#include <array>
#include <iostream>
#include <map>
#include <vector>
#include "listener_interface.hpp"

typedef struct kevent Kevent;

class Kqueue {
    int _kdata;
    std::map<uintptr_t, IListener*> _listeners;

   public:
    Kqueue();
    int get_kdata() const;
    IListener& get_event() const;
    void attach(IListener* listener);
    void monitor(IListener* listener);
    void detach(IListener* listener);
    size_t size() const;
};
