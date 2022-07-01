#ifndef PARSER_HPP
#define PARSER_HPP

#include "lexer.hpp"

class Parser {
    Lexer m_lexer;
    explicit Parser(const std::string& input, void (*error_handler)(std::size_t, const char* msg)) 
    : m_lexer(input, error_handler) {}
};

#endif