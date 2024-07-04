#include "./headers/codegen.h"
#include "./headers/lexer.h"
#include "./headers/parser.h"
#include "./headers/testing.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

int main(int argc, char *argv[]) {
  cout << endl;
  bool do_testing = false;

  if (argc < 2) {
    cerr << "ERR : Input File Missing" << endl;
    return EXIT_FAILURE;
  }

  if (argc == 3) {
    if (string(argv[2]) == "-t") {
      do_testing = true;
    } else {
      cerr << "ERR : Invalid Flag [" << argv[2] << "]" << endl;
      return EXIT_FAILURE;
    }
  }

  if (do_testing) {
    Test_Lexical_Analyser test_lexer;
    test_lexer.run_all_tests();
    Test_Parser test_parser;
    test_parser.run_all_tests();
  } else {
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

    cout << input_buffer.str();

    Lexical_Analyzer analyzer;
    analyzer.set_input_stream(input_buffer.str());
    analyzer.print_token_stream();

    Parser parser(analyzer);
    if (NodeProgram *program = parser.parse_program()) {
      cout << "Program Syntax Correct" << endl;
      cout << NodeProgram::print_program(program, 0) << endl;
    } else {
      return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
  }
}