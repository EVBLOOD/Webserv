#pragma once
#include <sys/event.h>
#include <sys/time.h>
#include <sys/types.h>
#include <iostream>
#include <map>
#include <vector>
#include "listener_interface.hpp"
#include "streamer_interface.hpp"

class Kqueue {
    int _kdata;
    std::vector<struct kevent> _change;
    std::map<int, IListener*> _listeners;

   public:
    Kqueue(std::vector<IListener*> listeners);
    int get_kdata();
    std::vector<struct kevent> get_targets();
    std::vector<IListener*> get_listeners();
    void kqueue_job(std::function<void(Kqueue&, void*)> call_back, void* data);
    void add_listener(IListener* listener);
    IListener& get_listener(int ident);
    std::pair<int, struct kevent*> get_events();
};
