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
  if (NodeStatementList *stmt_list = parse_statement_list(END_OF_FILE)) {
    NodeProgram *program = new NodeProgram();
    program->stmt_list = stmt_list;
    return program;
  }
  return NULL;
}

NodeStatementList *Parser::parse_statement_list(TOKEN_TYPES END_TOKEN) {
  NodeStatementList *stmt_list = new NodeStatementList();
  while (look_ahead().get_type() != END_TOKEN &&
         look_ahead().get_type() != END_OF_FILE) {
    NodeStatement *stmt = parse_statement();
    if (!stmt)
      return NULL;
    stmt_list->stmts.push_back(stmt);
  }
  return stmt_list;
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
  cout << look_ahead().get_type() << " ";
  Error::invalid_syntax("Invalid TOKEN - Expected 'dbg' or 'let' or 'if'");
  return NULL;
}

NodeIf *Parser::parse_if() {
  consume();
  if (NodeAdditiveExpression *add_exp = parse_additive_expression()) {
    if (look_ahead().get_type() == BRACKET_OPEN_CURLY) {
      consume();
      if (NodeStatementList *stmt_list_if =
              parse_statement_list(BRACKET_CLOSE_CURLY)) {
        if (look_ahead().get_type() == BRACKET_CLOSE_CURLY) {
          consume();
          if (NodeStatementList *stmt_list_else = parse_else()) {
            return new NodeIf(add_exp, stmt_list_if, stmt_list_else);
          }
          return NULL;
        }
        Error::invalid_syntax("Expected '}'");
        return NULL;
      }
    }
    Error::invalid_syntax("Expected '{'");
  }
  return NULL;
}

NodeStatementList *Parser::parse_else() {
  if (look_ahead().get_type() == ELSE) {
    consume();
    if (look_ahead().get_type() == BRACKET_OPEN_CURLY) {
      consume();
      if (NodeStatementList *stmt_list_else =
              parse_statement_list(BRACKET_CLOSE_CURLY)) {
        if (look_ahead().get_type() == BRACKET_CLOSE_CURLY) {
          consume();
          return stmt_list_else;
        }
        Error::invalid_syntax("Expected '}'");
        return NULL;
      }
      return NULL;
    }
    Error::invalid_syntax("Expected '}'");
    return NULL;
  }
  Error::invalid_syntax("Expected 'else'");
  return NULL;
}

NodeDebug *Parser::parse_debug() {
  consume();
  if (NodeAdditiveExpression *add_exp = parse_additive_expression()) {
    if (look_ahead().get_type() == SEMICOLON) {
      consume();
      return new NodeDebug(add_exp);
    }
    Error::invalid_syntax("Missing ';'");
  }
  return NULL;
}

NodeLet *Parser::parse_let() {
  consume();
  if (NodeIdentifier *identifier = parse_identifier(REDECLARATION)) {
    if (look_ahead().get_type() == EQUALS) {
      consume();
      if (NodeAdditiveExpression *add_exp = parse_additive_expression()) {
        if (look_ahead().get_type() == SEMICOLON) {
          consume();
          return new NodeLet(identifier, add_exp);
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
