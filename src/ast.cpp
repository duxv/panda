#include "ast.hpp" 

AST::FilePos AST::FilePos_from_offset(std::size_t offs, std::string_view input) {
    AST::FilePos pos {1, 1};

    for (int i = 0; i <= offs; i++) {
        if (input[i] == '\n') {
            pos.row += 1;
            pos.col = 1;
        } else {
            pos.col += 1;
        }
    }

    return pos;
}