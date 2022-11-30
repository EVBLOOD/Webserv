/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakllam <sakllam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 22:12:22 by sakllam           #+#    #+#             */
/*   Updated: 2022/11/15 18:43:13 by sakllam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <list>
#include <map>
#include <string>
#include <vector>
#include "location.hpp"
#include "tokengen.hpp"

class serverInfo {
   public:
    std::vector<std::string> index;
    std::string host;             // done
    std::vector<in_port_t> port;  // done
    std::vector<std::string> server_name;
    std::map<int, std::string> error_page;      // done
    std::string root;                           // done
    std::map<std::string, location> locations;  // done
    unsigned long long client_max_body_size;

   public:
    serverInfo(const serverInfo& sv);

    serverInfo();

    ~serverInfo();

    serverInfo& operator=(const serverInfo& info);

    template <int>
    void setters(std::list<tokengen>::iterator& big,
                 std::list<tokengen>::iterator& end);

    void setlocation(std::string x, location y);

    void execute(int i,
                 std::list<tokengen>::iterator& big,
                 std::list<tokengen>::iterator& end);
};
