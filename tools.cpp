#include "tools.hpp"
#include <sys/_types/_ssize_t.h>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <ostream>
#include <sstream>
#include <string>

std::vector<std::string> tools::split_(std::string request, std::string lims) {
    std::vector<std::string> end;
    size_t x = request.find(lims);
    if (x == std::string::npos)
        return (end.push_back(request), end);
    end.push_back(request.substr(0, x));
    end.push_back(request.substr(x + lims.length(), request.length() - x));
    return end;
}

std::string tools::G(int level) {
    switch (level) {
        case DEBUG:
            return "\033[1;32m[DEBUG]\033[0m";
        case INFO:
            return "\033[1;36m[INFO]\033[0m";
        case ERROR:
            return "\033[1;31m[ERROR]\033[0m";
        case TODO:
            return "\033[1;34m[ERROR]\033[0m";
    }
    return "\033[1;32m[DEBUG]\033[0m";
}

std::vector<std::string> tools::list_files_in_dir(std::string path) {
    DIR* dir = opendir(path.c_str());

    if (dir == NULL) {
        return std::vector<std::string>();
    }

    std::vector<std::string> files;
    struct dirent* file;
    while ((file = readdir(dir))) {
        if (file->d_name[0] == '.')
            continue;
        files.push_back(file->d_name);
    }
    closedir(dir);
    return (files);
}

std::string tools::url_path_correction(std::string a, std::string b) {
    std::string::iterator it = a.begin();
    std::string::iterator ite = a.end();
    std::string result = std::string();
    while (it != ite) {
        if (result.empty() || result.back() != '/' || result.back() != *it)
            result += *it;
        it++;
        ;
    }
    if (result.size() && result.back() != '/')
        result += '/';
    ite = b.end();
    it = b.begin();
    while (it != ite) {
        if (result.empty() || result.back() != '/' || result.back() != *it)
            result += *it;
        it++;
    }
    return result;
}

bool tools::is_part_of_root(std::string root, std::string location) {
    char actualpath[PATH_MAX + 1];
    std::cout << "[SAADX] " << tools::url_path_correction(root, location)
              << '\n';
    char* ptr =
        realpath(url_path_correction(root, location).c_str(), actualpath);
    if (ptr == NULL) {
        return false;
    }
    std::string actual_path(actualpath);
    if (actual_path.length() < root.length()) {
        return false;
    }
    if (actual_path.substr(0, root.size()) != root) {
        return false;
    }
    return true;
}

bool tools::is_dir(std::string path) {
    struct stat path_stat;
    stat(path.c_str(), &path_stat);
    return S_ISDIR(path_stat.st_mode);
}

bool tools::is_file(std::string path) {
    struct stat path_stat;
    stat(path.c_str(), &path_stat);
    return S_ISREG(path_stat.st_mode);
}

// std::vector<std::string> tools::split(std::string str, std::string del) {
//     std::vector<std::string> res;
//     size_t pos = 0;
//     while ((pos = str.find(del)) != std::string::npos) {
//         res.push_back(str.substr(0, pos));
//         str = str.substr(pos + del.size(), str.size());
//     }
//     if (str.empty())
//         return res;
//     res.push_back(str);
//     return res;
// }

std::vector<std::string> tools::split(std::string s, std::string delimiter) {
    std::vector<std::string> parts;
    std::string::size_type start = 0;
    std::string::size_type end = 0;
    while ((end = s.find(delimiter, start)) != std::string::npos) {
        parts.push_back(s.substr(start, end - start));
        start = end + delimiter.length();
    }
    parts.push_back(s.substr(start));
    return parts;
}

std::string tools::trim(std::string s, std::string delimiters) {
    std::string::size_type first = s.find_first_not_of(delimiters);
    if (first == std::string::npos) {
        return "";
    }
    std::string::size_type last = s.find_last_not_of(delimiters);
    return s.substr(first, last - first + 1);
}

