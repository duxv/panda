#ifndef PARSER_HPP
#define PARSER_HPP

#include "lexer.hpp"
#include "ast.hpp"

inline constexpr int MAX_ERRORS = 1;

struct ParseError {
    std::size_t pos;
    std::string msg;
};

class Parser {
    Lexer m_lexer;
    Token tok;
    std::size_t pos;
    std::string literal;
    std::vector<ParseError> errors;

    void next();
    AST::ExprBinary* parse_binary_expr();
    AST::ExprUnary* parse_unary_expr();
    AST::Expr* parse_expr();
    AST::Stmt* parse_stmt();
    AST::StmtExpr* parse_expr_stmt();
    std::vector<AST::Stmt*> parse_stmt_list();
    void (*error_handler) (std::size_t, const char* msg);

    std::size_t expect(Token tok);
    void error_expected(std::size_t pos, std::string wanted);
    void error(std::size_t pos, std::string msg);
public:
    explicit Parser(const std::string& input, void (*error_handler)(std::size_t, const char* msg)) 
    : m_lexer(input, error_handler) {}
    AST::Program* parse_program();    
};

#endif