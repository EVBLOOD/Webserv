#pragma once
class IListener {
   public:
    virtual int get_raw_fd() const = 0;
};
