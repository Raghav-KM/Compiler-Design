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
  static int max_count;
  static int var_count;

  static int if_count;
  static int for_count;

  string data_section;
  string bss_section;
  string text_section;

  string function_definations;

  bool require_print_integer_subroutine;
  bool require_comparison_subroutine;
  bool require_print_character_subroutine;

public:
  Codegen();

  string get_new_temp_variable();
  int get_if_count();
  int get_for_count();
  static void reset_count();

  void push_eax();
  void pop_eax();
  void declare_variable_data_section(string variable_name, int value);
  void declare_variable_bss_section(string variable_name);

  void generate_debug(string variable_name);

  void generate_debug(string value, DATA_TYPES type);
  void generate_let(string lval, string rval);
  void generate_if(string condition, NodeStatementList *stmt_list_if,
                   NodeStatementList *stmt_list_else, int if_count);
  void generate_assign(string lval, string rval);

  void generate_arithemetic_expressions(string, string, char, string);
  void generate_comparative_expression(string, string, string, string);
  void export_asm();

  void traverse_parse_tree(NodeProgram *program);
  void traverse_stmt_list(NodeStatementList *stmt_list);

  void traverse_stmt(NodeStatement *stmt);
  void traverse_debug(NodeDebug *debug);
  void traverse_let(NodeLet *let);
  void traverse_if(NodeIf *IF);
  void traverse_assign(NodeAssign *assign);
  void traverse_for(NodeFor *FOR);
  void traverse_function(NodeFunction *function);
  void traverse_function_call(NodeFunctionCall *function_call);

  string traverse_comparative_expression(NodeComparativeExpression *comp_exp);

  string traverse_additive_expression(NodeAdditiveExpression *add_exp);
  string
  traverse_multiplicative_expression(NodeMultiplicativeExpression *mul_exp);
  string traverse_expression(NodeExpression *exp);
};

#endif
