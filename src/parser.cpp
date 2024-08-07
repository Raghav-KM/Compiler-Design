#include "./headers/parser.h"

// --- Parser --- //

// --- Private Member Functions --- //

// --- Public Member Functions --- //

Parser::Parser() {
  ptr = 0;
  scope = 0;
  symbol_table = SymbolTable::get_instance();
}

void Parser::reset() {
  ptr = 0;
  symbol_table = SymbolTable::get_instance();
  symbol_table->reset();
  this->token_stream.clear();
}

Token Parser::look_ahead() {
  if (ptr == token_stream.size())
    return Token(END_OF_FILE);
  return token_stream[ptr];
}

void Parser::consume() { ptr++; }

void Parser::increment_scope() { scope++; }

void Parser::decrement_scope() {
  symbol_table->clear_scope(scope);
  scope--;
}

NodeProgram *Parser::parse_program(vector<Token> token_stream) {
  reset();
  this->token_stream = token_stream;

  NodeStatementList *stmt_list = parse_statement_list(END_OF_FILE);
  if (stmt_list == NULL)
    return NULL;

  return new NodeProgram(stmt_list);
}

NodeStatementList *Parser::parse_statement_list(TOKEN_TYPES END_TOKEN) {
  vector<NodeStatement *> stmt_list;
  while (look_ahead().get_type() != END_TOKEN &&
         look_ahead().get_type() != END_OF_FILE) {
    NodeStatement *stmt = parse_statement();
    if (!stmt)
      return NULL;
    stmt_list.push_back(stmt);
  }
  return new NodeStatementList(stmt_list);
}

NodeStatement *Parser::parse_statement() {
  if (look_ahead().get_type() == DEBUG) {
    if (NodeDebug *debug = parse_debug()) {
      return new NodeStatement(debug);
    }
    return NULL;
  }
  if (look_ahead().get_type() == LET) {

    if (NodeLet *let = parse_let()) {
      return new NodeStatement(let);
    }
    return NULL;
  }
  if (look_ahead().get_type() == IF) {
    if (NodeIf *IF = parse_if()) {
      return new NodeStatement(IF);
    }
    return NULL;
  }
  if (look_ahead().get_type() == IDENTIFIER) {
    if (NodeAssign *assign = parse_assign()) {
      return new NodeStatement(assign);
    }
    return NULL;
  }
  if (look_ahead().get_type() == FOR) {
    increment_scope();
    if (NodeFor *FOR = parse_for()) {
      return new NodeStatement(FOR);
    }
    decrement_scope();
    return NULL;
  }
  if (look_ahead().get_type() == FUNCTION) {
    increment_scope();
    if (NodeFunction *function = parse_function()) {
      return new NodeStatement(function);
    }
    decrement_scope();
    return NULL;
  }
  if (look_ahead().get_type() == CALL) {
    if (NodeFunctionCall *function_call = parse_function_call()) {
      return new NodeStatement(function_call);
    }
    return NULL;
  }

  Error::invalid_syntax("Invalid TOKEN - Expected 'dbg' or 'let' or 'if' or "
                        "'for' or 'IDENTIFIER' or 'function' or 'call'",
                        look_ahead().line_no, look_ahead().token_no);
  return NULL;
}

NodeDebug *Parser::parse_debug() {
  consume();

  if (NodeComparativeExpression *comp_exp = parse_comparative_expression()) {
    if (look_ahead().get_type() == SEMICOLON) {
      consume();
      return new NodeDebug(comp_exp);
    }
    Error::invalid_syntax("Missing ';'", look_ahead().line_no,
                          look_ahead().token_no);
  }
  return NULL;
}

