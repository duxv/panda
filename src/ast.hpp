#ifndef AST_HPP
#define AST_HPP

#include <iostream>
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
        virtual std::string string() = 0;
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
        explicit Expr(std::size_t pos, expr_t type)
        : Node(pos, NODE_EXPR), m_type(type) {}
    };

    enum literal_t {
        LIT_FLOAT,
        LIT_INT,
        LIT_STRING,
        LIT_IDENT,
    };

    struct Literal : public Expr {
        literal_t type;
        std::string value;
    };
}

#endif