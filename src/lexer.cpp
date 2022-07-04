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
bool is_digit(char c) {
    return ('0' <= c && c <= '9');
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
                error_handler(offset, "string literal not terminated");
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

std::pair<Token, std::string> Lexer::read_number() {
    int offs = offset;
    std::pair<Token, std::string> ret;
    ret.first = Token::INT; // Assuming it is an int
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
        ret.first = Token::FLOAT;
        read();
        read_digits(base);
    }

    // exponent
    if (ch == 'e' || ch == 'E' || ch == 'p' || ch == 'P') {
        ret.first = Token::FLOAT;
        read();
        if (ch == '-' || ch == '+') {
            read();
        }
        int offs = offset;
        read_digits(10);
        if (offs == offset) {
            error_handler(offset, "exponent has no digits");
        }
    }
    ret.second = input.substr(offs, offset-offs);
    return ret;
}

std::pair<Token, std::string> Lexer::nextToken() {
    skip_whitespace();
    char _ch = ch;
    std::pair<Token, std::string> ret;

    if (is_identifier_start(ch)) {
        ret.second = read_ident(); 
        ret.first = lookup_keyword(ret.second);
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
            ret.first = Token::ENDMARKER;
            break;
        case '\n':
            while(ch == '\n') read(); // skip all the newlines
            ret.first = Token::NEWLINE;
            break;
        case ',':
            ret.first = Token::COMMA;
            break;
        case '+': 
            ret.first = Token::ADD;
            break;
        case '-': 
            ret.first = Token::SUB;
            break;
        case '*': 
            ret.first = Token::MUL;
            break;
        case '/':
        {
            if (ch == '/' || ch == '*') {
                if (ch == '/')
                    while(ch != '\n' && ch != 0)
                        read();
                else if (ch == '*')
                    while (ch != 0) {
                        read();
                        if (ch == '*' && peek() == '/') {
                            read();
                            read();
                        }
                    }
                return nextToken();
            } else { // Token::DIV
                ret.first = Token::DIV;
                break;
            }
        }
        case '%': 
            ret.first = Token::REM;
            break;
        case ';':
            ret.first = Token::NEWLINE; // We treat ';' as a newline.
            break;
        case ':':
            ret.first = Token::COLON;
            break;
        case '(':
            ret.first = Token::LPAREN;
            break;
        case ')':
            ret.first = Token::RPAREN;
            break;
        case '[':
            ret.first = Token::LBRACKET;
            break;
        case ']':
            ret.first = Token::RBRACKET;
            break;
        case '{':
            ret.first = Token::LBRACE;
            break;
        case '}':
            ret.first = Token::RBRACE;
            break;
        case '.':
            ret.first = Token::DOT;
            break;
        case '&':
            if (ch != '&') {
                std::cout << ch << std::endl;
                ret.first = Token::UNKNOWN;
                ret.second = _ch;
            } else {
                read();
                ret.first = Token::AND;
            }
            break;
        case '|':
            if (ch != '|') {
                ret.first = Token::UNKNOWN;
                ret.second = _ch;
            } else {
                read();
                ret.first = Token::OR;
            }
            break;
        case '!':
        {
            if (ch == '=') {
                read();
                ret.first = Token::NOTEQ;
            } else
                ret.first = Token::NOT;
            
            break;
        }
        case '<':
        {
            if (ch == '=') {
                read();
                ret.first = Token::LESSEQ;
            } else
                ret.first = Token::LESS;
            break;
        }
        case '>':
        {
            if (ch == '=') {
                read();
                ret.first = Token::GREATEREQ;
            } else
                ret.first = Token::GREATER;
            break;
        }
        case '=':
        {
            if (ch == '=') {
                read();
                ret.first = Token::EQUAL;
            } else
                ret.first = Token::ASSIGN;
        }
        break;
        case '"':
            ret.first = Token::STRING;
            ret.second = read_string();
        break;
        default:
            ret.first = Token::UNKNOWN;
            ret.second.push_back(_ch);
    }
    return ret;
}

std::size_t Lexer::get_pos() { return offset; }