// std::string tools::trim(std::string str, std::string del) {
//     size_t i = 0;
//     while (i < str.length() - 1 && (del.find(str[i]) != std::string::npos)) {
//         ++i;
//     }
//     size_t j = str.length() - 1;
//     while (j && i != j && (del.find(str[j]) != std::string::npos)) {
//         --j;
//     }
//     if (i == j)
//         return "";
//     if (j == str.length() - 1)
//         return str.substr(i, j + 1);
//     str = str.substr(i, j);
//     return str;
// }

std::string tools::open_to_serve(std::ifstream& file) {
    if (!file.is_open()) {
        return "";
    }

    // Read the file into a stringstream
    std::stringstream stream;
    stream << file.rdbuf();

    // Close the file
    file.close();

    // Return the contents of the stringstream as a string
    return stream.str();
};

bool tools::is_file_readable(const std::string& path) {
    return access(path.c_str(), R_OK) != -1;
}
bool tools::is_file_exists(const std::string& path) {
    return access(path.c_str(), F_OK) != -1;
}

std::string tools::dealwithchuncked_buff(std::string primary,
                                         ssize_t& limit,
                                         bool x) {
    std::string ret;
    std::string number = "";
    ssize_t header_end = 0;
    ssize_t hexdel = 0;

    if (limit == 0) {
        if (x) {
            header_end = primary.find("\r\n\r\n");
            if (static_cast<size_t>(header_end) == std::string::npos)
                return "";
            header_end += 4;
            ret = primary.substr(0, header_end);
            if (ret == primary)
                return ret;
        } else {
            if (primary.find("\r\n", 0) == 0)
                header_end = 2;
        }
        hexdel = primary.find("\r\n", header_end);
        if (static_cast<size_t>(hexdel) == std::string::npos)
            return "";
        number = primary.substr(header_end, hexdel - header_end);
        std::stringstream StringStream;
        StringStream << std::hex << number;
        StringStream >> limit;
        if (limit == 0)
            return ret;
        hexdel += 2;
    }
    ssize_t size = static_cast<ssize_t>(primary.length()) - (hexdel + 2) -
                   limit;  // left_from_primary - all_well_be_removed
    ssize_t size_ = static_cast<ssize_t>(primary.length()) - hexdel -
                    limit;  // left_from_primary - all_well_be_removed
    if (size >= 0) {
        ret += primary.substr(hexdel, limit);
        if (size == 0) {
            limit = 0;
            return ret;
        }
        ssize_t tmp = 0;
        ret +=
            dealwithchuncked_buff(primary.substr(hexdel + limit, size_), tmp);
        limit = tmp;
    } else {
        ret += primary.substr(hexdel, primary.length() - hexdel);
        limit = limit - (primary.length() - hexdel);
    }
    return ret;
}

std::string tools::get_expires_time_in_sec(int seconds) {
    time_t current_time = time(NULL);
    current_time += seconds;
    tm* time_info = gmtime(&current_time);
    char expires_string[64];
    strftime(expires_string, sizeof(expires_string), "%a, %d %b %Y %H:%M:%S %Z",
             time_info);
    return expires_string;
}

std::string tools::get_expires_time_in_hour(int hours) {
    time_t current_time = time(nullptr);
    current_time += hours * 60 * 60;  // Convert hours to seconds
    tm* time_info = gmtime(&current_time);
    char expires_string[64];
    strftime(expires_string, sizeof(expires_string), "%a, %d %b %Y %H:%M:%S %Z",
             time_info);
    return expires_string;
}

std::string tools::toUppercase(const std::string& s) {
    std::string result = s;
    std::transform(result.begin(), result.end(), result.begin(), toupper);
    return result;
}

std::string tools::date_http(time_t timestamp) {
    char buffer[32];
    std::strftime(buffer, 32, "%Y-%m-%d %H:%M:%S", std::localtime(&timestamp));
    return buffer;
}

std::string tools::generateRandomTempFileName() {
    srand(time(NULL));
    return "/tmp/.web_server_" + std::to_string(rand());
}
