#ifndef CODEGEN_H
#define CODEGEN_H

#include "parser.h"
#include "utils.h"
#include <fstream>
#include <iostream>
#include <queue>
#include <string>

using namespace std;

class Codegen {
private:
  static int max_count;
  static int var_count;

  static int if_count;
  static int for_count;

  static int comp_count;

  string data_section;
  string bss_section;
  string text_section;

  queue<NodeFunction *> function_definations;

  bool require_print_integer_subroutine;
  bool require_comparison_subroutine;
  bool require_print_character_subroutine;

public:
  Codegen();

  string get_new_temp_variable();
  int get_if_count();
  int get_for_count();
  int get_comp_count();
  static void reset_count();

  void declare_variable_bss_section(string variable_name);

  void export_asm();

  void generate_parse_tree(NodeProgram *program);
  void generate_stmt_list(NodeStatementList *stmt_list);

  void generate_stmt(NodeStatement *stmt);
  void generate_debug(NodeDebug *debug);

  void generate_let(NodeLet *let);
  void generate_if(NodeIf *IF);
  void generate_assign(NodeAssign *assign);

  void generate_for(NodeFor *FOR);
  void generate_function(NodeFunction *function);
  void generate_function_call(NodeFunctionCall *function_call);

  string generate_comparative_expression(NodeComparativeExpression *comp_exp);

  string generate_additive_expression(NodeAdditiveExpression *add_exp);
  string
  generate_multiplicative_expression(NodeMultiplicativeExpression *mul_exp);
  string generate_expression(NodeExpression *exp);

  string get_identifier_name(NodeIdentifier *identifier);

  void finalize_text_section();
  void finalize_data_section();
  void finalize_bss_section();
};

#endif
