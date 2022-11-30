// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   Serverspack.cpp                                    :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: sakllam <sakllam@student.42.fr>            +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2022/11/30 17:56:27 by sakllam           #+#    #+#             */
// /*   Updated: 2022/11/30 18:37:21 by sakllam          ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #pragma once
// #include <string>
// #include <map>
// typedef int Info_data; // will change to the class name
// typedef int port;
// typedef std::string server_name;
// typedef std::pair<port , server_name> id;
// class Serverspack
// {
//     std::map<id, Info_data> x;
//     Serverspack(id, Info_data);
//     Info_data get_aproserver(int id_Port, server_name x);
// };


// Serverspack::Serverspack(id a, Info_data y)
// {
    // was palaning to generate stuff here to get a class instead of the nested class >_<
// }
// Info_data Serverspack::get_aproserver(const id a)
// {
//     std::map<id, Info_data>::iterator iter = x.find(a);
//     if (iter == x.end())
//     {
//         iter = x.find(std::make_pair(a.first, ""));
//         return iter->second;
//     }
//     return iter->second;
// }