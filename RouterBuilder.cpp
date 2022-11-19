#include "RouterBuilder.hpp"

RouteBuilder::RouteBuilder() : Route(){};

RouteBuilder& RouteBuilder::set_location(std::string location) {
    _location = location;
    return *this;
}

RouteBuilder& RouteBuilder::set_methodes(bool get,
                                         bool put,
                                         bool post,
                                         bool del) {
    _get = get;
    _put = put;
    _delete = del;
    _post = post;
    return *this;
}

Route RouteBuilder::build() {
    return *this;
}
