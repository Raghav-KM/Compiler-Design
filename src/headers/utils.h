#ifndef UTILS_H
#define UTILS_H

#include "lexer.h"
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>

using namespace std;

enum RESULT_TYPE { Undeclared, Redeclaration, Success };

class Error {
public:
  static void invalid_syntax();
  static void undefined_variable(string symbol_name);
  static void redeclaration_variable(string symbol_name);
};

class SymbolTable {
private:
  map<string, int> table;

public:
  RESULT_TYPE declare(string symbol_name);
  RESULT_TYPE update(string symbol_name, int symbol_value);
  RESULT_TYPE check(string symbol_name);
  int get_value(string symbol_name);
};

#endif