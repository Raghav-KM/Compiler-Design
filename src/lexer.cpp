#include "./headers/lexer.h"

//--- Private Member Function ---//

void Lexical_Analyzer::remove_endline() {
  for (int i = 0; i < input_stream.length(); i++) {
    if (input_stream[i] == '\n') {
      input_stream[i] = ' ';
    }
  }
}

void Lexical_Analyzer::clear_buffer() { buffer = ""; }

void Lexical_Analyzer::push_to_buffer(char c) { buffer.push_back(c); }

bool Lexical_Analyzer::is_integer_literal(string &token) {
  return all_of(token.begin(), token.end(), ::isdigit);
}

bool Lexical_Analyzer::is_identifier(string &token) {
  for (char c : token) {
    if (c >= '0' && c <= '9')
      return false;
  }
  return true;
}

string Lexical_Analyzer::get_token_name(TOKENS &token) {
  if (token == DEBUG) {
    return "DEBUG";
  }
  if (token == INT_LIT) {
    return "INT_LIT";
  }
  if (token == SEMICOLON) {
    return "SEMICOLON";
  }
  if (token == IDENTIFIER) {
    return "IDENTIFIER";
  }
  if (token == INVALID_TOKEN) {
    return "INVALID_TOKEN";
  }
  return "INVALID";
}

STATE_RETURN_VALUE Lexical_Analyzer::initial_state() {
  if (buffer == "DBG") {
    return FOUND_DEBUG;
  } else if (is_integer_literal(buffer)) {
    return FOUND_INT_LIT;
  } else if (is_identifier(buffer)) {
    return FOUND_IDENTIFIER;
  }

  return NOT_FOUND;
}

vector<TOKENS> Lexical_Analyzer::analyse() {
  int ptr = 0;
  while (ptr != input_stream.size()) {
    if (isspace(input_stream[ptr]) || input_stream[ptr] == ';') {
      if (buffer.size() > 0) {
        if (initial_state() == FOUND_DEBUG) {
          token_stream.push_back(DEBUG);
        } else if (initial_state() == FOUND_INT_LIT) {
          token_stream.push_back(INT_LIT);
        } else if (initial_state() == FOUND_IDENTIFIER) {
          token_stream.push_back(IDENTIFIER);
        } else {
          token_stream.push_back(INVALID_TOKEN);
        }
      }

      if (input_stream[ptr] == ';')
        token_stream.push_back(SEMICOLON);
      clear_buffer();
    } else {
      push_to_buffer(input_stream[ptr]);
    }
    ptr++;
  }
  return token_stream;
}

//--- Public Memeber Function ---//

Lexical_Analyzer::Lexical_Analyzer() {
  buffer = "";
  token_stream = vector<TOKENS>();
}

void Lexical_Analyzer::Lexical_Analyzer::set_input_stream(string stream) {
  input_stream = stream;
  remove_endline();
  cout << input_stream << "\n";
}

void Lexical_Analyzer::print_token_stream() {
  analyse();
  for (auto token : token_stream) {
    cout << get_token_name(token) << " ";
  }
  cout << "\n";
}
