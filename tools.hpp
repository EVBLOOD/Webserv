//
// Created by Oussama Rahmouni on 11/23/22.
//

#pragma once

#include <stdio.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace tools {
std::string trim(std::string str, std::string del);

std::vector<std::string> split(std::string str, std::string del);

std::vector<std::string> open_to_serve(std::ifstream& file);

bool is_file_readable(const std::string& path);

bool is_file_exists(const std::string& path);

};  // namespace tools
