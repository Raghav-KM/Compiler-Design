#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
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

  void remove_endline() {
    for (int i = 0; i < input_stream.length(); i++) {
      if (input_stream[i] == '\n') {
        input_stream[i] = ' ';
      }
    }
  }

  void clear_buffer() { buffer = ""; }

  void push_to_buffer(char c) { buffer.push_back(c); }

  bool is_integer_literal(string &token) {
    return all_of(token.begin(), token.end(), ::isdigit);
  }

  bool is_identifier(string &token) {
    for (char c : token) {
      if (c >= '0' && c <= '9')
        return false;
    }
    return true;
  }

  string get_token_name(TOKENS token) {
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

  STATE_RETURN_VALUE initial_state() {
    if (buffer == "DBG") {
      return FOUND_DEBUG;
    } else if (is_integer_literal(buffer)) {
      return FOUND_INT_LIT;
    } else if (is_identifier(buffer)) {
      return FOUND_IDENTIFIER;
    }

    return NOT_FOUND;
  }

  vector<TOKENS> analyse() {
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

public:
  Lexical_Analyzer() {
    buffer = "";
    token_stream = vector<TOKENS>();
  }

  void set_input_stream(string stream) {
    input_stream = stream;
    remove_endline();
    cout << input_stream << "\n";
  }

  void print_token_stream() {
    analyse();

    for (auto token : token_stream) {
      cout << get_token_name(token) << " ";
    }
    cout << "\n";
  }
};

int main(int argc, char *argv[]) {

  if (argc != 2) {
    cerr << "ERR : Input File Missing" << endl;
    return EXIT_FAILURE;
  }

  string input_file_path = argv[1];
  ifstream f_input(input_file_path);
  if (f_input.fail()) {
    cerr << "ERR : Error while reading file" << endl;
    return EXIT_FAILURE;
  }

  cout << "Reading " << argv[1] << endl;
  stringstream input_buffer;
  input_buffer << f_input.rdbuf();
  f_input.close();

  cout << input_buffer.str() << endl;

  Lexical_Analyzer analyzer;
  analyzer.set_input_stream(input_buffer.str());
  analyzer.print_token_stream();

  return EXIT_SUCCESS;
}