/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tocken.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakllam <sakllam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 16:18:40 by sakllam           #+#    #+#             */
/*   Updated: 2022/11/09 16:49:13 by sakllam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
namespace ft
{
    enum Type {
        OPENCURL,
        CLOSECURL,
        SEMICOLONS,
        WHITESPACE,
        WORD,
        QUOTES,
        COLON
    };
    
    template <int T>
    class tokengen
    {
    };
    
    template <>
    class tokengen<OPENCURL>
    {
        int type;
        tokengen() : type(OPENCURL) {}
    };
    
    template <>
    class tokengen<CLOSECURL>
    {
        int type;
        tokengen() : type(CLOSECURL) {}
    };

    template <>
    class tokengen<SEMICOLONS>
    {
        int type;
        tokengen() : type(SEMICOLONS) {}
    };

    template <>
    class tokengen<WHITESPACE>
    {
        int type;
        tokengen() : type(WHITESPACE) {}
    };
    
    template <>
    class tokengen<WORD>
    {
        int type;
        std::string word;
        tokengen(std::string word) : type(WORD), word(word) {}
    };

    template <>
    class tokengen<QUOTES>
    {
        int type;
        std::string word;
        tokengen(std::string word) : type(QUOTES), word(word) {}
    };
    
    template <>
    class tokengen<COLON>
    {
        int type;
        tokengen() : type(COLON) {}
    };
}