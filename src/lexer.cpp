#include "./headers/lexer.h"

//--- Token ---//
//--- Private Member Function ---//

//--- Public Member Function ---//
Token::Token(TOKEN_TYPES type) {
  token_type = type;
  value = 0;
}
Token::Token(TOKEN_TYPES type, int value) {
  token_type = type;
  this->value = value;
}

void Token::set_type(TOKEN_TYPES type) { token_type = type; }
TOKEN_TYPES Token::get_type() { return token_type; }
void Token::set_value(int val) { this->value = val; }
int Token::get_value() { return value; }

//--- Lexical Analyzer ---//

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

string Lexical_Analyzer::get_token_name(TOKEN_TYPES token) {
  switch (token) {
  case DEBUG:
    return "DEBUG";
  case INT_LIT:
    return "INT_LIT";
  case SEMICOLON:
    return "SEMICOLON";
  case IDENTIFIER:
    return "IDENTIFIER";
  case INVALID_TOKEN:
    return "INVALID_TOKEN";
  case EQUALS:
    return "EQUALS";
  case LET:
    return "LET";
  default:
    return "INVALID";
  }
}

STATE_RETURN_VALUE Lexical_Analyzer::initial_state() {

  if (buffer == "dbg") {
    return FOUND_DEBUG;
  } else if (is_integer_literal(buffer)) {
    return FOUND_INT_LIT;
  } else if (buffer == "=") {
    return FOUND_EQUALS;
  } else if (buffer == "let") {
    return FOUND_LET;
  } else if (is_identifier(buffer)) {
    return FOUND_IDENTIFIER;
  }
  return NOT_FOUND;
}

void Lexical_Analyzer::analyse() {
  int ptr = 0;
  while (ptr != input_stream.size()) {
    if (isspace(input_stream[ptr]) || input_stream[ptr] == ';') {
      if (buffer.size() > 0) {
        STATE_RETURN_VALUE state_value = initial_state();
        switch (state_value) {
        case FOUND_DEBUG:
          token_stream.push_back(Token(DEBUG));
          break;
        case FOUND_INT_LIT:
          token_stream.push_back(Token(INT_LIT, stoi(buffer)));
          break;
        case FOUND_LET:
          token_stream.push_back(Token(LET));
          break;
        case FOUND_EQUALS:
          token_stream.push_back(Token(EQUALS));
          break;
        case FOUND_IDENTIFIER:
          token_stream.push_back(Token(IDENTIFIER));
          break;
        default:
          throw std::runtime_error("Error: Invalid token encountered");
          token_stream.push_back(Token(INVALID_TOKEN));
          break;
        }
      }
      if (input_stream[ptr] == ';')
        token_stream.push_back(Token(SEMICOLON));
      clear_buffer();
    } else {
      push_to_buffer(input_stream[ptr]);
    }
    ptr++;
  }
}

//--- Public Memeber Function ---//

Lexical_Analyzer::Lexical_Analyzer() {
  buffer = "";
  token_stream = vector<Token>();
}

void Lexical_Analyzer::Lexical_Analyzer::set_input_stream(string stream) {
  input_stream = stream;
  // remove_endline();
  // cout << input_stream << "\n";
}

void Lexical_Analyzer::print_token_stream() {
  analyse();
  cout << "\n";
  for (auto token : token_stream) {
    if (token.get_type() == INT_LIT) {
      cout << "[" << get_token_name(token.get_type()) << ","
           << token.get_value() << "] ";
    } else {

      cout << "[" << get_token_name(token.get_type()) << "] ";
    }
  }
  cout << "\n";
}
