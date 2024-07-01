#ifndef LEXER_H
#define LEXER_H

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

enum TOKEN_TYPES {
  DEBUG,
  INT_LIT,
  IDENTIFIER,
  LET,
  EQUALS,
  ADD,
  SUB,
  MUL,
  DIV,
  SEMICOLON,
  INVALID_TOKEN,
  END_FILE,
};

enum STATE_RETURN_VALUE {
  FOUND_DEBUG,
  FOUND_INT_LIT,
  FOUND_IDENTIFIER,
  FOUND_LET,
  FOUND_EQUALS,
  FOUND_ADD,
  FOUND_SUB,
  FOUND_MUL,
  FOUND_DIV,
  NOT_FOUND,
};

class Token {
private:
  TOKEN_TYPES token_type;
  int value;
  string body;

public:
  Token(TOKEN_TYPES type);
  Token(TOKEN_TYPES type, int value, string body);

  void set_type(TOKEN_TYPES type);
  TOKEN_TYPES get_type();
  void set_value(int val);
  int get_value();
  void set_body(string body);
  string get_body();
};

class Lexical_Analyzer {

  friend class Parser;

private:
  string input_stream;
  string buffer;
  vector<Token> token_stream;

  void remove_endline();
  void clear_buffer();
  void push_to_buffer(char);
  bool is_integer_literal(string &token);
  bool is_identifier(string &token);

  string get_token_name(TOKEN_TYPES token);

  STATE_RETURN_VALUE initial_state();

  void analyse();

public:
  Lexical_Analyzer();
  void set_input_stream(string stream);
  void print_token_stream();
  string get_token_stream();
  void reset();
};

#endif