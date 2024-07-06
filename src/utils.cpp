#include "./headers/utils.h"

// --- Error --- //

string Error::err_buffer = "";

void Error::invalid_syntax() { err_buffer += "Error: Invalid Syntax\n"; }

void Error::invalid_syntax(string msg) { err_buffer += "Error: " + msg + "\n"; }

void Error::undefined_variable(string symbol_name) {
  err_buffer += "Error: Undefined Variable [ " + symbol_name + " ]\n";
}

void Error::redeclaration_variable(string symbol_name) {
  err_buffer += "Error: Redefination of Variable [ " + symbol_name + " ]\n";
}

void Error::print_error() { cerr << err_buffer; }

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
    table[symbol_name] = true;
  } else {
    return REDECLARATION;
  }
  return SUCCESS;
}

RESULT_TYPE SymbolTable::exists(string symbol_name) {
  if (table.find(symbol_name) == table.end()) {
    return UNDECLARED;
  }
  return SUCCESS;
}

int SymbolTable::get_value(string symbol_name) { return table[symbol_name]; }

void SymbolTable::reset() { this->table.clear(); }
