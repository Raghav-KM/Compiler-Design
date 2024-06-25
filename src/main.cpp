#include "./headers/lexer.h"
#include "./headers/parser.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

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

  Parser parser(analyzer);

  if (parser.parse_program()) {
    cout << "Program Syntax Correct" << endl;
  } else {
    cout << "Program Syntax Incorrect" << endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}