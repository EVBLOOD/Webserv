
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

int Kqueue::get_kdata() const {
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
    EV_SET(&evSet, fd, EVFILT_READ | EVFILT_WRITE | EVFILT_EXCEPT, EV_ADD, 0, 0,
           NULL);
    if (kevent(_kdata, &evSet, 1, NULL, 0, NULL) == -1) {
        std::cerr << "kevent is joking!\n";
        exit(1);
    }
    _listeners[fd] = listener;
};

void Kqueue::detach(IListener* listener) {
    if (listener == NULL || _listeners.size() == 0)
        return;
    std::map<uintptr_t, IListener*>::iterator it =
        _listeners.find(listener->get_raw_fd());
    if (it == _listeners.end()) {
        return;
    }
    struct kevent evSet;
    bzero(&evSet, sizeof(struct kevent));
    EV_SET(&evSet, listener->get_raw_fd(),
           EVFILT_READ | EVFILT_WRITE | EVFILT_EXCEPT, EV_DELETE, 0, 0, 0);
    if (kevent(_kdata, &evSet, 1, NULL, 0, 0) == -1)
        return;
    _listeners.size();
    _listeners.erase(it);
};

size_t Kqueue::size() const {
    return _listeners.size();
};

IListener& Kqueue::get_event() const {
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
