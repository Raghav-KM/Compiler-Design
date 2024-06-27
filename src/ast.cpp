#include "./headers/ast.h"

NodeStatement::NodeStatement(NodeDebug *debug) {
  this->let = NULL;
  this->debug = debug;
}
NodeStatement::NodeStatement(NodeLet *let) {
  this->let = let;
  this->debug = NULL;
}

NodeDebug::NodeDebug(NodeINT *INT) {
  this->identifier = NULL;
  this->INT = INT;
}
NodeDebug::NodeDebug(NodeIdentifier *identifier) {
  this->identifier = identifier;
  this->INT = NULL;
}

NodeLet::NodeLet(NodeINT *INT, NodeIdentifier *identifier) {
  this->INT = INT;
  this->identifier = identifier;
}

NodeIdentifier::NodeIdentifier(string value) { name = value; }

NodeINT::NodeINT(int value) { this->value = value; }

string NodeINT::print_INT(NodeINT *INT, int indent) {
  return tab(indent) + "{ int_lit: " + to_string(INT->value) + " }\n";
}

string NodeIdentifier::print_identifier(NodeIdentifier *identifier,
                                        int indent) {
  return tab(indent) + "{ identifier: " + identifier->name + " }\n";
}

string NodeLet::print_let(NodeLet *let, int indent) {
  return tab(indent) + "{ let: \n" +
         NodeIdentifier::print_identifier(let->identifier, indent + 1) +
         NodeINT::print_INT(let->INT, indent + 1) + tab(indent) + "}\n";
}

string NodeDebug::print_debug(NodeDebug *debug, int indent) {
  if (debug->INT) {
    return tab(indent) + "{ debug: \n" +
           NodeINT::print_INT(debug->INT, indent + 1) + tab(indent) + "}\n";
  }
  if (debug->identifier) {
    return tab(indent) + "{ debug: \n" +
           NodeIdentifier::print_identifier(debug->identifier, indent + 1) +
           tab(indent) + "}\n";
  }
  return "";
}

string NodeStatement::print_statement(NodeStatement *stmt, int indent) {
  if (stmt->debug)
    if (stmt->debug) {
      return tab(indent) + "{ statement: \n" +
             NodeDebug::print_debug(stmt->debug, indent + 1) + tab(indent) +
             "}\n";
    }
  if (stmt->let) {
    return tab(indent) + "{ statement: \n" +
           NodeLet::print_let(stmt->let, indent + 1) + tab(indent) + "}\n";
  }
  return "";
}

string NodeProgram::print_program(NodeProgram *program, int indent) {
  string res = "{ program: \n";
  for (auto stmt : program->stmts) {
    res += NodeStatement::print_statement(stmt, indent + 1);
  }
  res += "}";
  return res;
}

string tab(int n) {
  string s = "";
  for (int i = 0; i < n; i++) {
    s += " ";
  }
  return s;
}