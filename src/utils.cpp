#include "./headers/utils.h"

// --- Error --- //

void Error::invalid_syntax() { cerr << "Error: Invalid Syntax\n"; }

void Error::undefined_variable(string symbol_name) {
  cerr << "Error: Undefined Variable [ " << symbol_name << " ]\n";
}

void Error::redeclaration_variable(string symbol_name) {
  cerr << "Error: Redefination of Variable [ " << symbol_name << " ]\n";
}

// --- Symbol Table --- //

RESULT_TYPE SymbolTable::declare(string symbol_name) {
  if (table.find(symbol_name) == table.end()) {
    table[symbol_name] = 0;
  } else {
    // Error: Symbol Redeclaration
    return Redeclaration;
  }
  return Success;
}
RESULT_TYPE SymbolTable::update(string symbol_name, int symbol_value) {
  if (table.find(symbol_name) != table.end()) {
    table[symbol_name] = symbol_value;
  } else {
    return Undeclared;
  }
  return Success;
}

RESULT_TYPE SymbolTable::check(string symbol_name) {
  if (table.find(symbol_name) == table.end()) {
    return Undeclared;
  }
  return Success;
}

int SymbolTable::get_value(string symbol_name) { return table[symbol_name]; }
