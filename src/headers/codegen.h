#ifndef CODEGEN_H
#define CODEGEN_H

#include "parser.h"
#include "utils.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

class Codegen {
private:
  SymbolTable *symbol_table;

  string data_section;
  string bss_section;
  string text_section;
  bool print_integer_routine;

public:
  Codegen();
  void push_eax();
  void pop_eax();
  void declare_variable_data_section(string variable_name, int value);
  void generate_debug(string variable_name);
  void generate_debug(int value);
  void export_asm();

  void traverse_parse_tree(NodeProgram *program);
  void traverse_stmt(NodeStatement *stmt);
  void traverse_debug(NodeDebug *debug);
  void traverse_let(NodeLet *let);
};

#endif