NodeIf *Parser::parse_if() {
  consume();
  if (look_ahead().get_type() != BRACKET_OPEN) {
    Error::invalid_syntax("Expected '('", look_ahead().line_no,
                          look_ahead().token_no);
    return NULL;
  }
  consume();

  NodeComparativeExpression *comp_exp = parse_comparative_expression();
  if (comp_exp == NULL) {
    return NULL;
  }

  if (look_ahead().get_type() != BRACKET_CLOSE) {
    Error::invalid_syntax("Expected ')'", look_ahead().line_no,
                          look_ahead().token_no);
    return NULL;
  }
  consume();

  if (look_ahead().get_type() != BRACKET_OPEN_CURLY) {
    Error::invalid_syntax("Expected '{'", look_ahead().line_no,
                          look_ahead().token_no);
    return NULL;
  }
  consume();

  increment_scope();
  NodeStatementList *stmt_list_if = parse_statement_list(BRACKET_CLOSE_CURLY);
  decrement_scope();

  if (stmt_list_if == NULL) {
    return NULL;
  }

  if (look_ahead().get_type() != BRACKET_CLOSE_CURLY) {
    Error::invalid_syntax("Expected '}'", look_ahead().line_no,
                          look_ahead().token_no);
    return NULL;
  }
  consume();

  if (look_ahead().get_type() != ELSE) {
    return new NodeIf(comp_exp, stmt_list_if, NULL);
  }
  consume();

  if (look_ahead().get_type() != BRACKET_OPEN_CURLY) {
    Error::invalid_syntax("Expected '{'", look_ahead().line_no,
                          look_ahead().token_no);
    return NULL;
  }
  consume();

  increment_scope();
  NodeStatementList *stmt_list_else = parse_statement_list(BRACKET_CLOSE_CURLY);
  decrement_scope();
  if (stmt_list_else == NULL) {
    return NULL;
  }

  if (look_ahead().get_type() != BRACKET_CLOSE_CURLY) {
    Error::invalid_syntax("Expected '}'", look_ahead().line_no,
                          look_ahead().token_no);
    return NULL;
  }
  consume();

  return new NodeIf(comp_exp, stmt_list_if, stmt_list_else);
}

NodeLet *Parser::parse_let() {
  consume();
  NodeIdentifier *identifier = parse_identifier(REDECLARATION);
  if (identifier == NULL) {
    return NULL;
  }
  if (look_ahead().get_type() != COLON) {
    Error::invalid_syntax("Expected ':'", look_ahead().line_no,
                          look_ahead().token_no);
    return NULL;
  }
  consume();
  if (look_ahead().get_type() == DT_CHAR || look_ahead().get_type() == DT_INT) {
    if (look_ahead().get_type() == DT_CHAR) {
      identifier->type = (DATA_TYPES)CHAR;
    }
    if (look_ahead().get_type() == DT_INT) {
      identifier->type = (DATA_TYPES)INT;
    }
    symbol_table->set_datatype(identifier->name, identifier->type, scope);
  } else {
    Error::invalid_syntax("Expected a valid DATATYPE", look_ahead().line_no,
                          look_ahead().token_no);
    return NULL;
  }
  consume();

  if (look_ahead().get_type() != EQUALS) {
    Error::invalid_syntax("Invalid TOKEN - Expected '='", look_ahead().line_no,
                          look_ahead().token_no);
    return NULL;
  }
  consume();
  NodeComparativeExpression *comp_exp = parse_comparative_expression();
  if (comp_exp == NULL) {
    return NULL;
  }
  if (look_ahead().get_type() != SEMICOLON) {
    Error::invalid_syntax("Missing ';'", look_ahead().line_no,
                          look_ahead().token_no);
    return NULL;
  }
  consume();
  return new NodeLet(identifier, comp_exp);
}

