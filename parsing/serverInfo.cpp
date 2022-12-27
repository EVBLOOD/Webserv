

#include "serverInfo.hpp"

void serverInfo::setlocation(std::string x, Location y) {
    locations.insert(std::make_pair(x, y));
}

template <>
void serverInfo::setters<setport>(std::list<tokengen>::iterator& big,
                                  std::list<tokengen>::iterator& end) {
    std::string tmp;

    CURLWAIT(big, end, true);
    if (big == end || (big->type != WORD && big->type != QUOTES)) {
        std::cerr << "[ERROR] parsing error\n";
        exit(1);
    }
    if (big->type == QUOTES)
        tmp = big->content.substr(1, big->content.length() - 2);
    else
        tmp = big->content;
    for (int i = 0; tmp[i]; i++)
        if (isdigit(tmp[i]) == false) {
            std::cerr << "[ERROR] parsing error\n";
            exit(1);
        }
    port.push_back(tmp);
    big++;
    CURLWAIT(big, end, true);
    if (big == end || big->type != SEMICOLONS) {
        std::cerr << "[ERROR] parsing error\n";
        exit(1);
    }
    big++;
}

template <>
void serverInfo::setters<setservername>(std::list<tokengen>::iterator& big,
                                        std::list<tokengen>::iterator& end) {
    std::string tmp;

    CURLWAIT(big, end, true);
    if (big == end || (big->type != WORD && big->type != QUOTES)) {
        std::cerr << "[ERROR] parsing error\n";
        exit(1);
    }
    if (big->type == QUOTES)
        tmp = big->content.substr(1, big->content.length() - 2);
    else
        tmp = big->content;
    server_name.push_back(tmp);
    big++;
    CURLWAIT(big, end, true);
    if (big != end && big->type != SEMICOLONS)
        setters<setservername>(big, end);
    if (big == end) {
        std::cerr << "[ERROR] parsing error\n";
        exit(1);
    }
    big++;
}

template <>
void serverInfo::setters<setmap>(std::list<tokengen>::iterator& big,
                                 std::list<tokengen>::iterator& end) {
    std::string tmp;
    int status;
    std::stringstream x;

    CURLWAIT(big, end, true);
    if (big == end || (big->type != WORD && big->type != QUOTES)) {
        std::cerr << "[ERROR] parsing error\n";
        exit(1);
    }
    if (big->type == QUOTES)
        tmp = big->content.substr(1, big->content.length() - 2);
    else
        tmp = big->content;
    for (int i = 0; tmp[i]; i++)
        if (isdigit(tmp[i]) == false) {
            std::cerr << "[ERROR] parsing error\n";
            exit(1);
        }
    x << tmp;
    x >> status;
    big++;
    CURLWAIT(big, end, true);
    if (big == end || (big->type != WORD && big->type != QUOTES)) {
        std::cerr << "[ERROR] parsing error\n";
        exit(1);
    }
    if (big->type == QUOTES)
        tmp = big->content.substr(1, big->content.length() - 2);
    else
        tmp = big->content;
    error_page.insert(std::make_pair(status, tmp));
    big++;
    CURLWAIT(big, end, true);
    if (big == end || big->type != SEMICOLONS) {
        std::cerr << "[ERROR] parsing error\n";
        exit(1);
    }
    big++;
}

template <>
void serverInfo::setters<setroot>(std::list<tokengen>::iterator& big,
                                  std::list<tokengen>::iterator& end) {
    std::string tmp;

    CURLWAIT(big, end, true);
    if (big == end || (big->type != WORD && big->type != QUOTES)) {
        std::cerr << "[ERROR] parsing error\n";
        exit(1);
    }
    if (big->type == QUOTES)
        tmp = big->content.substr(1, big->content.length() - 2);
    else
        tmp = big->content;
    root = tmp;
    big++;
    CURLWAIT(big, end, true);
    if (big == end || big->type != SEMICOLONS) {
        std::cerr << "[ERROR] parsing error\n";
        exit(1);
    }
    big++;
}

template <>
void serverInfo::setters<setclient_max_body_size>(
    std::list<tokengen>::iterator& big,
    std::list<tokengen>::iterator& end) {
    std::string tmp;
    unsigned long long size;
    std::stringstream x;

    CURLWAIT(big, end, true);
    if (big == end || (big->type != WORD && big->type != QUOTES)) {
        std::cerr << "[ERROR] parsing error\n";
        exit(1);
    }
    if (big->type == QUOTES)
        tmp = big->content.substr(1, big->content.length() - 2);
    else
        tmp = big->content;
    for (int i = 0; tmp[i]; i++)
        if (isdigit(tmp[i]) == false) {
            std::cerr << "[ERROR] parsing error\n";
            exit(1);
        }
    x << tmp;
    x >> size;
    client_max_body_size = size;
    big++;
    CURLWAIT(big, end, true);
    if (big != end && big->type != SEMICOLONS) {
        std::cerr << "[ERROR] parsing error\n";
        exit(1);
    }
    big++;
}

template <>
void serverInfo::setters<sethost>(std::list<tokengen>::iterator& big,
                                  std::list<tokengen>::iterator& end) {
    std::string tmp;

    CURLWAIT(big, end, true);
    if (big == end || (big->type != WORD && big->type != QUOTES)) {
        std::cerr << "[ERROR] parsing error\n";
        exit(1);
    }
    if (big->type == QUOTES)
        tmp = big->content.substr(1, big->content.length() - 2);
    else
        tmp = big->content;
    host = std::string(tmp);
    big++;
    CURLWAIT(big, end, true);
    if (big == end || big->type != SEMICOLONS) {
        std::cerr << "[ERROR] parsing error\n";
        exit(1);
    }
    big++;
}

serverInfo::serverInfo()
    : index(),
      host(),
      port(),
      server_name(),
      error_page(),
      root(),
      locations(),
      client_max_body_size(1024) {}

serverInfo::~serverInfo() {}

typedef void (serverInfo::*function_server)(std::list<tokengen>::iterator& big,
                                            std::list<tokengen>::iterator& end);

void serverInfo::execute(int i,
                         std::list<tokengen>::iterator& big,
                         std::list<tokengen>::iterator& end) {
    function_server funs[] = {
        &serverInfo::setters<0>, &serverInfo::setters<1>,
        &serverInfo::setters<2>, &serverInfo::setters<3>,
        &serverInfo::setters<4>, &serverInfo::setters<sethost>};
    (this->*funs[i])(big, end);
}

serverInfo::serverInfo(const serverInfo& sv) {
    *this = sv;
}

serverInfo& serverInfo::operator=(const serverInfo& sv) {
    if (this == &sv) {
        std::cerr << "[ERROR] parsing error\n";
        exit(1);
    }
    host = sv.host;
    port = sv.port;
    server_name = sv.server_name;
    error_page = sv.error_page;
    root = sv.root;
    locations = sv.locations;
    client_max_body_size = sv.client_max_body_size;
    return *this;
}
