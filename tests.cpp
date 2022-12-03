#include <assert.h>
#include <iostream>
#include "Request.hpp"
#include "Response.hpp"

char resp[] =
    "HTTP/1.1 200 OK\r\n"
    "Content-Length: 58\r\n"
    "Content-type: text/html\r\n"
    "Server: webserver-c\r\n"
    "\r\n"
    "<h1>hello, world</h1>\r\n"
    "<ul><li>13</li>\r\n"
    "<li>37</li></ul>\r\n";

char req[] =
    "POST /cgi-bin/process.cgi HTTP/1.1\r\n"
    "User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\n"
    "Host: www.google.com\r\n"
    "Content-Type: text/xml; charset=utf-8\r\n"
    "Content-Length: length\r\n"
    "Accept-Language: en-us\r\n"
    "Accept-Encoding: gzip, deflate\r\n"
    "Connection: Keep-Alive\r\n"
    "\r\n"
    "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n"
    "<string xmlns=\"http://clearforest.com/\">string</string>\r\n";

int main() {
    {
        std::string body =
            "<h1>hello, world</h1>\r\n"
            "<ul><li>13</li>\r\n"
            "<li>37</li></ul>\r\n";
        HttpResponse test(200, "1.1", "OK");
        test.add_header("Server", "webserver-c")
            .add_header("Content-type", "text/html")
            .add_to_body("<h1>hello, world</h1>")
            .add_to_body("<ul><li>13</li>")
            .add_to_body("<li>37</li></ul>")
            .build();
        assert(test.build() == resp);
        assert(test.get_body_size() == body.length());
    }
    { HttpRequest request(req); }
    {
        assert(is_dir("."));
        assert(!is_file("."));
        assert(!is_dir("main.cpp"));
        assert(is_file("main.cpp"));
        assert(!is_dir("error/error"));
        assert(!is_file("error/error"));
    }
    {
        assert(url_path_correction("root", "next") == "root/next");
        assert(url_path_correction("root/", "next") == "root/next");
        assert(url_path_correction("root/", "/next") == "root/next");
        assert(url_path_correction("root/", "/next/") == "root/next/");
        assert(url_path_correction("/root/", "/next/") == "/root/next/");
        assert(url_path_correction("/root/", "/next") == "/root/next");

        assert(url_path_correction("root", "previous/next") ==
               "root/previous/next");
        assert(url_path_correction("root/", "/////next") == "root/next");
        assert(url_path_correction("root/", "/next/////") == "root/next/");
        assert(url_path_correction("//////root/", "/next/////") ==
               "/root/next/");
        assert(url_path_correction("//////root/hello", "/next/////world") ==
               "/root/hello/next/world");
    }
    {
        std::cout << "[TODO] test is_part_of_root\n";
        assert(false);
    }
    std::cout << "Response test 👍" << '\n';
    std::cout << "is_dir && is_file test 👍" << '\n';
    std::cout << "url_path_correction test 👍" << '\n';
    std::cout << "Request test 👍" << '\n';
}
