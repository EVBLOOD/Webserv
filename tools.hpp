//
// Created by Oussama Rahmouni on 11/23/22.
//

#pragma once

#include <string>
#include <vector>

namespace tools {
    std::string trim(std::string str, std::string del);

    std::vector<std::string> split(std::string str, std::string del);
};