#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <ostream>
#include <map>

enum class Token {
    ENDMARKER,
    NEWLINE, 
    UNKNOWN, // given to unknown chars

    _beg_literals,
    IDENT, // x, y, z...
    STRING, // "foo"
    INT, // 123
    FLOAT, // 3.14
    _end_literals,

    _beg_operators,
    ASSIGN, // =

    ADD, // +
    SUB, // -
    MUL, // *
    DIV, // /
    REM, // %

    NOT, // !

    EQUAL, // ==
    NOTEQ, // !=
    GREATER, // >
    LESS, // <
    LESSEQ, // <=
    GREATEREQ, // >=

    AND, // &&
    OR, // || 

    COMMA, // ,
    COLON, // :
    DOT, // .

    LBRACE, // {
    RBRACE, // }
    LBRACKET, // [
    RBRACKET, // ]
    LPAREN, // (
    RPAREN, // )
    _end_operators,

    _beg_keywords,
    LET,
    IF,
    IN,
    ELSE,
    TRUE,
    FALSE,
    FUN,
    FOR,
    WHILE,
    BREAK,
    CONTINUE,
    _end_keywords,
};

extern std::map<Token, std::string> token_string;
std::ostream& operator<<(std::ostream& os, Token tok);

#endif
