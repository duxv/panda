#include "token.hpp"
#include <iostream>
#include <map>


std::map<Token, std::string> token_string {
    {Token::ENDMARKER, "ENDMARKER"},
    {Token::NEWLINE, "NEWLINE"},
    {Token::UNKNOWN, "UNKNOWN"},
    {Token::IDENT, "IDENT"},
    {Token::STRING, "STRING"},
    {Token::INT, "INT"},
    {Token::FLOAT, "FLOAT"},
    {Token::ASSIGN, "="},
    {Token::ADD,  "+"},
    {Token::SUB, "-"},
    {Token::MUL, "*"},
    {Token::DIV, "/"},
    {Token::REM, "%"},
    {Token::NOT,  "!"},
    {Token::EQUAL,  "=="},
    {Token::NOTEQ,  "!="},
    {Token::GREATER,  ">"},
    {Token::LESS,  "<"},
    {Token::LESSEQ,  "<="},
    {Token::GREATEREQ,  ">="},

    {Token::AND, "&&"},
    {Token::OR, "||"},

    {Token::COMMA,  ","},
    {Token::COLON,  ":"},
    {Token::DOT,  "."},

   {Token::LBRACE, "{"},
   {Token::RBRACE, "}"},
   {Token::LBRACKET,   "["},
   {Token::RBRACKET,   "]"},
   {Token::LPAREN,     "("},
   {Token::RPAREN,     ")"},
   {Token::LET,"let"},
   {Token::IF,"if"},
   {Token::IN, "in"},
   {Token::ELSE,"else"},
   {Token::TRUE,"true"},
   {Token::FALSE,"false"},
   {Token::FUN,"fun"},
   {Token::FOR,"for"},
   {Token::WHILE,"while"},
   {Token::BREAK,"break"},
   {Token::CONTINUE,"continue"},
};

std::ostream& operator<<(std::ostream& os, Token tok) {
    os << token_string[tok];
    return os;
}
