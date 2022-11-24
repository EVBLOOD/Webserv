cmake_minimum_required(VERSION 3.23)
project(Webserver)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Webserver
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
        tools.cpp
        tools.hpp
        socket_header.hpp HttpResponse.cpp HttpResponse.hpp
        parsing/location.cpp
        parsing/location.hpp
        parsing/parser.cpp
        parsing/parser.hpp
        parsing/tokengen.cpp
        parsing/tokengen.hpp
        parsing/server.cpp
        parsing/server.hpp
        )
