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
  static int count;
  string data_section;
  string bss_section;
  string text_section;
  bool require_print_integer_subroutine;

public:
  Codegen();

  string get_new_temp_variable();

  void push_eax();
  void pop_eax();
  void declare_variable_data_section(string variable_name, int value);
  void declare_variable_bss_section(string variable_name);

  void generate_debug(string variable_name);
  void generate_debug(int value);
  void generate_expressions(string, string, char, string);

  void export_asm();

  void traverse_parse_tree(NodeProgram *program);
  void traverse_stmt(NodeStatement *stmt);
  void traverse_debug(NodeDebug *debug);
  void traverse_let(NodeLet *let);

  string traverse_additive_expression(NodeAdditiveExpression *add_exp);
  string
  traverse_multiplicative_expression(NodeMultiplicativeExpression *mul_exp);
  string traverse_expression(NodeExpression *exp);
};

#endif
