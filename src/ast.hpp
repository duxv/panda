#ifndef AST_HPP
#define AST_HPP

#include <iostream>
#include <vector>
#include "token.hpp"

namespace AST {

    enum node_t {
        NODE_STMT,
        NODE_EXPR,
    };
    struct Node {
    private:
        std::size_t m_pos;
        node_t m_type;
    public:
        std::size_t pos() { return m_pos; }
        node_t node_type() { return m_type; }
        explicit Node(std::size_t pos, node_t type) 
        : m_pos(pos), m_type(type) {}
    };

    enum stmt_t {
        STMT_EXPR,
        STMT_IF,
        STMT_FOR,
        STMT_WHILE,
        STMT_LET,
    };

    struct Stmt : public Node {
    private:
        stmt_t m_type;
    public:
        stmt_t stmt_type() { return m_type; }
        explicit Stmt(std::size_t pos, stmt_t type)
        : Node(pos, NODE_STMT), m_type(type) {}
    };

    enum expr_t {
        EXPR_LITERAL,
        EXPR_BINARY,
        EXPR_UNARY,
    };

    struct Expr : public Node {
    private:
        expr_t m_type;
    public:
        expr_t expr_type() { return m_type; }
        explicit Expr(std::size_t pos, expr_t type)
        : Node(pos, NODE_EXPR), m_type(type) {}
    };

    struct Program : public Node {
        std::vector<Stmt> stmts;
    };

    enum literal_t {
        LIT_FLOAT,
        LIT_INT,
        LIT_STRING,
        LIT_IDENT,
    };

    struct ExprLiteral : public Expr {
        literal_t type;
        std::string value;
        explicit ExprLiteral(std::size_t pos, std::string value) 
        : Expr(pos, EXPR_LITERAL), value(value) {}

        literal_t literal_type() { return type; }

        std::ostream& operator<<(std::ostream& os) {
            
        }
    };

    struct ExprUnary : public Expr {
        Token op;
        Expr *right;
        explicit ExprUnary(std::size_t pos, Token op, Expr *right)
        : Expr(pos, EXPR_UNARY), op(op), right(right) {}
    };

    struct ExprBinary : public Expr {
        Expr *left;
        Token op;
        Expr *right;
        explicit ExprBinary(std::size_t pos, Expr *left, Token op, Expr *right)
        : Expr(pos, EXPR_BINARY), left(left), op(op), right(right) {}
    };

    struct StmtIf : public Stmt {
        std::vector<Expr> cond; // all conditions
        std::vector<Stmt> actions; // all if actions
        std::vector<Stmt> alternative; // the else
    };
}

#endif