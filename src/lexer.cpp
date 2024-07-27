#include "./headers/lexer.h"

DATA_TYPES max_datatype(DATA_TYPES d1, DATA_TYPES d2) {
  if ((int)d1 >= (int)d2) {
    return d1;
  } else {
    return d2;
  }
}

//--- Token ---//

string Token::get_token_name(TOKEN_TYPES type) {
  switch (type) {
  case DEBUG:
    return "DEBUG";
  case INT_LIT:
    return "INT_LIT";
  case CHAR_LIT:
    return "CHAR_LIT";
  case SEMICOLON:
    return "SEMICOLON";
  case IDENTIFIER:
    return "IDENTIFIER";
  case EQUALS:
    return "EQUALS";
  case EQUAL_EQUALS:
    return "EQUAL_EQUALS";
  case GREATER:
    return "GREATER";
  case LESS:
    return "LESS";
  case GREATER_EQUALS:
    return "GREATER_EQUALS";
  case LESS_EQUALS:
    return "LESS_EQUALS";
  case NOT_EQUALS:
    return "NOT_EQUALS";
  case LET:
    return "LET";
  case IF:
    return "IF";
  case ELSE:
    return "ELSE";
  case FOR:
    return "FOR";
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
  case BRACKET_OPEN:
    return "BRACKET_OPEN";
  case BRACKET_CLOSE:
    return "BRACKET_CLOSE";
  case COLON:
    return "COLON";
  case DT_INT:
    return "DT_INT";
  case DT_CHAR:
    return "DT_CHAR";
  case ENDL:
    return "ENDL";
  default:
    return "INVALID_TOKEN";
  }
}

vector<string> Token::keywords = {"dbg", "let",  "if",  "else",
                                  "for", "endl", "int", "char"};
vector<char> Token::char_symbols = {';', '{', '}', '(', ')', '\'', ':'};
vector<char> Token::char_operators = {'+', '-', '*', '/', '=', '<', '>', '!'};

bool Token::is_keyword(string keyword) {
  for (auto it : Token::keywords) {
    if (it == keyword)
      return true;
  }
  return false;
}

bool Token::is_operator(char op) {
  for (auto it : Token::char_operators) {
    if (it == op)
      return true;
  }
  return false;
}

bool Token::is_symbol(char symbol) {
  for (auto it : Token::char_symbols) {
    if (it == symbol)
      return true;
  }
  return false;
}

Token::Token(TOKEN_TYPES type) {
  this->type = type;
  this->body = "";
  this->name = Token::get_token_name(type);
  this->line_no = Lexer::global_line_no;
  this->token_no = Lexer::global_token_no;
}

Token::Token(TOKEN_TYPES type, string body) {
  this->type = type;
  this->body = body;
  this->name = Token::get_token_name(type);
  this->line_no = Lexer::global_line_no;
  this->token_no = Lexer::global_token_no;
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
  } else if (keyword == "for") {
    return FOR;
  } else if (keyword == "endl") {
    return ENDL;
  } else if (keyword == "int") {
    return DT_INT;
  } else if (keyword == "char") {
    return DT_CHAR;
  } else {
    return INVALID_TOKEN;
  }
}

TOKEN_TYPES Token::get_operator_type(string op) {
  if (op == "+") {
    return ADD;
  } else if (op == "-") {
    return SUB;
  } else if (op == "*") {
    return MUL;
  } else if (op == "/") {
    return DIV;
  } else if (op == "=") {
    return EQUALS;
  } else if (op == "==") {
    return EQUAL_EQUALS;
  } else if (op == ">") {
    return GREATER;
  } else if (op == "<") {
    return LESS;
  } else if (op == "<=") {
    return LESS_EQUALS;
  } else if (op == ">=") {
    return GREATER_EQUALS;
  } else if (op == "!=") {
    return NOT_EQUALS;
  } else {
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
  case '(':
    return BRACKET_OPEN;
  case ')':
    return BRACKET_CLOSE;
  case ':':
    return COLON;
  default:
    return INVALID_TOKEN;
  }
}

//--- Lexical Analyzer ---//

int Lexer::global_line_no = 1;
int Lexer::global_token_no = 0;

Lexer::Lexer() { token_stream = vector<Token>(); }

bool Lexer::analyse(string input_stream) {
  token_stream.clear();

  int current_pos = 0;

  while (current_pos < input_stream.size()) {
    if (isspace(input_stream[current_pos])) {
      if (input_stream[current_pos] == '\n') {
        Lexer::global_line_no++;
        Lexer::global_token_no = 0;
      }
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
        TOKEN_TYPES token_type = Token::get_keyword_type(buffer);
        if (token_type == ENDL) {
          Lexer::global_token_no++;
          token_stream.push_back(Token(CHAR_LIT, "\n"));
        } else {
          Lexer::global_token_no++;
          token_stream.push_back(Token(Token::get_keyword_type(buffer)));
        }
      } else {
        Lexer::global_token_no++;
        token_stream.push_back(Token(IDENTIFIER, buffer));
      }
    } else if (isdigit(input_stream[current_pos])) {
      string buffer = "";
      while (current_pos < input_stream.size() &&
             isdigit(input_stream[current_pos])) {
        buffer.push_back(input_stream[current_pos]);
        current_pos++;
      }
      Lexer::global_token_no++;
      token_stream.push_back(Token(INT_LIT, buffer));
    } else if (Token::is_operator(input_stream[current_pos])) {
      string buffer = "";
      while (current_pos < input_stream.size() &&
             Token::is_operator(input_stream[current_pos])) {

        buffer.push_back(input_stream[current_pos]);
        current_pos++;
      }
      Lexer::global_token_no++;
      token_stream.push_back(Token(Token::get_operator_type(buffer)));
    } else if (Token::is_symbol(input_stream[current_pos])) {
      if (input_stream[current_pos] == '\'') {
        current_pos++;
        string buffer = "";
        while (current_pos < input_stream.size() &&
               input_stream[current_pos] != '\'') {
          buffer.push_back(input_stream[current_pos]);
          current_pos++;
        }
        if (buffer.size() > 1) {
          Lexer::global_token_no++;
          token_stream.push_back(Token(INVALID_TOKEN));
          return false;
        }
        Lexer::global_token_no++;
        token_stream.push_back(Token(CHAR_LIT, buffer));

      } else {
        Lexer::global_token_no++;
        token_stream.push_back(
            Token(Token::get_symbol_type(input_stream[current_pos])));
      }
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

    // token_stream_str += "[" + token_stream[i].get_name() + " " +
    //                     to_string(token_stream[i].line_no) + ":" +
    //                     to_string(token_stream[i].token_no) + "]";

    if (i != token_stream.size() - 1)
      token_stream_str += " ";
  }
  return token_stream_str;
}
