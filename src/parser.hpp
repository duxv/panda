#ifndef PARSER_HPP
#define PARSER_HPP

#include "lexer.hpp"
#include "ast.hpp"


class Parser {
    Lexer m_lexer;
    LexTok tok;
    std::size_t m_pos;

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


    // Handling errors
    void (*error_handler) (AST::FilePos, std::string);
    std::size_t expect(Token tok);
    void error_expected(std::size_t pos, std::string wanted);
    void error(std::size_t pos, std::string msg);
public:
    explicit Parser(const std::string& input, void (*error_handler)(AST::FilePos, std::string))
    : m_lexer(input, error_handler), error_handler(error_handler) {}
    AST::Program* parse_program();    
};

#endif