
#include "kqueue.hpp"
#include <fcntl.h>
#include <sys/errno.h>
#include <sys/event.h>
#include <sys/queue.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <iostream>
#include "../tools.hpp"
using namespace tools;

Kqueue::Kqueue() : _kdata(kqueue()), _listeners() {
    if (_kdata == -1) {
        std::cerr << G(ERROR) << " " << strerror(errno) << '\n';
        exit(1);
    }
};

int Kqueue::get_kdata() const {
    return _kdata;
};

void Kqueue::monitor(IListener* listener) {
    int fd = listener->get_raw_fd();
    if (fd < 0)
        return;
    if (_listeners.find(fd) != _listeners.end()) {
        return;
    }
    struct kevent evSet;
    bzero(&evSet, sizeof(struct kevent));
    EV_SET(&evSet, fd, EVFILT_VNODE, EV_ADD | EV_ENABLE | EV_CLEAR, NOTE_WRITE,
           0, nullptr);
    if (kevent(_kdata, &evSet, 1, NULL, 0, NULL) == -1) {
        return;
    }
    _listeners[fd] = listener;
}

void Kqueue::attach(IListener* listener) {
    int fd = listener->get_raw_fd();
    if (fd < 0)
        return;
    if (_listeners.find(fd) != _listeners.end()) {
        return;
    }
    struct kevent evSet;
    bzero(&evSet, sizeof(struct kevent));
    EV_SET(&evSet, fd, EVFILT_READ | EVFILT_WRITE | EVFILT_EXCEPT, EV_ADD, 0, 0,
           NULL);
    if (kevent(_kdata, &evSet, 1, NULL, 0, NULL) == -1) {
        return;
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
        std::cerr << G(ERROR) << " " << strerror(errno) << '\n';
        exit(1);
    }

    IListener* ret = _listeners.at(kv.ident);

    ret->set_kevent(kv);
    return *ret;
};
