#include "./headers/codegen.h"
#include "./headers/lexer.h"
#include "./headers/parser.h"
#include "./headers/testing.h"

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

using namespace std;

enum Flags {
  E_LEXER,
  E_PARSER,
  E_CODEGEN,
  P_LEXER,
  P_PARSER,
  P_ICODE,
  P_ASM,
};

const int FLAG_COUNT = 7;

map<string, Flags> flag_mapping;

vector<bool> execution_flags;

int main(int argc, char *argv[]) {

  execution_flags = vector<bool>(FLAG_COUNT, false);

  flag_mapping["-l"] = E_LEXER;
  flag_mapping["-p"] = E_PARSER;
  flag_mapping["-c"] = E_CODEGEN;
  flag_mapping["-pl"] = P_LEXER;
  flag_mapping["-pp"] = P_PARSER;
  flag_mapping["-pc"] = P_ICODE;
  flag_mapping["-pa"] = P_ASM;

  bool do_testing = false;

  if (argc < 2) {
    cerr << "ERR : Input File Missing" << endl;
    return EXIT_FAILURE;
  }

  if (argc >= 3) {
    for (int i = 2; i < argc; i++) {
      string flag = string(argv[i]);
      if (flag_mapping.find(flag) == flag_mapping.end()) {
        cerr << "ERR : Invalid Flag [" << argv[i] << "]" << endl;
        return EXIT_FAILURE;
      }
      execution_flags[flag_mapping[flag]] = true;
    }
  }

  string input_file_path = argv[1];
  ifstream f_input(input_file_path);
  if (f_input.fail()) {
    cerr << "ERR : Error while reading file" << endl;
    return EXIT_FAILURE;
  }

  stringstream input_buffer;
  input_buffer << f_input.rdbuf();
  f_input.close();

  // ----- Lexer ----- //

  Lexer lexer;
  bool lexer_success = lexer.analyse(input_buffer.str());

  if (!lexer_success) {
    cerr << "Lexical Analysis Failed\n";
    return EXIT_FAILURE;
  }

  if (execution_flags[P_LEXER]) {
    cout << lexer.get_token_stream_string() << "\n";
  }

  if (execution_flags[E_LEXER]) {
    return EXIT_SUCCESS;
  }

  // ----- Parser ----- //

  Parser parser;
  NodeProgram *program = parser.parse_program(lexer.get_token_stream());

  if (program == NULL) {
    cerr << "Parsing Failed\n";
    return EXIT_FAILURE;
  }

  if (execution_flags[P_PARSER]) {
    cout << NodeProgram::print(program, 0) << endl;
  }

  if (execution_flags[E_PARSER]) {
    return EXIT_SUCCESS;
  }

  // ----- Codegen ----- //

  Codegen codegen;
  codegen.generate_parse_tree(program);
  codegen.export_asm();

  return EXIT_SUCCESS;
}