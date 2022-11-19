#include "Route.hpp"

Route::Route()
    : _location("\\"), _get(false), _put(false), _post(false), _delete(false){};

std::string Route::get_location() {
    return _location;
}
bool Route::is_allowed_get() {
    return _get;
}
bool Route::is_allowed_put() {
    return _put;
}
bool Route::is_allowed_post() {
    return _post;
}
bool Route::is_allowed_delete() {
    return _delete;
}
