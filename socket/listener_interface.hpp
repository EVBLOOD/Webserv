#pragma once
typedef struct kevent Kevent;
class IListener {
   public:
    virtual int get_raw_fd() const = 0;
};
