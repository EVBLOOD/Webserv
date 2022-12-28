#pragma once
typedef struct kevent Kevent;
class IListener {
   public:
    virtual int get_raw_fd() const = 0;
    virtual Kevent get_kevent() const = 0;
    virtual void set_kevent(Kevent event) = 0;
};