NodeFor *Parser::parse_for() {

  consume();
  if (look_ahead().get_type() != BRACKET_OPEN) {
    Error::invalid_syntax("Missing '('", look_ahead().line_no,
                          look_ahead().token_no);
    return NULL;
  }

  consume();
  NodeLet *let = parse_let();
  if (!let) {
    return NULL;
  }

  NodeComparativeExpression *comp_exp = parse_comparative_expression();
  if (!comp_exp) {
    return NULL;
  }

  if (look_ahead().get_type() != SEMICOLON) {
    Error::invalid_syntax("Missing ';'", look_ahead().line_no,
                          look_ahead().token_no);
    return NULL;
  }

  consume();
  NodeAssign *assign = parse_assign(false);
  if (!assign) {
    return NULL;
  }

  if (look_ahead().get_type() != BRACKET_CLOSE) {
    Error::invalid_syntax("Missing ')'", look_ahead().line_no,
                          look_ahead().token_no);
    return NULL;
  }

  consume();
  if (look_ahead().get_type() != BRACKET_OPEN_CURLY) {
    Error::invalid_syntax("Missing '{'", look_ahead().line_no,
                          look_ahead().token_no);
    return NULL;
  }

  consume();
  NodeStatementList *stmt_list = parse_statement_list(BRACKET_CLOSE_CURLY);
  if (!stmt_list) {
    return NULL;
  }

  if (look_ahead().get_type() != BRACKET_CLOSE_CURLY) {
    Error::invalid_syntax("Missing '}'", look_ahead().line_no,
                          look_ahead().token_no);
    return NULL;
  }
  consume();
  return new NodeFor(let, comp_exp, assign, stmt_list);
}

NodeAssign *Parser::parse_assign(bool check_semicolon) {
  NodeIdentifier *identifier = parse_identifier(UNDECLARED);
  if (!identifier) {
    return NULL;
  }

  if (look_ahead().get_type() != EQUALS) {
    Error::invalid_syntax("Invalid TOKEN - Expected '='", look_ahead().line_no,
                          look_ahead().token_no);
    return NULL;
  }

  consume();
  NodeComparativeExpression *comp_exp = parse_comparative_expression();
  if (!comp_exp) {
    return NULL;
  }

  if (!check_semicolon) {
    return new NodeAssign(identifier, comp_exp);
  }

  if (look_ahead().get_type() != SEMICOLON) {
    Error::invalid_syntax("Missing ';'", look_ahead().line_no,
                          look_ahead().token_no);
    return NULL;
  }

  consume();
  return new NodeAssign(identifier, comp_exp);
}

NodeFunction *Parser::parse_function() {
  consume();

  if (look_ahead().get_type() != IDENTIFIER) {
    Error::invalid_syntax("Expected 'IDENTIFIER'", look_ahead().line_no,
                          look_ahead().token_no);
    return NULL;
  }
  NodeIdentifier *identifier = parse_identifier(REDECLARATION);
  if (identifier == NULL)
    return NULL;

  if (look_ahead().get_type() != BRACKET_OPEN) {
    Error::invalid_syntax("Expected 'BRACKET_OPEN'", look_ahead().line_no,
                          look_ahead().token_no);
    return NULL;
  }
  consume();
  if (look_ahead().get_type() != BRACKET_CLOSE) {
    Error::invalid_syntax("Expected 'BRACKET_CLOSE'", look_ahead().line_no,
                          look_ahead().token_no);
    return NULL;
  }
  consume();
  if (look_ahead().get_type() != BRACKET_OPEN_CURLY) {
    Error::invalid_syntax("Expected 'BRACKET_OPEN_CURLY'", look_ahead().line_no,
                          look_ahead().token_no);
    return NULL;
  }
  consume();

  NodeStatementList *stmt_list = parse_statement_list(BRACKET_CLOSE_CURLY);
  if (stmt_list == NULL)
    return NULL;

  if (look_ahead().get_type() != BRACKET_CLOSE_CURLY) {
    Error::invalid_syntax("Expected 'BRACKET_CLOSE_CURLY'",
                          look_ahead().line_no, look_ahead().token_no);
    return NULL;
  }
  consume();

  return new NodeFunction(identifier->name, stmt_list);
}

