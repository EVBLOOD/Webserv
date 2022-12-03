#include "tools.hpp"

bool tools::is_part_of_root(std::string root, std::string location) {
    assert(root.size() < PATH_MAX && location.size() < PATH_MAX);
    char actualpath[PATH_MAX + 1];
    char* ptr = realpath(location.c_str(), actualpath);
    if (ptr == NULL) {
        std::cerr << "actualpath function failed\n";
        assert(false);
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

std::vector<std::string> tools::split(std::string str, std::string del) {
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

std::string tools::trim(std::string str, std::string del) {
    size_t i = 0;
    while (i < str.length() - 1 && (del.find(str[i]) != std::string::npos)) {
        ++i;
    }
    size_t j = str.length() - 1;
    while (j && i != j && (del.find(str[j]) != std::string::npos)) {
        --j;
    }
    if (i == j)
        return "";
    if (j == str.length() - 1)
        return str.substr(i, j + 1);
    str = str.substr(i, j);
    return str;
}

std::vector<std::string> tools::open_to_serve(std::ifstream& file) {
    std::string line;
    std::vector<std::string> res;
    while (std::getline(file, line)) {
        res.push_back(line);
    }
    return res;
};

bool tools::is_file_readable(const std::string& path) {
    errno = 0;
    return access(path.c_str(), R_OK) != -1;
}
bool tools::is_file_exists(const std::string& path) {
    errno = 0;
    return access(path.c_str(), F_OK) != -1;
}
