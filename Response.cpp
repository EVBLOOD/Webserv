
#include "Response.hpp"
#include <sys/syslimits.h>
#include <iostream>
#include <string>
#include "Request.hpp"
#include "tools.hpp"

HttpResponse HttpResponse::generate_indexing(std::string dir,
                                             std::string location) {
    std::vector<std::string> body;
    std::vector<std::string> files = tools::list_files_in_dir(dir);

    for (size_t i = 0; i < files.size(); ++i) {
        body.push_back("<a href='" +
                       tools::url_path_correction(location, files[i]) +
                       "'><h1>" + files[i] + "</h1></a>");
    }
    return HttpResponse(200, "1.1", "OK")
        .add_to_body(body)
        .add_content_type(".html");
}

HttpResponse::HttpResponse(int status, std::string version, std::string action)
    : _status(status),
      _content_length(0),
      _version(version),
      _action(action),
      _headers(),
      _body(){};

HttpResponse::HttpResponse(int status,
                           std::string version,
                           std::string action,
                           HttpRequest request)
    : _status(status),
      _content_length(0),
      _version(version),
      _action(action),
      _headers(),
      _body() {
    if (request.getHeaderValue("cookie") != "")
        add_header("Set-Cookie: ", request.getHeaderValue("cookie"));
}

HttpResponse& HttpResponse::add_header(std::string key, std::string value) {
    _headers.insert(make_pair(key, value));
    return *this;
};

std::string HttpResponse::getHeaderValue(std::string key) {
    multi_iter it = _headers.find(key);
    if (it == _headers.end())
        return "";
    return it->second;
};

HttpResponse& HttpResponse::add_to_body(std::string line) {
    _body.push_back(line);
    _content_length += line.length();
    return *this;
};

HttpResponse& HttpResponse::add_to_body(std::vector<std::string> body) {
    std::vector<std::string>::iterator iter = body.begin();
    while (iter != body.end()) {
        add_to_body(*iter);
        ++iter;
    }
    return *this;
};

size_t HttpResponse::get_body_size() {
    return _content_length;
};

std::string HttpResponse::build() {
    std::string res;

    res += "HTTP/" + _version + " " + std::to_string(_status) + " " + _action +
           "\r\n";

    _headers.insert(make_pair("Content-Length",
                              std::to_string(_content_length + _body.size())));

    _headers.insert(make_pair("Date", tools::date_http(time(NULL))));
    _headers.insert(std::make_pair("Server", "1337_webserv"));
    {
        multi_iter iter = _headers.begin();
        while (iter != _headers.end()) {
            res += iter->first + ": " + iter->second + "\r\n";
            iter++;
        }
    }
    res += "\r\n";
    {
        std::vector<std::string>::iterator iter = _body.begin();
        while (iter != _body.end()) {
            res += *iter + '\n';
            iter++;
        }
    }
    res += "\r\n";
    return res;
}

void HttpResponse::dump() {
    std::cout << "HTTP/" + _version + " " + std::to_string(_status) + " " +
                     _action + "\n";
    for (multi_iter iter = _headers.begin(); iter != _headers.end(); ++iter) {
        std::cout << "[" << iter->first << "]"
                  << " : [" << iter->second << "]" << '\n';
    }
}

