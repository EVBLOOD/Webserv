#include "Request.hpp"

std::vector<std::string> HttpRequest::split(std::string str, std::string del) {
    std::vector<std::string> res;
    size_t pos = 0;
    while ((pos = str.find(del)) != std::string::npos) {
        res.push_back(str.substr(0, pos));
        str = str.substr(pos + del.size(), str.size());
    }
    if (str.empty())
        return res;
    res.push_back(str);
    return res;
}

std::string HttpRequest::trim(std::string str, std::string del) {
    size_t i = 0;
    while (i < str.length() && (del.find(str[i]) != std::string::npos)) {
        ++i;
    }
    size_t j = str.length() - 1;
    while (j && (del.find(str[i]) != std::string::npos)) {
        --j;
    }
    str = str.substr(i, j + 1);
    return str;
}

char resp[] =
    "HTTP/1.0 200 OK\r\n"
    "Server: webserver-c\r\n"
    "Content-type: text/html\r\n\r\n"
    "<h1>hello, world</1>\r\n"
    "<ul><li>13</li>\r\n"
    "<li>37</li></ul>\r\n";

HttpRequest::HttpRequest(std::string request) : body() {
    raw = request;
    std::vector<std::string> splited = split(request, "\n");

    std::vector<std::string> first_line = split(splited[0], " ");
    method = first_line[0];
    location = first_line[1];
    version = first_line[2];
    splited.erase(splited.begin());
    size_t i = 0;
    for (i = 0; i < splited.size() && splited[i] != "\r"; ++i) {
        std::vector<std::string> tmp = split(splited[i], ":");
        std::string key = trim(tmp[0], "\n\r ");
        std::string value = trim(tmp[1], "\n\r ");
        headers[key] = value;
    }

    for (size_t j = i; j < splited.size(); ++j) {
        std::string trimed = trim(splited[j], "\n\r");
        if (!trimed.empty())
            body.content.push_back(trimed);
    }
    body.content_type = headers["Content-type"];
}

void HttpRequest::dump() {
    std::cout << method << " " << location << " " << version << "\n";
    for (std::map<std::string, std::string>::iterator iter = headers.begin();
         iter != headers.end(); ++iter) {
        std::cout << iter->first << " : " << iter->second << '\n';
    }
    for (std::vector<std::string>::iterator iter = body.content.begin();
         iter != body.content.end(); ++iter) {
        std::cout << *iter << "\n";
    }
}

// int main() {
//     HttpRequest(
//         "HTTP/1.0 200 OK\r\n"
//         "Server: webserver-c\r\n"
//         "Content-type: text/html\r\n\r\n"
//         "<h1>hello, world</1>\r\n"
//         "<ul><li>13</li>\r\n"
//         "<li>37</li></ul>\r\n")
//         .dump();
// }
