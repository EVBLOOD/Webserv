#pragma once
#include <sys/event.h>
#include <sys/time.h>
#include <sys/types.h>
#include <array>
#include <cassert>
#include <iostream>
#include <map>
#include <vector>
#include "listener_interface.hpp"

class Kqueue {
    int _kdata;
    std::map<uintptr_t, IListener*> _listeners;

   private:
    int get_kdata();
    IListener& get_listener(int ident);
    std::pair<int, struct kevent*> get_kevents();

   public:
    Kqueue();
    std::vector<IListener*> get_events();
    void attach(IListener* listener);
    void detach(IListener* listener);
};
