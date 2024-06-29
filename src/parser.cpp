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

  if (look_ahead().get_type() == DEBUG) {
    NodeDebug *debug = parse_debug();
    if (!debug) {
      return NULL;
    }
    return new NodeStatement(debug);
  } else if (look_ahead().get_type() == LET) {
    NodeLet *let = parse_let();
    if (!let) {
      return NULL;
    }
    return new NodeStatement(let);
  }
  Error::invalid_syntax();
  return NULL;
}

NodeDebug *Parser::parse_debug() {
  consume();
  if (look_ahead().get_type() == INT_LIT) {
    NodeINT *INT = parse_int();
    if (!INT) {
      return NULL;
    }
    if (look_ahead().get_type() == SEMICOLON) {
      consume();
      return new NodeDebug(INT);
    } else {
      Error::invalid_syntax();
      return NULL;
    }
  } else if (look_ahead().get_type() == IDENTIFIER) {
    NodeIdentifier *identifier = parse_identifier();
    if (!identifier) {
      return NULL;
    }
    if (symbol_table->check(identifier->name) == Undeclared) {
      Error::undefined_variable(identifier->name);
      return NULL;
    }
    if (look_ahead().get_type() == SEMICOLON) {
      consume();
      return new NodeDebug(identifier);
    } else {
      Error::invalid_syntax();
      return NULL;
    }
  }
  Error::invalid_syntax();

  return NULL;
}

NodeLet *Parser::parse_let() {
  consume();
  if (look_ahead().get_type() == IDENTIFIER) {
    NodeIdentifier *identifier = parse_identifier();
    if (!identifier)
      return NULL;
    if (look_ahead().get_type() == EQUALS) {
      consume();
      if (look_ahead().get_type() == INT_LIT) {
        NodeINT *INT = parse_int();
        if (!INT) {
          return NULL;
        }
        if (look_ahead().get_type() == SEMICOLON) {
          consume();
          if (symbol_table->declare(identifier->name, INT->value) ==
              Redeclaration) {
            Error::redeclaration_variable(identifier->name);
            return NULL;
          }
          return new NodeLet(INT, identifier);
        } else {
          Error::invalid_syntax();
          return NULL;
        }
      } else {
        Error::invalid_syntax();
        return NULL;
      }
    } else {
      Error::invalid_syntax();
      return NULL;
    }
  }
  Error::invalid_syntax();
  return NULL;
}

NodeINT *Parser::parse_int() {
  NodeINT *INT = new NodeINT(look_ahead().get_value());
  consume();
  return INT;
}

NodeIdentifier *Parser::parse_identifier() {
  NodeIdentifier *identifier = new NodeIdentifier(look_ahead().get_body());
  consume();
  return identifier;
}
