#include "kqueue.hpp"

// class Kqueue {
//     int kq;

//    public:
//     Kqueue(std::vector<IListener*> listners);
// };

Kqueue::Kqueue(std::vector<IListener*> listeners)
    : _kdata(0), _change(), _listeners() {
    struct kevent evSet;
    _kdata = kqueue();
    if (_kdata == -1) {
        std::cerr << "kqueue is joking!\n";
        exit(1);
    }

    for (size_t i = 0; i < listeners.size(); i++) {
        bzero(&evSet, sizeof(struct kevent));
        EV_SET(&evSet, listeners[i]->get_raw_fd(), EVFILT_READ, EV_ADD, 0, 0,
               NULL);
        if (kevent(_kdata, &evSet, 1, NULL, 0, NULL) == -1) {
            std::cerr << "kevent is joking!\n";
            exit(1);
        }
        _change.push_back(evSet);
        _listeners[evSet.ident] = listeners[i];
    }
};

int Kqueue::get_kdata() {
    return _kdata;
};

void Kqueue::kqueue_job(std::function<void(Kqueue&, void*)> call_back,
                        void* data) {
    call_back(*this, data);
}

void Kqueue::add_listener(IListener* listener) {
    struct kevent evSet;
    // int kq;
    // kq = kqueue();
    // if (kq == -1) {
    //     std::cerr << "kqueue is joking!\n";
    //     exit(1);
    // }
    bzero(&evSet, sizeof(struct kevent));
    EV_SET(&evSet, listener->get_raw_fd(), EVFILT_READ, EV_ADD, 0, 0, NULL);
    if (kevent(_kdata, &evSet, 1, NULL, 0, NULL) == -1) {
        std::cerr << "kevent is joking!\n";
        exit(1);
    }
    _change.push_back(evSet);
    _listeners[evSet.ident] = listener;
};

std::pair<int, struct kevent*> Kqueue::get_events() {
    std::vector<struct kevent> ev = _change;
    int number_of_events =
        kevent(_kdata, _change.data(), 1, ev.data(), 1, NULL);
    return std::make_pair(number_of_events, ev.data());
}

IListener& Kqueue::get_listener(int ident) {
    return *_listeners.at(ident);
};

std::vector<struct kevent> Kqueue::get_targets() {
    return _change;
};

std::vector<IListener*> Kqueue::get_listeners() {
    std::pair<int, struct kevent*> events = get_events();
    std::vector<IListener*> ret;
    int number_of_events = events.first;
    if (number_of_events == -1) {
        exit(11);
    }
    for (int i = 0; i < number_of_events; i++) {
        ret.push_back(&get_listener(events.second[i].ident));
    }
    while (--number_of_events) {
    };
    return ret;
};
