#ifndef AST_H
#define AST_H

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

class NodeINT {
  friend class Parser;
  int value;

public:
  NodeINT(int value);
  static string print_INT(NodeINT *INT, int indent);
};

class NodeIdentifier {
  friend class Parser;
  string name;

public:
  NodeIdentifier(string name);
  static string print_identifier(NodeIdentifier *identifier, int indent);
};

class NodeLet {
  friend class Parser;
  NodeINT *INT;
  NodeIdentifier *identifier;

public:
  NodeLet(NodeINT *INT, NodeIdentifier *identifier);
  static string print_let(NodeLet *let, int indent);
};

class NodeDebug {
  friend class Parser;
  NodeINT *INT;
  NodeIdentifier *identifier;

public:
  NodeDebug(NodeINT *INT);
  NodeDebug(NodeIdentifier *identifier);
  static string print_debug(NodeDebug *debug, int indent);
};

class NodeStatement {
  friend class Parser;
  NodeDebug *debug;
  NodeLet *let;

public:
  NodeStatement(NodeDebug *debug);
  NodeStatement(NodeLet *let);

  static string print_statement(NodeStatement *stmt, int indent);
};

class NodeProgram {
  friend class Parser;
  vector<NodeStatement *> stmts;

public:
  static string print_program(NodeProgram *program, int indent);
};

string tab(int n);

#endif