NodeFunctionCall *Parser::parse_function_call() {
  consume();

  if (look_ahead().get_type() != IDENTIFIER) {
    Error::invalid_syntax("Expected 'IDENTIFIER'", look_ahead().line_no,
                          look_ahead().token_no);
    return NULL;
  }
  NodeIdentifier *identifier = parse_identifier(UNDECLARED);
  if (identifier == NULL)
    return NULL;

  if (look_ahead().get_type() != BRACKET_OPEN) {
    Error::invalid_syntax("Expected 'BRACKET_OPEN'", look_ahead().line_no,
                          look_ahead().token_no);
    return NULL;
  }
  consume();
  if (look_ahead().get_type() != BRACKET_CLOSE) {
    Error::invalid_syntax("Expected 'BRACKET_CLOSE'", look_ahead().line_no,
                          look_ahead().token_no);
    return NULL;
  }
  consume();
  if (look_ahead().get_type() != SEMICOLON) {
    Error::invalid_syntax("Missing ';'", look_ahead().line_no,
                          look_ahead().token_no);
    return NULL;
  }
  consume();

  return new NodeFunctionCall(identifier->name);
}

NodeComparativeExpression *Parser::parse_comparative_expression() {
  if (NodeAdditiveExpression *add_exp = parse_additive_expression()) {
    NodeComparativeExpression *comp_exp =
        new NodeComparativeExpression(add_exp);
    while (NodeComparativeOperator *comp_op = parse_comparative_operator()) {
      if (NodeAdditiveExpression *new_add_exp = parse_additive_expression()) {
        comp_exp =
            new NodeComparativeExpression(comp_exp, comp_op, new_add_exp);
      } else {
        return NULL;
      }
    }
    return comp_exp;
  }
  return NULL;
}

NodeAdditiveExpression *Parser::parse_additive_expression() {
  if (NodeNegativeExpression *neg_exp = parse_negative_expression()) {
    NodeAdditiveExpression *add_exp = new NodeAdditiveExpression(neg_exp);
    while (NodeAdditiveOperator *add_op = parse_additive_operator()) {
      if (NodeNegativeExpression *new_neg_exp = parse_negative_expression()) {
        add_exp = new NodeAdditiveExpression(add_exp, add_op, new_neg_exp);
      } else {
        return NULL;
      }
    }
    return add_exp;
  }
  return NULL;
}

NodeNegativeExpression *Parser::parse_negative_expression() {
  if (look_ahead().get_type() == SUB) {
    consume();
    if (NodeNegativeExpression *neg_exp = parse_negative_expression()) {
      return new NodeNegativeExpression(neg_exp);
    }
    return NULL;
  } else {
    if (NodeMultiplicativeExpression *mul_exp =
            parse_multiplicative_expression()) {
      return new NodeNegativeExpression(mul_exp);
    }
    return NULL;
  }
}

NodeMultiplicativeExpression *Parser::parse_multiplicative_expression() {
  if (NodeExpression *exp = parse_expression()) {
    NodeMultiplicativeExpression *mul_exp =
        new NodeMultiplicativeExpression(exp);
    while (NodeMultiplicativeOperator *mul_op =
               parse_multiplicative_operator()) {
      if (NodeExpression *new_exp = parse_expression()) {
        mul_exp = new NodeMultiplicativeExpression(mul_exp, mul_op, new_exp);
      } else {
        return NULL;
      }
    }
    return mul_exp;
  }
  return NULL;
}

NodeExpression *Parser::parse_expression() {
  if (look_ahead().get_type() == INT_LIT ||
      look_ahead().get_type() == CHAR_LIT) {

    if (NodeLiteral *literal = parse_literal()) {
      return new NodeExpression(literal);
    }
    return NULL;
  }
  if (look_ahead().get_type() == IDENTIFIER) {
    if (NodeIdentifier *identifier = parse_identifier()) {
      return new NodeExpression(identifier);
    }
    return NULL;
  }
  if (look_ahead().get_type() == BRACKET_OPEN) {
    consume();
    NodeComparativeExpression *comp_exp = parse_comparative_expression();
    if (comp_exp == NULL) {
      return NULL;
    }
    if (look_ahead().get_type() != BRACKET_CLOSE) {
      Error::invalid_syntax("Exprected '('", look_ahead().line_no,
                            look_ahead().token_no);
      return NULL;
    }
    consume();
    return new NodeExpression(comp_exp);
  }

  Error::invalid_syntax(
      "Invalid TOKEN - Expected 'INT_LIT' or 'IDENTIFIER' or '('",
      look_ahead().line_no, look_ahead().token_no);
  return NULL;
}

