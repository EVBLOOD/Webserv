/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokengen.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakllam <sakllam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 16:18:40 by sakllam           #+#    #+#             */
/*   Updated: 2022/11/09 18:06:20 by sakllam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once
# include <string>

enum Type {
    OPENCURL,
    CLOSECURL,
    SEMICOLONS,
    WHITESPACE,
    WORD,
    QUOTES,
    COLON
};


struct tokengen
{
    int type;
    std::string content;
    tokengen(int type);
    tokengen(int type, std::string content);
};

