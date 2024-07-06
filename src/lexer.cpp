#include "./headers/lexer.h"

//--- Token ---//
//--- Private Member Function ---//

//--- Public Member Function ---//
Token::Token(TOKEN_TYPES type) {
  token_type = type;
  value = 0;
}
Token::Token(TOKEN_TYPES type, int value, string body) {
  token_type = type;
  this->value = value;
  this->body = body;
}

void Token::set_type(TOKEN_TYPES type) { token_type = type; }
TOKEN_TYPES Token::get_type() { return token_type; }
void Token::set_value(int val) { this->value = val; }
int Token::get_value() { return value; }

void Token::set_body(string body) { this->body = body; }
string Token::get_body() { return body; }

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
bool Lexical_Analyzer::is_operator(char c) {
  if (c == '+' || c == '-' || c == '*' || c == '/')
    return true;
  return false;
}

string Token::get_token_name(TOKEN_TYPES token) {
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
    return "EQUAL";
  case LET:
    return "LET";
  case ADD:
    return "ADD";
  case SUB:
    return "SUB";
  case MUL:
    return "MUL";
  case DIV:
    return "DIV";
  default:
    return "INVALID";
  }
}

TOKEN_TYPES Lexical_Analyzer::analyse_buffer() {
  if (buffer == "dbg") {
    return DEBUG;
  } else if (is_integer_literal(buffer)) {
    return INT_LIT;
  } else if (buffer == "=") {
    return EQUALS;
  } else if (buffer == "let") {
    return LET;
  } else if (buffer == "+") {
    return ADD;
  } else if (buffer == "-") {
    return SUB;
  } else if (buffer == "*") {
    return MUL;
  } else if (buffer == "/") {
    return DIV;
  } else if (is_identifier(buffer)) {
    return IDENTIFIER;
  }
  return INVALID_TOKEN;
}

void Lexical_Analyzer::analyse(string stream) {
  reset();
  input_stream = stream;
  int ptr = 0;
  while (ptr != input_stream.size()) {
    if (isspace(input_stream[ptr]) || input_stream[ptr] == ';' ||
        ptr == input_stream.size() - 1) {
      if (buffer.size() > 0) {
        TOKEN_TYPES token = analyse_buffer();
        switch (token) {
        case DEBUG:
          token_stream.push_back(Token(DEBUG));
          break;
        case INT_LIT:
          token_stream.push_back(Token(INT_LIT, stoi(buffer), ""));
          break;
        case LET:
          token_stream.push_back(Token(LET));
          break;
        case EQUALS:
          token_stream.push_back(Token(EQUALS));
          break;
        case IDENTIFIER:
          token_stream.push_back(Token(IDENTIFIER, 0, buffer));
          break;
        case ADD:
          token_stream.push_back(Token(ADD));
          break;
        case SUB:
          token_stream.push_back(Token(SUB));
          break;
        case MUL:
          token_stream.push_back(Token(MUL));
          break;
        case DIV:
          token_stream.push_back(Token(DIV));
          break;
        default:
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

string Lexical_Analyzer::to_string() {
  string token_stream_str = "";
  for (int i = 0; i < token_stream.size(); i++) {
    token_stream_str +=
        "[" + Token::get_token_name(token_stream[i].get_type()) + "]";
    if (i != token_stream.size() - 1)
      token_stream_str += " ";
  }
  return token_stream_str;
}

vector<Token> Lexical_Analyzer::get_token_stream() { return token_stream; }

void Lexical_Analyzer::reset() {
  input_stream = "";
  buffer = "";
  token_stream.clear();
}
