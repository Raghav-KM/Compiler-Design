#include "./headers/parser.h"

// --- Parser --- //

// --- Private Member Functions --- //

// --- Public Member Functions --- //

Parser::Parser() {
  ptr = 0;
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
  } else if (look_ahead().get_type() == LET) {
    if (NodeLet *let = parse_let()) {
      return new NodeStatement(let);
    }
    return NULL;
  } else if (look_ahead().get_type() == IF) {
    if (NodeIf *IF = parse_if()) {
      return new NodeStatement(IF);
    }
    return NULL;
  } else if (look_ahead().get_type() == IDENTIFIER) {
    if (NodeAssign *assign = parse_assign()) {
      return new NodeStatement(assign);
    }
    return NULL;
  } else if (look_ahead().get_type() == FOR) {
    if (NodeFor *FOR = parse_for()) {
      return new NodeStatement(FOR);
    }
    return NULL;
  }

  Error::invalid_syntax("Invalid TOKEN - Expected 'dbg' or 'let' or 'if' or "
                        "'for' or 'IDENTIFIER'");
  return NULL;
}

NodeDebug *Parser::parse_debug() {
  consume();
  if (NodeComparativeExpression *comp_exp = parse_comparative_expression()) {
    if (look_ahead().get_type() == SEMICOLON) {
      consume();
      return new NodeDebug(comp_exp);
    }
    Error::invalid_syntax("Missing ';'");
  } else if (NodeCHAR *CHAR = parse_char()) {
    if (look_ahead().get_type() == SEMICOLON) {
      consume();
      return new NodeDebug(CHAR);
    }
    Error::invalid_syntax("Missing ';'");
  }
  return NULL;
}

NodeIf *Parser::parse_if() {
  consume();
  if (look_ahead().get_type() != BRACKET_OPEN) {
    Error::invalid_syntax("Expected '('");
    return NULL;
  }
  consume();

  NodeComparativeExpression *comp_exp = parse_comparative_expression();
  if (comp_exp == NULL) {
    return NULL;
  }

  if (look_ahead().get_type() != BRACKET_CLOSE) {
    Error::invalid_syntax("Expected ')'");
    return NULL;
  }
  consume();

  if (look_ahead().get_type() != BRACKET_OPEN_CURLY) {
    Error::invalid_syntax("Expected '{'");
    return NULL;
  }
  consume();

  NodeStatementList *stmt_list_if = parse_statement_list(BRACKET_CLOSE_CURLY);
  if (stmt_list_if == NULL) {
    return NULL;
  }

  if (look_ahead().get_type() != BRACKET_CLOSE_CURLY) {
    Error::invalid_syntax("Expected '}'");
    return NULL;
  }
  consume();

  if (look_ahead().get_type() != ELSE) {
    return new NodeIf(comp_exp, stmt_list_if, NULL);
  }
  consume();

  if (look_ahead().get_type() != BRACKET_OPEN_CURLY) {
    Error::invalid_syntax("Expected '{'");
    return NULL;
  }
  consume();

  NodeStatementList *stmt_list_else = parse_statement_list(BRACKET_CLOSE_CURLY);
  if (stmt_list_else == NULL) {
    return NULL;
  }

  if (look_ahead().get_type() != BRACKET_CLOSE_CURLY) {
    Error::invalid_syntax("Expected '}'");
    return NULL;
  }
  consume();

  return new NodeIf(comp_exp, stmt_list_if, stmt_list_else);
}

NodeLet *Parser::parse_let() {
  consume();
  if (NodeIdentifier *identifier = parse_identifier(REDECLARATION)) {
    if (look_ahead().get_type() == EQUALS) {
      consume();
      if (NodeComparativeExpression *comp_exp =
              parse_comparative_expression()) {
        if (look_ahead().get_type() == SEMICOLON) {
          consume();
          return new NodeLet(identifier, comp_exp);
        }
        Error::invalid_syntax("Missing ';'");
        return NULL;
      }
    }
    Error::invalid_syntax("Invalid TOKEN - Expected '='");
    return NULL;
  }
  return NULL;
}