NodeComparativeOperator *Parser::parse_comparative_operator() {
  if (look_ahead().get_type() == EQUAL_EQUALS) {
    consume();
    return new NodeComparativeOperator("==");
  }
  if (look_ahead().get_type() == GREATER) {
    consume();
    return new NodeComparativeOperator(">");
  }
  if (look_ahead().get_type() == LESS) {
    consume();
    return new NodeComparativeOperator("<");
  }
  if (look_ahead().get_type() == GREATER_EQUALS) {
    consume();
    return new NodeComparativeOperator(">=");
  }
  if (look_ahead().get_type() == LESS_EQUALS) {
    consume();
    return new NodeComparativeOperator("<=");
  }
  if (look_ahead().get_type() == NOT_EQUALS) {
    consume();
    return new NodeComparativeOperator("!=");
  }
  return NULL;
}

NodeAdditiveOperator *Parser::parse_additive_operator() {
  if (look_ahead().get_type() == ADD) {
    consume();
    return new NodeAdditiveOperator('+');
  }
  if (look_ahead().get_type() == SUB) {
    consume();
    return new NodeAdditiveOperator('-');
  }
  return NULL;
}

NodeMultiplicativeOperator *Parser::parse_multiplicative_operator() {
  if (look_ahead().get_type() == MUL) {
    consume();
    return new NodeMultiplicativeOperator('*');
  }
  if (look_ahead().get_type() == DIV) {
    consume();
    return new NodeMultiplicativeOperator('/');
  }
  return NULL;
}

NodeIdentifier *Parser::parse_identifier(RESULT_TYPE check_type) {

  string symbol_name = look_ahead().get_body();
  /*
  if (symbol_table->exists(symbol_name) == UNDECLARED &&
      check_type == UNDECLARED) {
    Error::undefined_variable(symbol_name, look_ahead().line_no,
                              look_ahead().token_no);
    return NULL;
  }
  if (symbol_table->declare(symbol_name) == REDECLARATION &&
      check_type == REDECLARATION) {
    Error::redeclaration_variable(symbol_name, look_ahead().line_no,
                                  look_ahead().token_no);
    return NULL;
  }
  */

  int present_scope = scope;
  if (check_type == UNDECLARED &&
      symbol_table->exists(symbol_name, scope, present_scope) == UNDECLARED) {
    Error::undefined_variable(symbol_name, look_ahead().line_no,
                              look_ahead().token_no);
    return NULL;
  }

  if (check_type == REDECLARATION &&
      symbol_table->declare(symbol_name, scope) == REDECLARATION) {
    Error::redeclaration_variable(symbol_name, look_ahead().line_no,
                                  look_ahead().token_no);
    return NULL;
  }

  NodeIdentifier *identifier = new NodeIdentifier(symbol_name, scope);
  if (check_type == UNDECLARED) {
    identifier->type = symbol_table->get_datatype(symbol_name, present_scope);
    identifier->scope = present_scope;
  }

  consume();
  return identifier;
}

NodeLiteral *Parser::parse_literal() {
  if (look_ahead().get_type() == INT_LIT) {
    int value = stoi(look_ahead().get_body());
    consume();
    return new NodeLiteral(value, INT);
  }
  if (look_ahead().get_type() == CHAR_LIT) {
    int value = look_ahead().get_body()[0];
    consume();
    return new NodeLiteral(value, CHAR);
  }
  return NULL;
}
