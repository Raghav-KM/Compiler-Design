#ifndef LEXER_H
#define LEXER_H

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

enum TOKENS {
  DEBUG,
  INT_LIT,
  IDENTIFIER,
  SEMICOLON,
  INVALID_TOKEN,
};

enum STATE_RETURN_VALUE {
  FOUND_DEBUG,
  FOUND_INT_LIT,
  FOUND_IDENTIFIER,
  NOT_FOUND,
};

class Lexical_Analyzer {
private:
  string input_stream;
  string buffer;
  vector<TOKENS> token_stream;

  void remove_endline();
  void clear_buffer();
  void push_to_buffer(char);
  bool is_integer_literal(string &token);
  bool is_identifier(string &token);

  string get_token_name(TOKENS &token);

  STATE_RETURN_VALUE initial_state();

  vector<TOKENS> analyse();

public:
  Lexical_Analyzer();
  void set_input_stream(string stream);
  void print_token_stream();
};

#endif