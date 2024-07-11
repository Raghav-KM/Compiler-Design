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
  IF,
  ELSE,
  EQUALS,
  ADD,
  SUB,
  MUL,
  DIV,
  SEMICOLON,
  BRACKET_OPEN_CURLY,
  BRACKET_CLOSE_CURLY,
  INVALID_TOKEN,
  END_FILE,
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
  static string get_token_name(TOKEN_TYPES token);
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
  bool is_operator(char);
  TOKEN_TYPES analyse_buffer();
  void reset();

public:
  Lexical_Analyzer();
  void analyse(string stream);
  string to_string();
  vector<Token> get_token_stream();
};

#endif