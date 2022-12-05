#include "kqueue.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <cassert>

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
    struct kevent evSet;
    bzero(&evSet, sizeof(struct kevent));
    EV_SET(&evSet, listener->get_raw_fd(), EVFILT_READ | EVFILT_WRITE, EV_ADD,
           0, 0, NULL);
    if (kevent(_kdata, &evSet, 1, NULL, 0, NULL) == -1) {
        std::cerr << "kevent is joking!\n";
        exit(1);
    }
    // fcntl(listener->get_raw_fd(), F_SETFL, O_NONBLOCK);
    _listeners[evSet.ident] = listener;
};

void Kqueue::detach(IListener* listener) {
    struct kevent evSet;
    bzero(&evSet, sizeof(struct kevent));
    EV_SET(&evSet, listener->get_raw_fd(), EVFILT_READ | EVFILT_WRITE,
           EV_DELETE, 0, 0, 0);
    kevent(_kdata, &evSet, 1, NULL, 0, 0);
    _listeners.erase(_listeners.find(listener->get_raw_fd()));
};

std::pair<int, struct kevent*> Kqueue::get_kevents() {
    std::array<struct kevent, 1024> ev;
    memset(ev.data(), 0, ev.size() * sizeof(struct kevent));
    int number_of_events =
        kevent(_kdata, NULL, 0, ev.data(), _listeners.size(), NULL);
    std::cerr << "number_of_events ASSSD " << number_of_events << '\n';
    if (number_of_events == -1) {
        perror("Error");
        exit(1);
    }
    return std::make_pair(number_of_events, ev.data());
}

IListener& Kqueue::get_listener(int ident) {
    return *_listeners.at(ident);
};

std::vector<IListener*> Kqueue::get_events() {
    std::pair<int, struct kevent*> events = get_kevents();
    std::vector<IListener*> ret;
    int number_of_events = events.first;
    if (number_of_events == -1) {
        exit(11);
    }
    for (int i = 0; i < number_of_events; i++) {
        ret.push_back(_listeners[events.second[i].ident]);
    }
    return ret;
};