std::string HttpResponse::get_content_type(std::string file_name) {
    std::string content_type = "text/plain";
    if (file_name.rfind('.') != std::string::npos) {
        std::string ext =
            file_name.substr(file_name.rfind('.'), file_name.size());
        if (ext == ".html") {
            content_type = "text/html";
        } else if (ext == ".js") {
            content_type = "application/javascript";
        } else if (ext == ".css") {
            content_type = "text/css";
        } else if (ext == ".ico") {
            content_type = "image/x-icon";
        } else if (ext == ".jpeg" || ext == ".jpg") {
            content_type = "image/jpeg";
        } else if (ext == ".png") {
            content_type = "image/png";
        } else if (ext == ".mp4") {
            content_type = "video/mp4";
        } else if (ext == ".gif") {
            content_type = "image/gif";
        } else if (ext == ".tiff") {
            content_type = "image/tiff";
        } else if (ext == ".svg") {
            content_type = "image/svg+xml";
        } else if (ext == ".csv") {
            content_type = "text/csv";
        } else if (ext == ".xml") {
            content_type = "application/xml";
        } else if (ext == ".zip") {
            content_type = "application/zip";
        } else if (ext == ".mp3") {
            content_type = "audio/mpeg";
        } else if (ext == ".wma") {
            content_type = "audio/x-ms-wma";
        } else if (ext == ".ra") {
            content_type = "audio/vnd.rn-realaudio";
        } else if (ext == ".wav") {
            content_type = "audio/x-wav";
        } else if (ext == ".mpeg") {
            content_type = "video/mpeg";
        } else if (ext == ".qt") {
            content_type = "video/quicktime";
        } else if (ext == ".wmv") {
            content_type = "video/x-ms-wmv";
        } else if (ext == ".avi") {
            content_type = "video/x-msvideo";
        } else if (ext == ".flv") {
            content_type = "video/x-flv";
        } else if (ext == ".webm") {
            content_type = "video/webm";
        } else if (ext == ".odt") {
            content_type = "application/vnd.oasis.opendocument.text";
        } else if (ext == ".ods") {
            content_type = "application/vnd.oasis.opendocument.spreadsheet";
        } else if (ext == ".odp") {
            content_type = "application/vnd.oasis.opendocument.presentation";
        } else if (ext == ".odg") {
            content_type = "application/vnd.oasis.opendocument.graphics";
        } else if (ext == ".ods") {
            content_type = "application/vnd.oasis.opendocument.spreadsheet";
        } else if (ext == ".odt") {
            content_type = "application/vnd.oasis.opendocument.text";
        } else if (ext == ".ppt") {
            content_type = "application/vnd.ms-powerpoint";
        } else if (ext == ".pptx") {
            content_type =
                "application/"
                "vnd.openxmlformats-officedocument.presentationml.presentation";
        } else if (ext == ".xls") {
            content_type = "application/vnd.ms-excel";
        } else if (ext == ".xlsx") {
            content_type =
                "application/"
                "vnd.openxmlformats-officedocument.spreadsheetml.sheet";
        } else if (ext == ".doc") {
            content_type = "application/msword";
        } else if (ext == ".docx") {
            content_type =
                "application/"
                "vnd.openxmlformats-officedocument.wordprocessingml.document";
        } else if (ext == ".xul") {
            content_type = "application/vnd.mozilla.xul+xml";
        }
    }

    return content_type;
};