NodeFor *Parser::parse_for() {
  consume();
  if (look_ahead().get_type() == BRACKET_OPEN) {
    consume();
    if (NodeLet *let = parse_let()) {
      if (NodeComparativeExpression *comp_exp =
              parse_comparative_expression()) {
        if (look_ahead().get_type() == SEMICOLON) {
          consume();
          if (NodeAssign *assign = parse_assign(false)) {
            if (look_ahead().get_type() == BRACKET_CLOSE) {
              consume();
              if (look_ahead().get_type() == BRACKET_OPEN_CURLY) {
                consume();
                if (NodeStatementList *stmt_list =
                        parse_statement_list(BRACKET_CLOSE_CURLY)) {
                  if (look_ahead().get_type() == BRACKET_CLOSE_CURLY) {
                    consume();
                    return new NodeFor(let, comp_exp, assign, stmt_list);
                  }
                  Error::invalid_syntax("Missing '}'");
                  return NULL;
                }
                return NULL;
              }
              Error::invalid_syntax("Missing '{'");
              return NULL;
            }
            Error::invalid_syntax("Missing '*)'");
            return NULL;
          }
        }
        Error::invalid_syntax("Missing ';'");
        return NULL;
      }
      return NULL;
    }
    return NULL;
  }
  Error::invalid_syntax("Missing '('");
  return NULL;
}

NodeAssign *Parser::parse_assign(bool check_semicolon) {
  if (NodeIdentifier *identifier = parse_identifier(UNDECLARED)) {
    if (look_ahead().get_type() == EQUALS) {
      consume();
      if (NodeComparativeExpression *comp_exp =
              parse_comparative_expression()) {
        if (!check_semicolon) {
          return new NodeAssign(identifier, comp_exp);
        }
        if (look_ahead().get_type() == SEMICOLON) {
          consume();
          return new NodeAssign(identifier, comp_exp);
        }
        Error::invalid_syntax("Missing ';'");
        return NULL;
      }
    }
    Error::invalid_syntax("Invalid TOKEN - Expected '='");
    return NULL;
  }
  return NULL;
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
  if (NodeMultiplicativeExpression *mul_exp =
          parse_multiplicative_expression()) {
    NodeAdditiveExpression *add_exp = new NodeAdditiveExpression(mul_exp);
    while (NodeAdditiveOperator *add_op = parse_additive_operator()) {
      if (NodeMultiplicativeExpression *new_mul_exp =
              parse_multiplicative_expression()) {
        add_exp = new NodeAdditiveExpression(add_exp, add_op, new_mul_exp);
      } else {
        return NULL;
      }
    }
    return add_exp;
  }
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
  if (look_ahead().get_type() == INT_LIT) {
    if (NodeINT *INT = parse_int()) {
      return new NodeExpression(INT);
    }
    return NULL;
  }
  if (look_ahead().get_type() == IDENTIFIER) {
    if (NodeIdentifier *identifier = parse_identifier()) {
      return new NodeExpression(identifier);
    }
    return NULL;
  }
  Error::invalid_syntax("Invalid TOKEN - Expected 'INT_LIT' or 'IDENTIFIER'");
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

NodeINT *Parser::parse_int() {
  NodeINT *INT = new NodeINT(stoi(look_ahead().get_body()));
  consume();
  return INT;
}

NodeCHAR *Parser::parse_char() {
  NodeCHAR *CHAR = new NodeCHAR((int)look_ahead().get_body()[0]);
  consume();
  return CHAR;
}

NodeIdentifier *Parser::parse_identifier(RESULT_TYPE check_type) {

  string symbol_name = look_ahead().get_body();
  if (symbol_table->exists(symbol_name) == UNDECLARED &&
      check_type == UNDECLARED) {
    Error::undefined_variable(symbol_name);
    return NULL;
  }
  if (symbol_table->declare(symbol_name) == REDECLARATION &&
      check_type == REDECLARATION) {
    Error::redeclaration_variable(symbol_name);
    return NULL;
  }

  NodeIdentifier *identifier = new NodeIdentifier(symbol_name);
  consume();
  return identifier;
}
