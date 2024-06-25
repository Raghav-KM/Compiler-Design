#ifndef PARSER_H
#define PARSER_H
#include "lexer.h"

class Parser {
private:
  vector<Token> token_stream;
  int ptr;

public:
  Parser(Lexical_Analyzer &lexer);
  Token look_ahead();
  void consume();

  bool parse_program();
  bool parse_statement();
  bool parse_debug();
  bool parse_let();
};

#endif