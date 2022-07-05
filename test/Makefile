all: lexer_test parser_test


lexer_test: lexer_test.cpp ../src/lexer.cpp ../src/token.cpp
	g++ $^ -o $@ -std=c++2a

parser_test: parser_test.cpp ../src/parser.cpp ../src/token.cpp ../src/lexer.cpp
	g++ $^ -o $@ -std=c++2a