#ifndef LEXER_H
#define LEXER_H

#include "utils.h"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

enum TOKEN_TYPES {
  // Keywords
  LET,
  DEBUG,
  IF,
  ELSE,
  FOR,
  ENDL,

  // Identifiers
  IDENTIFIER,

  // Literals
  INT_LIT,
  CHAR_LIT,

  // Symbols
  SEMICOLON,
  BRACKET_OPEN_CURLY,
  BRACKET_CLOSE_CURLY,
  BRACKET_OPEN,
  BRACKET_CLOSE,
  SINGLE_QUOTE,

  // Operators
  EQUALS,
  ADD,
  SUB,
  MUL,
  DIV,

  // Comparison Operators
  EQUAL_EQUALS,
  LESS,
  GREATER,
  GREATER_EQUALS,
  LESS_EQUALS,
  NOT_EQUALS,

  // End of File
  END_OF_FILE,

  // Invalid Token
  INVALID_TOKEN
};

class Token {
private:
  TOKEN_TYPES type;
  string body;
  string name;

  static string get_token_name(TOKEN_TYPES type);

public:
  Token(TOKEN_TYPES type);
  Token(TOKEN_TYPES types, string body);
  TOKEN_TYPES get_type();
  string get_name();
  string get_body();

  static vector<string> keywords;
  static vector<char> char_symbols;
  static vector<char> char_operators;

  static bool is_keyword(string keyword);
  static bool is_operator(char op);
  static bool is_symbol(char symbol);

  static TOKEN_TYPES get_keyword_type(string keyword);
  static TOKEN_TYPES get_operator_type(string op);
  static TOKEN_TYPES get_symbol_type(char symbol);
};

class Lexer {
private:
  vector<Token> token_stream;

public:
  Lexer();
  bool analyse(string input_stream);
  vector<Token> get_token_stream();
  string get_token_stream_string();
};

#endif