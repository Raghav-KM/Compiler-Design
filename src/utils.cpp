#include "./headers/utils.h"

// --- Error --- //

string Error::err_buffer = "";

void Error::invalid_syntax(int line_no, int token_no) {
  err_buffer += "Error " + to_string(line_no) + ":" + to_string(token_no) +
                " - Invalid Syntax\n";
}

void Error::invalid_syntax(string msg, int line_no, int token_no) {
  err_buffer += "Error " + to_string(line_no) + ":" + to_string(token_no) +
                " - " + msg + "\n ";
}

void Error::undefined_variable(string symbol_name, int line_no, int token_no) {
  err_buffer += "Error " + to_string(line_no) + ":" + to_string(token_no) +
                " - Undefined identifier [ " + symbol_name + " ]\n";
}

void Error::redeclaration_variable(string symbol_name, int line_no,
                                   int token_no) {
  err_buffer += "Error " + to_string(line_no) + ":" + to_string(token_no) +
                " - Redefination of Variable [ " + symbol_name + " ]\n";
}

void Error::print_error() { cerr << err_buffer; }

// --- Symbol Table --- //

SymbolTable *SymbolTable::instance = NULL;

SymbolTable::SymbolTable() { _table = map<int, map<string, DATA_TYPES>>(); }

SymbolTable *SymbolTable::get_instance() {
  if (instance == NULL) {
    return instance = new SymbolTable();
  }
  return instance;
}

RESULT_TYPE SymbolTable::declare(string symbol_name, int scope) {

  if (_table[scope].find(symbol_name) == _table[scope].end()) {
    _table[scope][symbol_name] = UNDEFINED;
  } else {
    return REDECLARATION;
  }
  return SUCCESS;
}

RESULT_TYPE SymbolTable::exists(string symbol_name, int scope,
                                int &present_scope) {
  for (int i = scope; i >= 1; i--) {
    if (_table[i].find(symbol_name) != _table[i].end()) {
      present_scope = i;
      return SUCCESS;
    }
  }
  return UNDECLARED;
}

DATA_TYPES SymbolTable::get_datatype(string symbol_name, int scope) {
  return _table[scope][symbol_name];
}

void SymbolTable::set_datatype(string symbol_name, DATA_TYPES datatye,
                               int scope) {
  _table[scope][symbol_name] = datatye;
}

void SymbolTable::reset() { this->_table.clear(); }

void SymbolTable::clear_scope(int scope) { _table[scope].clear(); }

// ---- General Fucntion ---- //