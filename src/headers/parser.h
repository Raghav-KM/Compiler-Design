#ifndef PARSER_H
#define PARSER_H
#include "ast.h"
#include "lexer.h"

class Parser {
private:
  vector<Token> token_stream;
  int ptr;
  NodeStatement *parse_statement();
  NodeDebug *parse_debug();
  NodeLet *parse_let();
  NodeINT *parse_int();
  NodeIdentifier *parse_identifier();

public:
  Parser(Lexical_Analyzer &lexer);
  Token look_ahead();
  void consume();

  NodeProgram *parse_program();
};

#endif