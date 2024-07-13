#include "./headers/lexer.h"

//--- Token ---//

string Token::get_token_name(TOKEN_TYPES type) {
  switch (type) {
  case DEBUG:
    return "DEBUG";
  case INT_LIT:
    return "INT_LIT";
  case SEMICOLON:
    return "SEMICOLON";
  case IDENTIFIER:
    return "IDENTIFIER";
  case EQUALS:
    return "EQUALS";
  case LET:
    return "LET";
  case IF:
    return "IF";
  case ELSE:
    return "ELSE";
  case ADD:
    return "ADD";
  case SUB:
    return "SUB";
  case MUL:
    return "MUL";
  case DIV:
    return "DIV";
  case BRACKET_OPEN_CURLY:
    return "BRACKET_OPEN_CURLY";
  case BRACKET_CLOSE_CURLY:
    return "BRACKET_CLOSE_CURLY";
  default:
    return "INVALID_TOKEN";
  }
}

vector<string> Token::keywords = {"dbg", "let", "if", "else"};
vector<char> Token::symbols = {';', '{', '}'};
vector<char> Token::operators = {'+', '-', '*', '/', '='};

bool Token::is_keyword(string keyword) {
  for (auto it : Token::keywords) {
    if (it == keyword)
      return true;
  }
  return false;
}

bool Token::is_operator(char op) {
  for (auto it : Token::operators) {
    if (it == op)
      return true;
  }
  return false;
}

bool Token::is_symbol(char symbol) {
  for (auto it : Token::symbols) {
    if (it == symbol)
      return true;
  }
  return false;
}

Token::Token(TOKEN_TYPES type) {
  this->type = type;
  this->body = "";
  this->name = Token::get_token_name(type);
}

Token::Token(TOKEN_TYPES type, string body) {
  this->type = type;
  this->body = body;
  this->name = Token::get_token_name(type);
}

TOKEN_TYPES Token::get_type() { return this->type; }
string Token::get_name() { return this->name; }
string Token::get_body() { return this->body; }

TOKEN_TYPES Token::get_keyword_type(std::string keyword) {
  if (keyword == "dbg") {
    return DEBUG;
  } else if (keyword == "let") {
    return LET;
  } else if (keyword == "if") {
    return IF;
  } else if (keyword == "else") {
    return ELSE;
  } else {
    return INVALID_TOKEN;
  }
}

TOKEN_TYPES Token::get_operator_type(char op) {
  switch (op) {
  case '+':
    return ADD;
  case '-':
    return SUB;
  case '*':
    return MUL;
  case '/':
    return DIV;
  case '=':
    return EQUALS;
  default:
    return INVALID_TOKEN;
  }
}

TOKEN_TYPES Token::get_symbol_type(char symbol) {
  switch (symbol) {
  case ';':
    return SEMICOLON;
  case '{':
    return BRACKET_OPEN_CURLY;
  case '}':
    return BRACKET_CLOSE_CURLY;
  default:
    return INVALID_TOKEN;
  }
}

//--- Lexical Analyzer ---//

Lexer::Lexer() { token_stream = vector<Token>(); }

bool Lexer::analyse(string input_stream) {
  token_stream.clear();

  int current_pos = 0;

  while (current_pos < input_stream.size()) {
    if (isspace(input_stream[current_pos])) {
      current_pos++;
    } else if (isalpha(input_stream[current_pos])) {
      string buffer = "";
      while (current_pos < input_stream.size() &&
             (isalpha(input_stream[current_pos]) ||
              isdigit(input_stream[current_pos]) ||
              input_stream[current_pos] == '_')) {
        buffer.push_back(input_stream[current_pos]);
        current_pos++;
      }
      if (Token::is_keyword(buffer)) {
        token_stream.push_back(Token(Token::get_keyword_type(buffer)));
      } else {
        token_stream.push_back(Token(IDENTIFIER, buffer));
      }
    } else if (isdigit(input_stream[current_pos])) {
      string buffer = "";
      while (current_pos < input_stream.size() &&
             isdigit(input_stream[current_pos])) {
        buffer.push_back(input_stream[current_pos]);
        current_pos++;
      }
      token_stream.push_back(Token(INT_LIT, buffer));
    } else if (Token::is_operator(input_stream[current_pos])) {
      token_stream.push_back(
          Token(Token::get_operator_type(input_stream[current_pos])));
      current_pos++;
    } else if (Token::is_symbol(input_stream[current_pos])) {
      token_stream.push_back(
          Token(Token::get_symbol_type(input_stream[current_pos])));
      current_pos++;
    } else {
      return false;
    }
  }
  return true;
}

vector<Token> Lexer::get_token_stream() { return token_stream; }

string Lexer::get_token_stream_string() {
  string token_stream_str = "";
  for (int i = 0; i < token_stream.size(); i++) {
    token_stream_str += "[" + token_stream[i].get_name() + "]";
    if (i != token_stream.size() - 1)
      token_stream_str += " ";
  }
  return token_stream_str;
}
