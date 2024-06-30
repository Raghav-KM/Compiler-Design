#ifndef AST_H
#define AST_H

#include "utils.h"
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

class NodeINT {
  friend class Parser;
  friend class Codegen;
  int value;

public:
  NodeINT(int value);
  static string print_INT(NodeINT *INT, int indent);
};

class NodeIdentifier {
  friend class Parser;
  friend class Codegen;
  string name;

public:
  NodeIdentifier(string name);
  static string print_identifier(NodeIdentifier *identifier, int indent);
};

class NodeExpression {
  friend class Parser;
  friend class Codegen;

  NodeINT *INT;
  NodeIdentifier *identifier;

public:
  NodeExpression(NodeIdentifier *identifier);
  NodeExpression(NodeINT *INT);
  static string print_expression(NodeExpression *expression, int indent);
};

class NodeLet {
  friend class Parser;
  friend class Codegen;
  NodeIdentifier *identifier;
  NodeExpression *expression;

public:
  NodeLet(NodeIdentifier *identifier, NodeExpression *exp);
  static string print_let(NodeLet *let, int indent);
};

class NodeDebug {
  friend class Parser;
  friend class Codegen;
  NodeExpression *expression;

public:
  NodeDebug(NodeExpression *expression);
  static string print_debug(NodeDebug *debug, int indent);
};

class NodeStatement {
  friend class Parser;
  friend class Codegen;
  NodeDebug *debug;
  NodeLet *let;

public:
  NodeStatement(NodeDebug *debug);
  NodeStatement(NodeLet *let);

  static string print_statement(NodeStatement *stmt, int indent);
};

class NodeProgram {
  friend class Parser;
  friend class Codegen;
  vector<NodeStatement *> stmts;

public:
  static string print_program(NodeProgram *program, int indent);
};

string tab(int n);

#endif