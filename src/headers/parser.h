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

  NodeStatementList *parse_statement_list(TOKEN_TYPES END_TOKEN);
  NodeStatement *parse_statement();

  NodeDebug *parse_debug();
  NodeLet *parse_let();

  NodeIf *parse_if();
  NodeStatementList *parse_else();

  NodeINT *parse_int();
  NodeIdentifier *parse_identifier(RESULT_TYPE check_type = UNDECLARED);

  NodeComparativeExpression *parse_comparative_expression();
  NodeAdditiveExpression *parse_additive_expression();
  NodeMultiplicativeExpression *parse_multiplicative_expression();
  NodeExpression *parse_expression();

  NodeAdditiveOperator *parse_additive_operator();
  NodeMultiplicativeOperator *parse_multiplicative_operator();
  NodeComparativeOperator *parse_comparative_operator();

  void reset();

public:
  Parser();
  Token look_ahead();
  void consume();
  NodeProgram *parse_program(vector<Token> token_stream);
};

#endif