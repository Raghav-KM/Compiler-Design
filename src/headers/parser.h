#ifndef PARSER_H
#define PARSER_H
#include "ast.h"
#include "lexer.h"
#include "utils.h"

class Parser {

private:
  vector<Token> token_stream;
  int ptr;
  SymbolTable *symbol_table;
  NodeStatement *parse_statement();
  NodeDebug *parse_debug();
  NodeLet *parse_let();
  NodeINT *parse_int();
  NodeIdentifier *parse_identifier();
  NodeExpression *parse_expression();
  NodeAdditiveExpression *parse_additive_expression();
  NodeMultiplicativeExpression *parse_multiplicative_expression();
  NodeAdditiveOperator *parse_additive_operator();
  NodeMultiplicativeOperator *parse_multiplicative_operator();

  void reset();

public:
  Parser();
  Token look_ahead();
  void consume();
  NodeProgram *parse_program(vector<Token> token_stream);
};

#endif