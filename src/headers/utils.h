#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <map>
#include <set>
#include <stdexcept>
#include <string>

using namespace std;

enum DATA_TYPES { UNDEFINED, CHAR, INT };

enum RESULT_TYPE { UNDECLARED, REDECLARATION, SUCCESS };

class Error {
  static string err_buffer;

public:
  static void invalid_token(int line_no, int token_no);
  static void invalid_syntax(int line_no, int token_no);
  static void invalid_syntax(string msg, int line_no, int token_no);
  static void undefined_variable(string symbol_name, int line_no, int token_no);
  static void redeclaration_variable(string symbol_name, int line_no,
                                     int token_no);
  static void print_error();
};

class SymbolTable {
  friend class Codegen;

private:
  static SymbolTable *instance;
  map<int, map<string, DATA_TYPES>> _table;
  SymbolTable();

public:
  static SymbolTable *get_instance();

  RESULT_TYPE declare(string symbol_name, int scope);

  RESULT_TYPE exists(string symbol_name, int scope, int &present_scope);

  void set_datatype(string symbol_name, DATA_TYPES type, int scope);
  DATA_TYPES get_datatype(string symbol_name, int scope);

  void clear_scope(int scope);

  void reset();
};

#endif