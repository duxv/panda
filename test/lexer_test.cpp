#include <iostream>
#include "../src/lexer.hpp"


int main() {
    std::string input = "let x = 11\n\n"
                        "let y: i32 = 1.29\n"
                        "if x == y && x + y < 200 || 1 == 1 {\n"
                        "println(\"hello world\")\n"
                        "for x in arr { println(\"Found x right here {}\", x)}\n";
                        "/*Multiline string\n\n*/";
    Lexer lex(input, [](std::size_t pos, const char* msg) {
        printf("POS: %ld: Error %s\n", pos, msg);
        std::exit(0);
    });
    struct Test {
        Token tok;
        std::string literal;
    };
    Test tests[] {
        {Token::LET, "let"},
        {Token::IDENT, "x"},
        {Token::ASSIGN, ""},
        {Token::INT, "11"},
        {Token::NEWLINE, ""},
        {Token::LET, "let"},
        {Token::IDENT, "y"},
        {Token::COLON, ""},
        {Token::IDENT, "i32"},
        {Token::ASSIGN, ""},
        {Token::FLOAT,"1.29"},
        {Token::NEWLINE, ""},
        {Token::IF, "if"},
        {Token::IDENT, "x"},
        {Token::EQUAL, ""},
        {Token::IDENT, "y"},
        {Token::AND, ""},
        {Token::IDENT, "x"},
        {Token::ADD, ""},
        {Token::IDENT, "y"},
        {Token::LESS, ""},
        {Token::INT, "200"},
        {Token::OR, ""},
        {Token::INT, "1"},
        {Token::EQUAL, ""},
        {Token::INT, "1"},
        {Token::LBRACE, ""},
        {Token::NEWLINE, ""},
        {Token::IDENT, "println"},
        {Token::LPAREN, ""},
        {Token::STRING, "hello world"},
        {Token::RPAREN, ""},
        {Token::NEWLINE, ""},
        {Token::FOR, "for"},
        {Token::IDENT, "x"},
        {Token::IN, "in"},
        {Token::IDENT, "arr"},
        {Token::LBRACE, ""},
        {Token::IDENT, "println"},
        {Token::LPAREN, ""},
        {Token::STRING, "Found x right here {}"},
        {Token::COMMA, ""},
        {Token::IDENT, "x"},
        {Token::RPAREN, ""},
        {Token::RBRACE, ""},
        {Token::NEWLINE, ""},
        {Token::ENDMARKER, ""},
    };
    int i = 0;
    for (const auto& test: tests) {
        auto lex_tok = lex.nextToken();
        if (lex_tok.first != test.tok) {
            std::cout << "[ERROR] tokens don't match: " << "test number " << i << ": ";
            std::cout << "want " << test.tok << " got '" << lex_tok.first << "'\n";
            return 1;
        } else if (lex_tok.second != test.literal) {
            std::cout << "[ERROR] literals don't match: " << "test number " << i << ": ";
            std::cout << "want " << test.literal << " got '" << lex_tok.second << "'\n"; 
            return 1;
        }
        i++; 
    }
    std::cout << "LEXER tests passed successfully.\n";
}