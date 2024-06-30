#include "./headers/parser.h"

// --- Parser --- //

// --- Private Member Functions --- //

// --- Public Member Functions --- //

Parser::Parser(Lexical_Analyzer &lexer) {
  token_stream = lexer.token_stream;
  ptr = 0;
  symbol_table = SymbolTable::get_instance();
}

Token Parser::look_ahead() {
  if (ptr == token_stream.size())
    return Token(END_FILE);
  return token_stream[ptr];
}

void Parser::consume() { ptr++; }

NodeProgram *Parser::parse_program() {
  NodeProgram *program = new NodeProgram();
  while (look_ahead().get_type() != END_FILE) {
    NodeStatement *stmt = parse_statement();
    if (!stmt)
      return NULL;
    program->stmts.push_back(stmt);
  }
  return program;
}

NodeStatement *Parser::parse_statement() {
  if (NodeDebug *debug = parse_debug()) {
    return new NodeStatement(debug);
  } else if (NodeLet *let = parse_let()) {
    return new NodeStatement(let);
  }
  return NULL;
}

NodeDebug *Parser::parse_debug() {
  if (look_ahead().get_type() == DEBUG) {
    consume();
    if (NodeExpression *expression = parse_expression()) {
      if (look_ahead().get_type() == SEMICOLON) {
        consume();
        return new NodeDebug(expression);
      }
    }
  }
  return NULL;
}

NodeLet *Parser::parse_let() {
  if (look_ahead().get_type() == LET) {
    consume();
    if (NodeIdentifier *identifier = parse_identifier()) {
      if (look_ahead().get_type() == EQUALS) {
        consume();
        if (NodeExpression *expression = parse_expression()) {
          if (look_ahead().get_type() == SEMICOLON) {
            consume();
            return new NodeLet(identifier, expression);
          }
        }
      }
    }
  }
  return NULL;
}

NodeExpression *Parser::parse_expression() {
  if (NodeINT *INT = parse_int()) {
    return new NodeExpression(INT);
  }
  if (NodeIdentifier *identifier = parse_identifier()) {
    return new NodeExpression(identifier);
  }
  return NULL;
}

NodeINT *Parser::parse_int() {
  if (look_ahead().get_type() == INT_LIT) {
    NodeINT *INT = new NodeINT(look_ahead().get_value());
    consume();
    return INT;
  }
  return NULL;
}

NodeIdentifier *Parser::parse_identifier() {
  if (look_ahead().get_type() == IDENTIFIER) {
    NodeIdentifier *identifier = new NodeIdentifier(look_ahead().get_body());
    consume();
    return identifier;
  }
  return NULL;
}
