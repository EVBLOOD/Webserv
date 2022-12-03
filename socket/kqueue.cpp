#include "kqueue.hpp"
#include <cassert>

Kqueue::Kqueue() : _kdata(0), _change(), _listeners() {
    _kdata = kqueue();
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
    EV_SET(&evSet, listener->get_raw_fd(), EVFILT_READ, EV_ADD, 0, 0, NULL);
    if (kevent(_kdata, &evSet, 1, NULL, 0, NULL) == -1) {
        std::cerr << "kevent is joking!\n";
        exit(1);
    }
    _change.push_back(evSet);
    _listeners[evSet.ident] = listener;
};

void Kqueue::detach(IListener* listener) {
    std::map<uintptr_t, IListener*>::iterator it;
    for (it = _listeners.begin(); it != _listeners.end(); ++it)
        if (it->second == listener)
            break;
    assert(it != _listeners.end());

    std::vector<struct kevent>::iterator it_vec;
    for (it_vec = _change.begin(); it_vec != _change.end(); ++it_vec)
        if (static_cast<uintptr_t>(it_vec->ident) == it->first)
            break;
    assert(it_vec != _change.end());
    assert(static_cast<uintptr_t>(listener->get_raw_fd()) == it->first);
    _change.erase(it_vec);
    _listeners.erase(it);
};

std::pair<int, struct kevent*> Kqueue::get_kevents() {
    std::array<struct kevent, 1024> ev;
    memset(ev.data(), 0, ev.size());
    int number_of_events =
        kevent(_kdata, _change.data(), 1, ev.data(), 1, NULL);
    if (number_of_events == -1) {
        perror("Error");
        exit(1);
    }
    return std::make_pair(number_of_events, ev.data());
}

IListener& Kqueue::get_listener(int ident) {
    return *_listeners.at(ident);
};

std::vector<struct kevent> Kqueue::get_targets() {
    return _change;
};

std::vector<IListener*> Kqueue::get_events() {
    std::pair<int, struct kevent*> events = get_kevents();
    std::vector<IListener*> ret;
    int number_of_events = events.first;
    if (number_of_events == -1) {
        exit(11);
    }
    for (int i = 0; i < number_of_events; i++) {
        ret.push_back(&get_listener(events.second[i].ident));
    }

    // std::cout << "[KQUEUE] number of change " << _change.size() << '\n';
    // std::cout << "[KQUEUE] number of listners " << _listeners.size() << '\n';
    // std::cout << "[KQUEUE] number of events " << ret.size() << '\n';
    return ret;
};
