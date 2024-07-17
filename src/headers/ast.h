#ifndef AST_H
#define AST_H

#include "utils.h"
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

class NodeStatementList;

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

class NodeComparativeOperator {
  friend class Parser;
  friend class Codegen;
  string op;

public:
  NodeComparativeOperator(string op);
  static string print(NodeComparativeOperator *comp_op, int indent);
};

class NodeAdditiveOperator {
  friend class Parser;
  friend class Codegen;
  char op;

public:
  NodeAdditiveOperator(char op);
  static string print(NodeAdditiveOperator *add_op, int indent);
};

class NodeMultiplicativeOperator {
  friend class Parser;
  friend class Codegen;
  char op;

public:
  NodeMultiplicativeOperator(char op);
  static string print(NodeMultiplicativeOperator *mul_op, int indent);
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

class NodeMultiplicativeExpression {
  friend class Parser;
  friend class Codegen;

  NodeMultiplicativeExpression *mul_exp;
  NodeMultiplicativeOperator *mul_operator;
  NodeExpression *exp;

public:
  NodeMultiplicativeExpression(NodeMultiplicativeExpression *mul_exp,
                               NodeMultiplicativeOperator *mul_operators,
                               NodeExpression *exp);

  NodeMultiplicativeExpression(NodeExpression *exp);
  static string print(NodeMultiplicativeExpression *mul_exp, int indent);
};

class NodeAdditiveExpression {
  friend class Parser;
  friend class Codegen;

  NodeAdditiveExpression *add_exp;
  NodeAdditiveOperator *add_operator;
  NodeMultiplicativeExpression *mul_exp;

public:
  NodeAdditiveExpression(NodeAdditiveExpression *add_exp,
                         NodeAdditiveOperator *add_operator,
                         NodeMultiplicativeExpression *mul_exp);

  NodeAdditiveExpression(NodeMultiplicativeExpression *mul_exp);

  static string print(NodeAdditiveExpression *add_exp, int indent);
};

class NodeComparativeExpression {
  friend class Parser;
  friend class Codegen;

  NodeComparativeExpression *comp_exp;
  NodeComparativeOperator *comp_operator;
  NodeAdditiveExpression *add_exp;

public:
  NodeComparativeExpression(NodeComparativeExpression *comp_exp,
                            NodeComparativeOperator *comp_operator,
                            NodeAdditiveExpression *add_exp);

  NodeComparativeExpression(NodeAdditiveExpression *add_exp);

  static string print(NodeComparativeExpression *comp_exp, int indent);
};

class NodeAssign {
  friend class Parser;
  friend class Codegen;

  NodeIdentifier *identifier;
  NodeComparativeExpression *comp_exp;

public:
  NodeAssign(NodeIdentifier *identifier, NodeComparativeExpression *comp_exp);
  static string print(NodeAssign *assign, int indent);
};

class NodeLet {
  friend class Parser;
  friend class Codegen;
  NodeIdentifier *identifier;
  NodeComparativeExpression *comp_exp;

public:
  NodeLet(NodeIdentifier *identifier, NodeComparativeExpression *comp_exp);
  static string print_let(NodeLet *let, int indent);
};

class NodeFor {
  friend class Parser;
  friend class Codegen;

  NodeLet *let;
  NodeComparativeExpression *comp_exp;
  NodeAssign *assign;
  NodeStatementList *stmt_list;

public:
  NodeFor(NodeLet *let, NodeComparativeExpression *comp_exp, NodeAssign *assign,
          NodeStatementList *stmt_list);
  static string print(NodeFor *FOR, int indent);
};

class NodeDebug {
  friend class Parser;
  friend class Codegen;
  NodeComparativeExpression *comp_exp;

public:
  NodeDebug(NodeComparativeExpression *comp_exp);
  static string print_debug(NodeDebug *debug, int indent);
};

class NodeIf {
  friend class Parser;
  friend class Codegen;

  NodeComparativeExpression *comp_exp;
  NodeStatementList *stmt_list_if;
  NodeStatementList *stmt_list_else;

public:
  NodeIf(NodeComparativeExpression *comp_exp, NodeStatementList *stmt_list_if,
         NodeStatementList *stmt_list_else);
  static string print_if(NodeIf *If, int indent);
};

class NodeStatement {
  friend class Parser;
  friend class Codegen;
  NodeDebug *debug;
  NodeLet *let;
  NodeIf *IF;
  NodeAssign *assign;
  NodeFor *FOR;

public:
  NodeStatement(NodeDebug *debug);
  NodeStatement(NodeLet *let);
  NodeStatement(NodeIf *IF);
  NodeStatement(NodeAssign *assign);
  NodeStatement(NodeFor *FOR);

  static string print_statement(NodeStatement *stmt, int indent);
};

class NodeStatementList {
  friend class Parser;
  friend class Codegen;

public:
  vector<NodeStatement *> stmts;
  static string print_statement_list(NodeStatementList *stmt, int indent);
};

class NodeProgram {
  friend class Parser;
  friend class Codegen;
  NodeStatementList *stmt_list;

public:
  static string print_program(NodeProgram *program, int indent);
};

string tab(int n);

#endif