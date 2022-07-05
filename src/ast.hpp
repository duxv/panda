#ifndef AST_HPP
#define AST_HPP
#include <iostream>
#include <vector>
#include <cstdint>
#include "token.hpp"

namespace AST {

    enum NodeType {
        NODE_PROGRAM,

        STMT_EXPR,
        STMT_IF,
        STMT_FOR,
        STMT_WHILE,
        STMT_LET,

        EXPR_LIT_STRING,
        EXPR_LIT_INT,
        EXPR_LIT_FLOAT,
        EXPR_LIT_IDENT,
        EXPR_UNARY,
        EXPR_BINARY,
        EXPR_BAD,
    };
    struct Node {
    private:
        std::size_t location;
    public:
        explicit Node(std::size_t location)
        : location(location) {}
        std::size_t pos() { return location; };
        virtual std::string string() = 0;
        virtual NodeType type() = 0;
        virtual ~Node() { };
    };

    struct Stmt : public Node {
        explicit Stmt(std::size_t location)
        : Node(location) {};
        virtual std::string string() = 0;
        virtual NodeType type() = 0;
    };

    struct Expr : public Node {
        explicit Expr(std::size_t location)
        : Node(location) {}
        virtual std::string string() = 0;
        virtual NodeType type() = 0;
    };

    struct Program : public Node {
        std::vector<Stmt*> stmts;

        Program() : Node(0) {}
        ~Program() {
            for (Stmt* stmt : stmts)
                delete stmt;
        }
    
        std::string string() override {
            std::string s;
            for (Stmt* stmt : stmts) s += stmt->string();
            return s;
        }
        NodeType type() override { return NODE_PROGRAM; }

    };


    /* 
     * Exprs
     */
    struct StringLit : public Expr {
        std::string value;

        explicit StringLit(std::size_t pos) : Expr(pos) {}
        explicit StringLit(std::string value, std::size_t pos)
        : Expr(pos), value(value) {}
        std::string string() override { return value; }
        NodeType type() override { return EXPR_LIT_STRING; }
    };

    struct IntLit : public Expr {
        int64_t value;

        explicit IntLit(std::size_t pos) : Expr(pos) {}
        explicit IntLit(int64_t value, std::size_t pos)
        : Expr(pos), value(value) {}
        std::string string() override { return std::to_string(value); }
        NodeType type() override { return EXPR_LIT_INT; }
    };

    struct FloatLit : public Expr {
        double value;

        explicit FloatLit(std::size_t pos) : Expr(pos) {}
        explicit FloatLit(double value, std::size_t pos)
        : Expr(pos), value(value) {}
        std::string string() override { return std::to_string(value); }
        NodeType type() override { return EXPR_LIT_FLOAT; }
    };

    struct IdentLit : public Expr {
        std::string value;

        explicit IdentLit(std::size_t pos) : Expr(pos) {}
        explicit IdentLit(std::string value, std::size_t pos)
        : Expr(pos), value(value) {}
        std::string string() override { return value; }
        NodeType type() override { return EXPR_LIT_IDENT; }
    };

    struct ExprUnary : public Expr {
        Token op;
        Expr *right;

        explicit ExprUnary(std::size_t pos) : Expr(pos) {}
        std::string string() override { return token_string[op] + ' ' + right->string(); }
        NodeType type() override { return EXPR_UNARY; }
    };

    struct ExprBinary : public Expr {
        Expr *left;
        Token op;
        Expr *right;

        explicit ExprBinary(std::size_t pos) : Expr(pos) {}
        std::string string() override { 
            return left->string() + ' ' + token_string[op] + ' ' + right->string();
        }
        NodeType type() override { return EXPR_BINARY; }
    };

    struct ExprBad : public Expr {
        std::string string() override { return "<INVALID EXPRESSION>"; }
        explicit ExprBad(std::size_t pos) : Expr(pos) {}
        NodeType type() override { return EXPR_BAD; }
    };
    /*
     * Statements
     */

    struct StmtExpr : public Stmt {
        Expr *expr;

        explicit StmtExpr(std::size_t pos) : Stmt(pos) {} 
        std::string string() override { return expr->string() + '\n'; }
        NodeType type() override { return STMT_EXPR; }
    };
}
#endif
