#include <iostream>
#include "../src/lexer.hpp"


int main() {
    int i = 0;
    std::string input = "let x = y\n"
                        "break // comments should be ignored\n"
                        "/* multiline comments\n"
                        "should be ignored as \n\n\nwell*/";
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
        {Token::IDENT, "y"},
        {Token::NEWLINE, ""},
        {Token::BREAK, "break"},
        {Token::NEWLINE, ""},
        {Token::ENDMARKER, ""},
    };
    for (const auto& test: tests) {
        auto lex_tok = lex.nextToken();
        if (lex_tok.first != test.tok) {
            std::cout << "[ERROR] tokens don't match: " << "test number " << i << ": ";
            std::cout << "want " << test.tok << " got " << lex_tok.first << '\n';
            return 1;
        } else if (lex_tok.second != test.literal) {
            std::cout << "[ERROR] literals don't match: " << "test number " << i << ": ";
            std::cout << "want " << test.literal << " got " << lex_tok.second << '\n'; 
        }
        i++; 
    }
    std::cout << "LEXER tests passed successfully.\n";
}
