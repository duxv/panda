#include <iostream>
#include <fstream>
#include "./lexer.hpp"

int main() {
    std::string s = "FFFFFF\"";
    Lexer lex(s, [](std::size_t offs, const char *s) {
        fprintf(stderr, "got an error at %ld\n", offs);
        std::cerr << s << std::endl;
        std::exit(1);
    });
}
