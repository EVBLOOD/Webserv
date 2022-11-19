#include "Route.hpp"

class RouteBuilder : protected Route {
   public:
    RouteBuilder();
    RouteBuilder& set_location(std::string location);
    RouteBuilder& set_methodes(bool get, bool put, bool post, bool del);
    Route build();
};
