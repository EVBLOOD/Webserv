

#pragma once
#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace tools {

enum {
    DEBUG,
    INFO,
    ERROR,
    TODO,
};

std::string G(int level);

std::vector<std::string> list_files_in_dir(std::string const& path);

std::string url_path_correction(std::string const& a, std::string const& b);

bool is_part_of_root(std::string const& root, std::string const& location);

bool is_dir(std::string const& path);

bool is_file(std::string const& path);

std::string trim(std::string const& str, std::string const& del);

std::vector<std::string> split(std::string const& str, std::string const& del);

std::string open_to_serve(std::ifstream& file);

bool is_file_readable(const std::string& path);

bool is_file_exists(const std::string& path);
std::string dealwithchuncked_buff(std::string const& primary,
                                  ssize_t& limit,
                                  bool x = false);
std::vector<std::string> split_(std::string const& request,
                                std::string const& lims);
std::string get_expires_time_in_sec(int seconds);
std::string get_expires_time_in_hour(int hours);
std::string toUppercase(const std::string& s);
std::string date_http(time_t timestamp);
std::string generateRandomTempFileName();
};  // namespace tools
