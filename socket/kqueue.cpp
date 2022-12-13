
#include "kqueue.hpp"
#include <fcntl.h>
#include <sys/event.h>
#include <unistd.h>
#include <cassert>
#include <iostream>
#include "../tools.hpp"
using namespace tools;

Kqueue::Kqueue() : _kdata(kqueue()), _listeners() {
    if (_kdata == -1) {
        std::cerr << "kqueue is joking!\n";
        exit(1);
    }
};

int Kqueue::get_kdata() {
    return _kdata;
};

void Kqueue::attach(IListener* listener) {
    int fd = listener->get_raw_fd();
    assert(fd != -1);
    if (_listeners.find(fd) != _listeners.end()) {
        assert(false);
    }
    struct kevent evSet;
    bzero(&evSet, sizeof(struct kevent));
    EV_SET(&evSet, fd, EVFILT_READ | EVFILT_WRITE | EVFILT_EXCEPT | EVFILT_AIO,
           EV_ADD, 0, 0, NULL);
    if (kevent(_kdata, &evSet, 1, NULL, 0, NULL) == -1) {
        std::cerr << "kevent is joking!\n";
        exit(1);
    }
    _listeners[fd] = listener;
};

void Kqueue::detach(IListener* listener) {
    assert(listener != NULL);
    assert(_listeners.size() != 0);
    std::map<uintptr_t, IListener*>::iterator it =
        _listeners.find(listener->get_raw_fd());
    if (it == _listeners.end()) {
        std::cerr << G(ERROR) << " trying to detach a non existing listener\n";
        return;
    }
    struct kevent evSet;
    bzero(&evSet, sizeof(struct kevent));
    EV_SET(&evSet, listener->get_raw_fd(),
           EVFILT_READ | EVFILT_WRITE | EVFILT_EXCEPT | EVFILT_AIO, EV_DELETE,
           0, 0, 0);
    kevent(_kdata, &evSet, 1, NULL, 0, 0);
    size_t x = _listeners.size();
    _listeners.erase(it);
    assert(x > _listeners.size());
};

size_t Kqueue::size() const {
    return _listeners.size();
};

IListener& Kqueue::get_listener(int ident) {
    return *_listeners.at(ident);
};

IListener& Kqueue::get_event() {
    Kevent kv;
    bzero(&kv, sizeof(Kevent));

    if (kevent(_kdata, NULL, 0, &kv, 1, NULL) == -1) {
        std::cerr << "Error : " << strerror(errno) << '\n';
        exit(1);
    }

    IListener* ret = _listeners.at(kv.ident);
    ret->set_kevent(kv);
    std::cerr << G(INFO) << " Getting event .." << '\n';
    std::cerr << G(INFO) << " type " << kv.filter << " data " << kv.data
              << '\n';
    std::cerr << G(INFO) << " client fd ==  " << ret->get_raw_fd() << '\n';
    return *ret;
};
