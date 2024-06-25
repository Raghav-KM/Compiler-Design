#include "./headers/parser.h"

// --- Parser --- //

// --- Private Member Functions --- //

// --- Public Member Functions --- //

Parser::Parser(Lexical_Analyzer &lexer) {
  token_stream = lexer.token_stream;
  ptr = 0;
}

Token Parser::look_ahead() {
  if (ptr == token_stream.size())
    return Token(END_FILE);
  return token_stream[ptr];
}

void Parser::consume() { ptr++; }

bool Parser::parse_program() {
  while (look_ahead().get_type() != END_FILE) {
    if (!parse_statement())
      return false;
  }
  return true;
}

bool Parser::parse_statement() {
  if (token_stream[ptr].get_type() == DEBUG) {
    consume();
    return parse_debug();
  } else if (token_stream[ptr].get_type() == LET) {
    consume();
    return parse_let();
  } else {
    return false;
  }
}

bool Parser::parse_debug() {
  if (look_ahead().get_type() == INT_LIT) {
    consume();
    if (look_ahead().get_type() == SEMICOLON) {
      consume();
      return true;
    } else {
      cerr << "Error: ';' Missing" << endl;
      return false;
    }
  } else {
    cerr << "Error: Invalid DEBUG Statement" << endl;
    return false;
  }
}

bool Parser::parse_let() {
  if (look_ahead().get_type() == IDENTIFIER) {
    consume();
    if (look_ahead().get_type() == EQUALS) {
      consume();
      if (look_ahead().get_type() == INT_LIT) {
        consume();
        if (look_ahead().get_type() == SEMICOLON) {
          consume();
          return true;
        } else {
          cerr << "Error: ';' Missing" << endl;
          return false;
        }
      } else {
        cerr << "Error: Invalid LET Statement" << endl;
        return false;
      }
    } else {
      cerr << "Error: Invalid LET Statement" << endl;
      return false;
    }
  } else {
    cerr << "Error: Invalid LET Statement" << endl;
    return false;
  }
}