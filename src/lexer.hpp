#ifndef LEXER_HPP
#define LEXER_HPP

#include <iostream>
#include "token.hpp"

struct LexTok {
    Token type;
    std::string literal;

    friend bool operator==(LexTok l, Token tok) { return l.type == tok; }
};

class Lexer {
    std::string input;
    char ch = 0;
    std::size_t offset = 0;
    int row = 1, col = 1; // pos in the input
    void (*error_handler)(std::size_t, const char*);
    
    // Advance to the next byte
    void read();
    // Peek the next char, after the curent one and return it. Without advancing.
    char peek();
    void skip_whitespace();
    void read_digits(int base);
    std::string& read_string();
    std::string& read_ident();
    std::pair<Token, std::string>& read_number();
    bool read_escape();

public:
    explicit Lexer(const std::string& s, void(*error_handler)(std::size_t, const char*)) 
    : input(s),  error_handler(error_handler) {
        if (input.size() > 0)
            ch = input[0]; // initialize the first char
    }
    std::pair<Token, std::string>& nextToken();
};

#endif
