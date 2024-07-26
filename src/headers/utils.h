#ifndef UTILS_H
#define UTILS_H

#include "lexer.h"
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>

using namespace std;

enum RESULT_TYPE { UNDECLARED, REDECLARATION, SUCCESS };

class Error {
  static string err_buffer;

public:
  static void invalid_syntax();
  static void invalid_syntax(string msg);
  static void undefined_variable(string symbol_name);
  static void redeclaration_variable(string symbol_name);
  static void print_error();
};

class SymbolTable {
  friend class Codegen;

private:
  static SymbolTable *instance;
  map<string, DATA_TYPES> table;
  SymbolTable();

public:
  static SymbolTable *get_instance();

  RESULT_TYPE declare(string symbol_name);
  RESULT_TYPE exists(string symbol_name);
  void set_datatype(string symbol_name, DATA_TYPES type);
  DATA_TYPES get_datatype(string symbol_name);

  void reset();
};

#endif