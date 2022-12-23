//
// Created by Oussama Rahmouni on 11/23/22.
//

#pragma once

#include <assert.h>
#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace tools {

enum {
    DEBUG,
    INFO,
    ERROR,
    TODO,
};

std::string G(int level);

std::vector<std::string> list_files_in_dir(std::string path);

std::string url_path_correction(std::string a, std::string b);

bool is_part_of_root(std::string root, std::string location);

bool is_dir(std::string path);

bool is_file(std::string path);

std::string trim(std::string str, std::string del);

std::vector<std::string> split(std::string str, std::string del);

std::vector<std::string> open_to_serve(std::ifstream& file);

bool is_file_readable(const std::string& path);

bool is_file_exists(const std::string& path);
std::string dealwithchuncked_buff(std::string primary,
                                  ssize_t& limit,
                                  bool x = false);
std::vector<std::string> split_(std::string request, std::string lims);
};  // namespace tools
