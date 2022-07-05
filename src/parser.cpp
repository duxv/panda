#include "parser.hpp"
#include <iostream>

using namespace AST;

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
    pos = m_lexer.get_pos();
    tok = m_lexer.nextToken();
}

void Parser::error(std::size_t pos, std::string msg) {
    FilePos file_pos = get_row_and_col(pos, m_lexer.get_input());

    errors.push_back({file_pos, msg});
    if (errors.size() > MAX_ERRORS)
        throw Bailout{};
}