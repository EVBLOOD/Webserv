#pragma once
#include "socket_header.hpp"
class Route {
   protected:
    std::string _location;
    bool _get;
    bool _put;
    bool _post;
    bool _delete;
    Route();

   public:
    std::string get_location();
    bool is_allowed_get();
    bool is_allowed_put();
    bool is_allowed_post();
    bool is_allowed_delete();
};
