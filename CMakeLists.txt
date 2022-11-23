cmake_minimum_required(VERSION 3.23)
project(Webserv)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Webserv
        main.cpp
        Request.cpp
        Request.hpp
        Route.cpp
        Route.hpp
        RouterBuilder.cpp
        RouterBuilder.hpp
        Server.cpp
        Server.hpp
        ServerBuilder.cpp
        ServerBuilder.hpp
        ServerPoll.cpp
        ServerPoll.hpp
        socket_header.hpp HttpResponse.cpp HttpResponse.h)
