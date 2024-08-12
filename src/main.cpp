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

string asm_dir = "./build/Assembly/program.asm";

void print_output_json(string &output) { cout << output << "}"; }

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
  if (argc < 3) {
    cerr << "ERR : Output File Missing" << endl;
    return EXIT_FAILURE;
  }

  if (argc >= 4) {
    asm_dir = string(argv[2]);
    for (int i = 3; i < argc; i++) {
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

  string output_json = "{";
  // ----- Lexer ----- //

  Lexer lexer;
  bool lexer_success = lexer.analyse(input_buffer.str());

  if (!lexer_success) {
    cerr << "Lexical Analysis Failed\n";
    return EXIT_FAILURE;
  }

  if (execution_flags[P_LEXER]) {
    output_json += "\"lexer\":";
    // output_json += "\"" + lexer.get_token_stream_string() + "\"";
    cout << lexer.get_token_stream_string() << "\n";
  }

  if (execution_flags[E_LEXER]) {
    print_output_json(output_json);
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
    output_json += ",\"parser\":";
    output_json += "\"" + NodeProgram::print(program, 0) + "\"";
    cout << NodeProgram::print(program, 0) << endl;
  }

  if (execution_flags[E_PARSER]) {
    print_output_json(output_json);
    return EXIT_SUCCESS;
  }

  // ----- Codegen ----- //

  Codegen codegen;
  codegen.generate_parse_tree(program);

  if (execution_flags[P_ICODE]) {
    output_json += ",\"icode\":";
    // output_json += "\"" + codegen.get_icode() + "\"";
    cout << codegen.get_icode() << endl;
  }

  if (execution_flags[P_ASM]) {
    output_json += ",\"asm\":";
    // output_json += "\"" + codegen.get_asm_code() + "\"";
    cout << codegen.get_asm_code() << endl;
  }

  codegen.export_asm(asm_dir);

  if (execution_flags[E_CODEGEN]) {
    print_output_json(output_json);
    return EXIT_SUCCESS;
  }
  // print_output_json(output_json);

  return EXIT_SUCCESS;
}