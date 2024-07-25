#ifndef AST_H
#define AST_H

#include "utils.h"
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

class NodeProgram;

class NodeStatementList;
class NodeStatement;

class NodeDebug;
class NodeLet;
class NodeIf;
class NodeAssign;
class NodeFor;

class NodeComparativeExpression;
class NodeAdditiveExpression;
class NodeMultiplicativeExpression;
class NodeExpression;

class NodeComparativeOperator;
class NodeAdditiveOperator;
class NodeMultiplicativeOperator;

class NodeIdentifier;
class NodeINT;
class NodeCHAR;

class NodeProgram {
public:
  NodeStatementList *stmt_list;

  NodeProgram(NodeStatementList *stmt_list);
  static string print(NodeProgram *program, int indent);
};

class NodeStatementList {
public:
  vector<NodeStatement *> stmts;

  NodeStatementList(vector<NodeStatement *> stmts);
  static string print(NodeStatementList *stmt, int indent);
};

class NodeStatement {
public:
  NodeDebug *debug;
  NodeLet *let;
  NodeIf *IF;
  NodeAssign *assign;
  NodeFor *FOR;

  NodeStatement(NodeDebug *debug);
  NodeStatement(NodeLet *let);
  NodeStatement(NodeIf *IF);
  NodeStatement(NodeAssign *assign);
  NodeStatement(NodeFor *FOR);

  static string print(NodeStatement *stmt, int indent);
};

class NodeDebug {
public:
  NodeComparativeExpression *comp_exp;
  NodeCHAR *CHAR;

  NodeDebug(NodeComparativeExpression *comp_exp);
  NodeDebug(NodeCHAR *CHAR);

  static string print(NodeDebug *debug, int indent);
};

class NodeLet {
public:
  NodeIdentifier *identifier;
  NodeComparativeExpression *comp_exp;

  NodeLet(NodeIdentifier *identifier, NodeComparativeExpression *comp_exp);
  static string print(NodeLet *let, int indent);
};

class NodeIf {
public:
  NodeComparativeExpression *comp_exp;
  NodeStatementList *stmt_list_if;
  NodeStatementList *stmt_list_else;

  NodeIf(NodeComparativeExpression *comp_exp, NodeStatementList *stmt_list_if,
         NodeStatementList *stmt_list_else);
  static string print(NodeIf *If, int indent);
};

class NodeAssign {
public:
  NodeIdentifier *identifier;
  NodeComparativeExpression *comp_exp;

  NodeAssign(NodeIdentifier *identifier, NodeComparativeExpression *comp_exp);
  static string print(NodeAssign *assign, int indent);
};

class NodeFor {
public:
  NodeLet *let;
  NodeComparativeExpression *comp_exp;
  NodeAssign *assign;
  NodeStatementList *stmt_list;

  NodeFor(NodeLet *let, NodeComparativeExpression *comp_exp, NodeAssign *assign,
          NodeStatementList *stmt_list);
  static string print(NodeFor *FOR, int indent);
};

class NodeComparativeExpression {
public:
  NodeComparativeExpression *comp_exp;
  NodeComparativeOperator *comp_operator;
  NodeAdditiveExpression *add_exp;
  NodeComparativeExpression(NodeComparativeExpression *comp_exp,
                            NodeComparativeOperator *comp_operator,
                            NodeAdditiveExpression *add_exp);

  NodeComparativeExpression(NodeAdditiveExpression *add_exp);

  static string print(NodeComparativeExpression *comp_exp, int indent);
};

class NodeAdditiveExpression {
public:
  NodeAdditiveExpression *add_exp;
  NodeAdditiveOperator *add_operator;
  NodeMultiplicativeExpression *mul_exp;
  NodeAdditiveExpression(NodeAdditiveExpression *add_exp,
                         NodeAdditiveOperator *add_operator,
                         NodeMultiplicativeExpression *mul_exp);

  NodeAdditiveExpression(NodeMultiplicativeExpression *mul_exp);

  static string print(NodeAdditiveExpression *add_exp, int indent);
};

class NodeMultiplicativeExpression {
public:
  NodeMultiplicativeExpression *mul_exp;
  NodeMultiplicativeOperator *mul_operator;
  NodeExpression *exp;

  NodeMultiplicativeExpression(NodeMultiplicativeExpression *mul_exp,
                               NodeMultiplicativeOperator *mul_operators,
                               NodeExpression *exp);

  NodeMultiplicativeExpression(NodeExpression *exp);
  static string print(NodeMultiplicativeExpression *mul_exp, int indent);
};

class NodeExpression {
public:
  NodeINT *INT;
  NodeIdentifier *identifier;

  NodeExpression(NodeIdentifier *identifier);
  NodeExpression(NodeINT *INT);
  static string print(NodeExpression *expression, int indent);
};

class NodeComparativeOperator {
public:
  string op;
  NodeComparativeOperator(string op);
  static string print(NodeComparativeOperator *comp_op, int indent);
};

class NodeAdditiveOperator {
public:
  char op;
  NodeAdditiveOperator(char op);
  static string print(NodeAdditiveOperator *add_op, int indent);
};

class NodeMultiplicativeOperator {
public:
  char op;
  NodeMultiplicativeOperator(char op);
  static string print(NodeMultiplicativeOperator *mul_op, int indent);
};

class NodeIdentifier {
public:
  string name;

  NodeIdentifier(string name);
  static string print(NodeIdentifier *identifier, int indent);
};

class NodeINT {
public:
  int value;
  NodeINT(int value);
  static string print(NodeINT *INT, int indent);
};

class NodeCHAR {
public:
  int value;
  NodeCHAR(int value);
  static string print(NodeCHAR *CHAR, int indent);
};

string tab(int n);

#endif