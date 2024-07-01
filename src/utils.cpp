#include "./headers/utils.h"

// --- Error --- //

void Error::invalid_syntax() { cerr << "Error: Invalid Syntax\n"; }
void Error::invalid_syntax(string msg) { cerr << "Error: " + msg + "\n"; }

void Error::undefined_variable(string symbol_name) {
  cerr << "Error: Undefined Variable [ " << symbol_name << " ]\n";
}

void Error::redeclaration_variable(string symbol_name) {
  cerr << "Error: Redefination of Variable [ " << symbol_name << " ]\n";
}

// --- Symbol Table --- //

SymbolTable *SymbolTable::instance = NULL;

SymbolTable::SymbolTable() {}

SymbolTable *SymbolTable::get_instance() {
  if (instance == NULL) {
    return instance = new SymbolTable();
  }
  return instance;
}

RESULT_TYPE SymbolTable::declare(string symbol_name) {
  if (table.find(symbol_name) == table.end()) {
    table[symbol_name] = 0;
  } else {
    return Redeclaration;
  }
  return Success;
}
RESULT_TYPE SymbolTable::declare(string symbol_name, int symbol_value) {
  if (table.find(symbol_name) == table.end()) {
    table[symbol_name] = symbol_value;
  } else {
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
