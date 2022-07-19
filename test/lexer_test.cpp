#include <iostream>
#include "../src/lexer.hpp"

int main() {
    std::string input = "let x = 11\n"
                        "\n"
                        "let y: i32 = 1.29\n"
                        "if x == y && x + y < 200 || 1 == 1 {\n"
                        "println(\"hello world\") // some inline comment\n"
                        "for x in arr {println(\"Found x right here {}\", x)}\n"
                        "< > <= >= != ==\n";
    Lexer lex(input, [](AST::FilePos pos, std::string msg) {
        printf("%lld:%lld %s\n", pos.row, pos.col, msg.c_str());
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
        {Token::LESS, ""},
        {Token::GREATER, ""},
        {Token::LESSEQ, ""},
        {Token::GREATEREQ, ""},
        {Token::NOTEQ, ""},
        {Token::EQUAL, ""},
        {Token::NEWLINE, ""},
        {Token::ENDMARKER, ""},
    };
    int i = 0;
  //  LexTok lt; 
  //  while ((lt = lex.nextToken()).type != Token::ENDMARKER) {
  //      std::cout << token_string[lt.type] << ' ';
  //      std::cout << "'" << lt.literal << "'" << std::endl;
  //  }

    for (const auto& test: tests) {
        auto lex_tok = lex.nextToken();
        if (lex_tok.type != test.tok) {
            std::cout << "[ERROR] tokens don't match: " << "test number " << i << ": ";
            std::cout << "want " << test.tok << " got '" << lex_tok.type << "'\n";
            return 1;
        } else if (lex_tok.literal != test.literal) {
            std::cout << "[ERROR] literals don't match: " << "test number " << i << ": ";
            std::cout << "want " << test.literal << " got '" << lex_tok.literal << "'\n"; 
            return 1;
        }
        i++; 
    }
    std::cout << "LEXER tests passed successfully.\n";
}
