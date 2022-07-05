#ifndef LEXER_HPP
#define LEXER_HPP

#include <iostream>
#include "token.hpp"
#include "ast.hpp"

struct LexTok {
    Token type;
    std::string literal;

    friend bool operator==(LexTok &l, Token tok) { return l.type == tok; }
    friend bool operator!=(LexTok &l, Token tok) { return l.type != tok; }
    friend bool operator==(LexTok &l, std::string literal) { return l.literal == literal; }
    friend bool operator!=(LexTok &l, std::string literal) { return l.literal != literal; }
};

class Lexer {
    std::string_view input;
    char ch = 0;
    std::size_t offset = 0;
    int row = 1, col = 1; // pos in the input
    void (*error_handler)(AST::FilePos, std::string);
    
    // Advance to the next byte
    void read();
    // Peek the next char, after the curent one and return it. Without advancing.
    char peek();
    void skip_whitespace();
    void read_digits(int base);
    std::string read_string();
    std::string read_ident();

    // converts an offset to the position in rows and columns
    // then passes them to the error_handler, together with the std::string
    void error(std::size_t offs, std::string msg); 
    LexTok read_number();
    bool read_escape();

public:
    std::string_view get_input() { return input; }
    std::size_t get_pos() { return offset; };
    explicit Lexer(const std::string& s, void(*error_handler)(AST::FilePos, std::string)) 
    : input(s),  error_handler(error_handler) {
        if (input.size() > 0)
            ch = input[0]; // initialize the first char
    }
    LexTok nextToken();
};
#endif
