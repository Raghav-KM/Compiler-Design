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
  NodeAssign *parse_assign(bool check_semicolon = true);

  NodeIf *parse_if();

  NodeFor *parse_for();

  NodeComparativeExpression *parse_comparative_expression();
  NodeAdditiveExpression *parse_additive_expression();
  NodeMultiplicativeExpression *parse_multiplicative_expression();
  NodeExpression *parse_expression();

  NodeAdditiveOperator *parse_additive_operator();
  NodeMultiplicativeOperator *parse_multiplicative_operator();
  NodeComparativeOperator *parse_comparative_operator();

  NodeINT *parse_int();
  NodeCHAR *parse_char();
  NodeIdentifier *parse_identifier(RESULT_TYPE check_type = UNDECLARED);

  void reset();

public:
  Parser();
  Token look_ahead();
  void consume();
  NodeProgram *parse_program(vector<Token> token_stream);
};

#endif