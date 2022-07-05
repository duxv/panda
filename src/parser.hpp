#ifndef PARSER_HPP
#define PARSER_HPP

#include "lexer.hpp"
#include "ast.hpp"

// Parser cannot go behind this number of errors
inline constexpr int MAX_ERRORS = 0;

struct Bailout{}; // this is thrown when there are too many errors

struct FilePos {
    std::size_t row;
    std::size_t col;
};

struct ParseError {
    FilePos pos;
    std::string msg;
    std::string format();
};

class Parser {
    Lexer m_lexer;
    LexTok tok;
    std::size_t m_pos;
    std::vector<ParseError> errors;

    void next();
    AST::IdentLit* parse_ident();
    AST::IntLit* parse_int();
    AST::FloatLit* parse_float();
    AST::Expr* parse_binary_expr(int prec1);
    AST::Expr* parse_unary_expr();
    AST::Expr* parse_operand();
    AST::Expr* parse_expr();
    AST::Stmt* parse_stmt();
    AST::StmtExpr* parse_stmt_expr();
    std::vector<AST::Stmt*> parse_stmt_list();
    void (*error_handler) (std::size_t, const char* msg);

    // Handling errors
    std::size_t expect(Token tok);
    void error_expected(std::size_t pos, std::string wanted);
    void error(std::size_t pos, std::string msg);
public:
    explicit Parser(const std::string& input, void (*error_handler)(std::size_t, const char* msg)) 
    : m_lexer(input, error_handler) {}
    AST::Program* parse_program();    
};

#endif