HttpResponse HttpResponse::error_response(int status, std::string path) {
    std::string action;
    if (status == 400) {
        action = "Bad Request";
    } else if (status == 401) {
        action = "Unauthorized";
    } else if (status == 402) {
        action = "Payment Required";
    } else if (status == 404) {
        action = "Not Found";
    } else if (status == 405) {
        action = "Method Not Allowed";
    } else if (status == 403) {
        action = "Forbidden";
    } else if (status == 406) {
        action = "Not Acceptable";
    } else if (status == 407) {
        action = "Proxy Authentication Required";
    } else if (status == 408) {
        action = "Request Timeout";
    } else if (status == 409) {
        action = "Conflict";
    } else if (status == 410) {
        action = "Gone";
    } else if (status == 411) {
        action = "Length Required";
    } else if (status == 412) {
        action = "Precondition Failed";
    } else if (status == 413) {
        action = "Payload Too Large";
    } else if (status == 414) {
        action = "URI Too Long";
    } else if (status == 415) {
        action = "Unsupported Media Type";
    } else if (status == 416) {
        action = "Range Not Satisfiable";
    } else if (status == 417) {
        action = "Expectation Failed";
    } else if (status == 422) {
        action = "Unprocessable Entity";
    } else if (status == 423) {
        action = "Locked";
    } else if (status == 424) {
        action = "Failed Dependency";
    } else if (status == 425) {
        action = "Too Early";
    } else if (status == 426) {
        action = "Upgrade Required";
    } else if (status == 428) {
        action = "Precondition Required";
    } else if (status == 429) {
        action = "Too Many Requests";
    } else if (status == 431) {
        action = "Request Header Fields Too Large";
    } else if (status == 451) {
        action = "Unavailable For Legal Reasons";
    } else if (status == 500) {
        action = "Internal Server Error";
    } else if (status == 501) {
        action = "Not Implemented";
    } else if (status == 502) {
        action = "Bad Gateway";
    } else if (status == 503) {
        action = "Service Unavailable";
    } else if (status == 504) {
        action = "Gateway Timeout";
    } else if (status == 505) {
        action = "HTTP Version Not Supported";
    } else if (status == 506) {
        action = "Variant Also Negotiates";
    } else if (status == 507) {
        action = "Insufficient Storage";
    } else if (status == 508) {
        action = "Loop Detected";
    } else if (status == 510) {
        action = "Not Extended";
    } else if (status == 511) {
        action = "Network Authentication Required";
    } else {
        status = 404;
        action = "Not Found";
    }
    if (!path.empty() && is_file(path) && is_file_exists(path) &&
        is_file_readable(path)) {
        std::ifstream file(path);
        if (!file.is_open() || file.fail()) {
            return HttpResponse::error_response(500, "");
        }
        return HttpResponse(status, "1.1", action)
            .add_to_body(tools::open_to_serve(file))
            .add_content_type(path);
    }

    return HttpResponse(status, "1.1", action)
        .add_to_body(generateErrorPage(status, action))
        .add_content_type(".html");
};

std::string HttpResponse::generateErrorPage(int statusCode,
                                            const std::string& statusMessage) {
    std::stringstream pageStream;
    pageStream << "<html><head><title>" << statusCode << " " << statusMessage
               << "</title></head><body><h1>" << statusCode << " "
               << statusMessage << "</h1></body></html>";
    return pageStream.str();
}

HttpResponse& HttpResponse::add_content_type(std::string path) {
    return add_header("Content-Type", get_content_type(path));
};

HttpResponse HttpResponse::send_file(std::string path,
                                     std::string root,
                                     std::map<int, std::string> error_pages) {
    std::string full_path = tools::url_path_correction(root, path);

    if (!tools::is_dir(full_path) && !tools::is_file(full_path)) {
        return error_response(404, root + error_pages[404]);
    }

    if (!tools::is_part_of_root(root, path) ||
        (tools::is_file_readable(full_path) == false)) {
        return error_response(403, root + error_pages[403]);
    }

    std::ifstream file(full_path);
    if (!file.is_open() || file.fail()) {
        return HttpResponse::error_response(500, error_pages[500]);
    }
    return HttpResponse(200, "1.1", "OK")
        .add_to_body(tools::open_to_serve(file))
        .add_content_type(full_path);
}

HttpResponse HttpResponse::redirect_moved_response(
    std::string const& location) {
    return HttpResponse(301, "1.1", "Moved Permanently")
        .add_header("Location", location);
};
HttpResponse HttpResponse::redirect_found_response(
    std::string const& location) {
    return HttpResponse(302, "1.1", "Found").add_header("Location", location);
};

HttpResponse HttpResponse::index_response(
    std::vector<std::string> index,
    std::string root,
    std::map<int, std::string> error_pages) {
    std::vector<std::string>::iterator it = index.begin();

    while (it != index.end()) {
        std::string path = tools::url_path_correction(root, *it);
        if (tools::is_file_exists(path) && tools::is_file_readable(path))
            break;
        it++;
    }
    if (it == index.end())
        return error_response(404, error_pages[404]);
    return HttpResponse::send_file(*it, root, error_pages);
}
