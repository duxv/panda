#include "parser.hpp"
#include <iostream>
#include <string>
#include <limits.h>


using namespace AST;

int lowest_prec = 0;

static
int precedence(Token tok) {
    switch (tok) {
        case Token::OR: return 1;
        case Token::AND: return 2;
        case Token::EQUAL:
        case Token::NOTEQ:
        case Token::LESS:
        case Token::GREATER:
        case Token::LESSEQ:
        case Token::GREATEREQ:
            return 3;
        case Token::ADD:
        case Token::SUB:
            return 4;
        case Token::MUL:
        case Token::DIV:
            return 5;
        default:
            return lowest_prec;
    }
}

static
bool is_stmt_start(Token tok) {
    switch (tok) {
        case Token::IF:
        case Token::LET:
        case Token::WHILE:
        case Token::FOR:
        case Token::RETURN:
        case Token::BREAK:
        case Token::CONTINUE:
            return true;
        default:
            return false;
    }
}

std::string ParseError::format() {
    std::string s = std::to_string(this->pos.row);
    s += ':';
    s += std::to_string(this->pos.col);
    s += " ParserError: ";
    s += msg;
    return s;
}

static
FilePos get_row_and_col(std::size_t offset, std::string_view s) {
    FilePos pos = {1, 1};

    for (int i = 0; i <= offset; i++) {
        if (s[i] == '\n') {
            pos.col = 1;
            pos.row += 1;
        } else {
            pos.col += 1;
        }
    }
    return pos;
}

void Parser::next() {
    m_pos = m_lexer.get_pos();
    tok = m_lexer.nextToken();
}

void Parser::error(std::size_t pos, std::string msg) {
    FilePos file_pos = get_row_and_col(pos, m_lexer.get_input());

    errors.push_back({file_pos, msg});
    if (errors.size() > MAX_ERRORS)
        throw Bailout{};
}

void Parser::error_expected(std::size_t pos, std::string msg) {
    msg = "expected " + msg;
    error(pos, msg);
}

std::size_t Parser::expect(Token e) {
    std::size_t pos = m_pos;
    if (tok != e) {
        error_expected(pos, "'"+token_string[e]+"'");
    }
    next();
    return pos;
}

std::vector<Stmt*> Parser::parse_stmt_list() {
    std::vector<Stmt*> ret;
    while (tok != Token::RBRACE && tok != Token::ENDMARKER)
        ret.push_back(parse_stmt());
    return ret;
}

Stmt* Parser::parse_stmt() {
    switch (tok.type) {
        default:
            return parse_stmt_expr();
    }
}

StmtExpr* Parser::parse_stmt_expr() {
    Expr* expr =  parse_expr();
    StmtExpr* stmt = new StmtExpr(m_pos);
    stmt->expr = expr;
    //expect(Token::NEWLINE);
    return stmt;
}

Expr* Parser::parse_expr() {
    Expr *expr = parse_binary_expr(lowest_prec);
    return expr;
}

IdentLit* Parser::parse_ident() {
    IdentLit* ident = new IdentLit(m_pos);
    std::string name = "_";

    if (tok == Token::IDENT) {
        name = tok.literal;
        next();
    } else {
        expect(Token::IDENT);
    }

    ident->value = name;
    return ident;
}

IntLit* Parser::parse_int() {
    IntLit* num = new IntLit(m_pos);
    char* e;
    errno = 0;
    int64_t value = std::strtoll(tok.literal.c_str(), &e, 0);
    if (*e != '\0')
        error(m_pos, "invalid integer");
    if (errno != 0)
        error(m_pos, "integer out of range");
    num->value = value; 
    next();
    return num; 
}

FloatLit* Parser::parse_float() {
    char *e;
    FloatLit* num = new FloatLit(m_pos);
    errno = 0;
    double value = std::strtod(tok.literal.c_str(), &e);
    if (*e != '\0')
        error(m_pos, "invalid float");
    if (errno != 0)
        error(m_pos, "float out of range");
    num->value = value;
    next();
    return num;
}

Expr* Parser::parse_operand() {
    switch (tok.type) {
        case Token::IDENT:    return parse_ident();
        case Token::INT:      return parse_int();
        case Token::FLOAT:    return parse_float();
        default:
            error(m_pos, "invalid expression");
            Expr* bad = new ExprBad(m_pos);
            while (!is_stmt_start(tok.type) && tok != Token::ENDMARKER)
                next();
            return bad;
    }
}

Expr* Parser::parse_unary_expr() {
    std::size_t pos = m_pos;
    Token op = tok.type;
    Expr *x;
    ExprUnary *ret;
    switch(tok.type) {
        case Token::ADD:
        case Token::SUB:
        case Token::NOT:
            next();
            x = parse_unary_expr();
            ret = new ExprUnary(pos);
            ret->op = op;
            ret->right = x; 
            return ret;
        default:
            return parse_operand();
    }
}

Expr* Parser::parse_binary_expr(int prec1) {
    Expr* left = parse_unary_expr();
    for (;;) {
        Token op = tok.type;
        int prec = precedence(op);

        if (prec < prec1) return left;

        std::size_t pos = expect(op);

        Expr *right = parse_binary_expr(prec + 1);

        ExprBinary* temp = new ExprBinary(pos);
        temp->left = left;
        temp->op = op;
        temp->right = right; 
        left = temp;
    }
}