/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tocken.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakllam <sakllam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 16:18:40 by sakllam           #+#    #+#             */
/*   Updated: 2022/11/09 16:51:43 by sakllam          ###   ########.fr       */
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
    struct tokengen
    {
    };
    
    template <>
    struct tokengen<OPENCURL>
    {
        int type;
        tokengen() : type(OPENCURL) {}
    };
    
    template <>
    struct tokengen<CLOSECURL>
    {
        int type;
        tokengen() : type(CLOSECURL) {}
    };

    template <>
    struct tokengen<SEMICOLONS>
    {
        int type;
        tokengen() : type(SEMICOLONS) {}
    };

    template <>
    struct tokengen<WHITESPACE>
    {
        int type;
        tokengen() : type(WHITESPACE) {}
    };
    
    template <>
    struct tokengen<WORD>
    {
        int type;
        std::string word;
        tokengen(std::string word) : type(WORD), word(word) {}
    };

    template <>
    struct tokengen<QUOTES>
    {
        int type;
        std::string word;
        tokengen(std::string word) : type(QUOTES), word(word) {}
    };
    
    template <>
    struct tokengen<COLON>
    {
        int type;
        tokengen() : type(COLON) {}
    };
}