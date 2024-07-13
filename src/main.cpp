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
    cout << "Lexical-Anlayser : \n";
    Test_Lexical_Analyser test_lexer;
    test_lexer.run_all_tests();

    // cout << "\nParser-\n";
    // Test_Parser test_parser;
    // test_parser.run_all_tests();
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

    cout << input_buffer.str() << endl;

    Lexer lexer;
    if (lexer.analyse(input_buffer.str())) {
      cout << lexer.get_token_stream_string() << "\n";
      Parser parser;
      if (NodeProgram *program =
              parser.parse_program(lexer.get_token_stream())) {
        cout << "Program Syntax Correct" << endl;
        cout << NodeProgram::print_program(program, 0) << endl;

        Codegen codegen;
        codegen.traverse_parse_tree(program);
        codegen.export_asm();
      } else {
        Error::print_error();
        return EXIT_FAILURE;
      }
    } else {
      cout << "Lexical Analysis Failed\n";
      return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
  }
}