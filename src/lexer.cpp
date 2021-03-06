#include "lexer.hpp"

#include <memory>
#include <map>


static inline
bool is_identifier_start(char c) {
    return ('a' <= c && c <= 'z') ||
        ('A' <= c && c <= 'Z') || 
        (c == '_');
}

static inline
bool is_identifier_part(char c) {
    return ('a' <= c && c <= 'z') ||
        ('A' <= c && c <= 'Z') ||
        ('0' <= c && c <= '9') || 
        (c == '_');
}


static inline
bool is_space(char c) {
    return c == ' ' || c == '\t' || c == '\r';
    // Newlines are not considered ordinary space.
}

static inline
int digit_value(char c) {
    if ('0' <= c && c <= '9')
        return (c - '0');
    else if ('a' <= c && c <= 'f')
        return (c - 'a' + 10);
    else if ('A' <= c && c <= 'F')
        return (c - 'A' + 10);
    return 16; // larger than any legal digit
}

static
Token lookup_keyword(const std::string& ident) {
    if (ident == "let")         return Token::LET;
    if (ident == "if")          return Token::IF;
    if (ident == "else")        return Token::ELSE;
    if (ident == "true")        return Token::TRUE;
    if (ident == "false")       return Token::FALSE;
    if (ident == "fun")         return Token::FUN;
    if (ident == "for")         return Token::FOR;
    if (ident == "while")       return Token::WHILE;
    if (ident == "break")       return Token::BREAK;
    if (ident == "continue")    return Token::CONTINUE;
    if (ident == "in")          return Token::IN;

    return Token::IDENT;
}

void Lexer::error(std::size_t offs, std::string msg) {
    error_handler(AST::FilePos_from_offset(offs, input), "Lexer Error: " + msg);
}

void Lexer::read() {    
    offset += 1;
    if (offset >= input.size()) {
        ch = 0;
        return;
    }
    ch = input[offset];

    if (ch == '\n') {
        row += 1;
        col = 1;
    } else {
        col += 1;
    }
}

char Lexer::peek() {
    std::size_t nextOffset = offset + 1;
    if (nextOffset < input.size()) {
        return input[nextOffset];
    }
    return 0;
}

void Lexer::skip_whitespace() {
    while (is_space(ch)) read();
}

void Lexer::read_digits(int base) {
    while (ch == '_' || digit_value(ch) < base) {
        read();
    }
}

bool Lexer::read_escape() {
    switch (ch) {
        case 'a':
        case 'b':
        case 'f':
        case 'n':
        case 'r':
        case 't':
        case 'v':
        case '\\':
        case '\'':
        case '"':
        case '\n':
            {
                read();
                return true;
            }
        default:
            return false;
    }
}

std::string Lexer::read_string() {
    std::size_t offs = offset; // already skipped the '"'
    for (;;) {
        char _ch = ch;
        if (ch == '\n' || ch == 0) {
                error(offset, "string literal not terminated");
                break;
        }
        read();
        if (_ch == '"') break;
        if (_ch == '\\') read_escape();
    }
    return std::string(input.substr(offs, offset - offs - 1));
    // -1 in order to not include the last '"'
}

std::string Lexer::read_ident() {
    std::size_t offs = offset;

    while (is_identifier_part(ch)) {
        read();
    }
    return std::string(input.substr(offs, offset-offs));
}

LexTok Lexer::read_number() {
    int offs = offset;
    LexTok ret;
    ret.type = Token::INT; // Assuming it is an int
    int base = 10; // assumed base is 10
    if (ch == '0') {
        char nch = tolower(peek());
        if (nch == 'b') {
            base = 2;
            read();
            read();
        } else if (nch == 'o') {
            base = 8;
            read();
            read();
        } else if (nch == 'x') {
            base = 16;
            read();
            read();
        }
    }

    // read entire number
    read_digits(base);

    // fractional part
    if (ch == '.' && (base == 10 || base == 16)) {
        ret.type = Token::FLOAT;
        read();
        read_digits(base);
    }

    // exponent
    if (ch == 'e' || ch == 'E' || ch == 'p' || ch == 'P') {
        ret.type = Token::FLOAT;
        read();
        if (ch == '-' || ch == '+') {
            read();
        }
        int offs = offset;
        read_digits(10);
        if (offs == offset) {
            error(offset, "exponent has no digits");
        }
    }
    ret.literal = input.substr(offs, offset-offs);
    return ret;
}

LexTok Lexer::nextToken() {
    skip_whitespace();
    char _ch = ch;
    LexTok ret; 

    if (is_identifier_start(ch)) {
        ret.literal = read_ident(); 
        ret.type = lookup_keyword(ret.literal);
    } else if (isdigit(ch)) {
        ret = read_number();
    } else { 
        goto read_operators;
    }
    
    return ret;


read_operators:
    // always advance
    read();

    switch (_ch) {
        case 0:
            ret.type = Token::ENDMARKER;
            break;
        case '\n':
            while(ch == '\n') read(); // skip all the newlines
            ret.type = Token::NEWLINE;
            break;
        case ',':
            ret.type = Token::COMMA;
            break;
        case '+': 
            ret.type = Token::ADD;
            break;
        case '-': 
            ret.type = Token::SUB;
            break;
        case '*': 
            ret.type = Token::MUL;
            break;
        case '/':
        {
            if (ch == '/') {
                while (ch != '\n') read();
                return nextToken();
            }
            else
                ret.type = Token::DIV;
            break;
        }
        case '%': 
            ret.type = Token::REM;
            break;
        case ';':
            ret.type = Token::NEWLINE; // We treat ';' as a newline.
            break;
        case ':':
            ret.type = Token::COLON;
            break;
        case '(':
            ret.type = Token::LPAREN;
            break;
        case ')':
            ret.type = Token::RPAREN;
            break;
        case '[':
            ret.type = Token::LBRACKET;
            break;
        case ']':
            ret.type = Token::RBRACKET;
            break;
        case '{':
            ret.type = Token::LBRACE;
            break;
        case '}':
            ret.type = Token::RBRACE;
            break;
        case '.':
            ret.type = Token::DOT;
            break;
        case '&':
            if (ch != '&') {
                std::cout << ch << std::endl;
                ret.type = Token::UNKNOWN;
                ret.literal = _ch;
            } else {
                read();
                ret.type = Token::AND;
            }
            break;
        case '|':
            if (ch != '|') {
                ret.type = Token::UNKNOWN;
                ret.literal = _ch;
            } else {
                read();
                ret.type = Token::OR;
            }
            break;
        case '!':
        {
            if (ch == '=') {
                read();
                ret.type = Token::NOTEQ;
            } else
                ret.type = Token::NOT;
            
            break;
        }
        case '<':
        {
            if (ch == '=') {
                read();
                ret.type = Token::LESSEQ;
            } else
                ret.type = Token::LESS;
            break;
        }
        case '>':
        {
            if (ch == '=') {
                read();
                ret.type = Token::GREATEREQ;
            } else
                ret.type = Token::GREATER;
            break;
        }
        case '=':
        {
            if (ch == '=') {
                read();
                ret.type = Token::EQUAL;
            } else
                ret.type = Token::ASSIGN;
        }
        break;
        case '"':
            ret.type = Token::STRING;
            ret.literal = read_string();
        break;
        default:
            ret.type = Token::UNKNOWN;
            ret.literal = _ch;
    }
    return ret;
}