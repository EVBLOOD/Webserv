/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTP_Response.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakllam <sakllam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 19:10:31 by sakllam           #+#    #+#             */
/*   Updated: 2022/11/30 19:31:27 by sakllam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once
typedef int serverInfo; // to change with the parsing data
typedef int requestInfo; // to change with the parsing data

class Http_Response
{
    serverInfo x;
    
    Http_Response(serverInfo x);
    void answer(requestInfo a);
    void httpGet();
    void httpPost();
    void httpDelete();
};

Http_Response::Http_Response(serverInfo x)
{
    this->x = x;
}

void Http_Response::answer(requestInfo a)
{
    if (a.method == "GET")
        this->httpGet();
    else if (a.method == "POST")
        this->httpPost();
    else if (a.method == "DELETE")
        this->httpDelete();
    else
     // method u are requesting doesn't exist;
}