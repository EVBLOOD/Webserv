#include "HttpResponse.h"
#include "Request.hpp"
#include <assert.h>
#include <iostream>


char resp[] =
        "HTTP/1.1 200 OK\r\n"
        "Content-type: text/html\r\n"
        "Server: webserver-c\r\n\r\n"
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
        HttpResponse test(200, "1.1", "OK");
        test.add_header("Server", "webserver-c")
                .add_header("Content-type", "text/html")
                .add_to_body("<h1>hello, world</h1>")
                .add_to_body("<ul><li>13</li>")
                .add_to_body("<li>37</li></ul>")
                .build();
        assert(test.build() == resp);
        std::cout << "RESPONSE\n";
        test.dump();
        std::cout << "END RESPONSE\n";
    }
    {
        HttpRequest request(req);
        std::cout << "REQUEST\n";
        request.dump();
        std::cout << "END REQUEST\n";
    }
}