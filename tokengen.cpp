/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokengen.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakllam <sakllam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 18:06:02 by sakllam           #+#    #+#             */
/*   Updated: 2022/11/09 18:06:14 by sakllam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokengen.hpp"

tokengen::tokengen(int type, std::string content) : type(type), content(content)
{
}

tokengen::tokengen(int type) : type(type)
{
    
}