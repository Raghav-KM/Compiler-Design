#include "./headers/ast.h"

NodeProgram::NodeProgram(NodeStatementList *stmt_list) {
  this->stmt_list = stmt_list;
}

NodeStatementList::NodeStatementList(vector<NodeStatement *> stmts) {
  this->stmts = stmts;
}

NodeStatement::NodeStatement(NodeDebug *debug) {
  this->let = NULL;
  this->debug = debug;
  this->IF = NULL;
  this->assign = NULL;
}

NodeStatement::NodeStatement(NodeLet *let) {
  this->let = let;
  this->debug = NULL;
  this->IF = NULL;
  this->assign = NULL;
  this->FOR = NULL;
}

NodeStatement::NodeStatement(NodeIf *IF) {
  this->let = NULL;
  this->debug = NULL;
  this->IF = IF;
  this->assign = NULL;
  this->FOR = NULL;
}

NodeStatement::NodeStatement(NodeAssign *assign) {
  this->let = NULL;
  this->debug = NULL;
  this->IF = NULL;
  this->assign = assign;
  this->FOR = NULL;
}

NodeStatement::NodeStatement(NodeFor *FOR) {
  this->let = NULL;
  this->debug = NULL;
  this->IF = NULL;
  this->assign = NULL;
  this->FOR = FOR;
}

NodeDebug::NodeDebug(NodeComparativeExpression *comp_exp) {
  this->comp_exp = comp_exp;
  this->CHAR = NULL;
}

NodeDebug::NodeDebug(NodeCHAR *CHAR) {
  this->comp_exp = NULL;
  this->CHAR = CHAR;
}

NodeLet::NodeLet(NodeIdentifier *identifier,
                 NodeComparativeExpression *comp_exp) {
  this->identifier = identifier;
  this->comp_exp = comp_exp;
}

NodeIf::NodeIf(NodeComparativeExpression *comp_exp,
               NodeStatementList *stmt_list_if,
               NodeStatementList *stmt_list_else) {
  this->comp_exp = comp_exp;
  this->stmt_list_if = stmt_list_if;
  this->stmt_list_else = stmt_list_else;
}

NodeAssign::NodeAssign(NodeIdentifier *identifier,
                       NodeComparativeExpression *comp_exp) {
  this->identifier = identifier;
  this->comp_exp = comp_exp;
}

NodeFor::NodeFor(NodeLet *let, NodeComparativeExpression *comp_exp,
                 NodeAssign *assign, NodeStatementList *stmt_list) {
  this->let = let;
  this->comp_exp = comp_exp;
  this->assign = assign;
  this->stmt_list = stmt_list;
}

NodeComparativeExpression::NodeComparativeExpression(
    NodeAdditiveExpression *add_exp) {
  this->add_exp = add_exp;
  this->comp_exp = NULL;
  this->comp_operator = NULL;
}

NodeComparativeExpression::NodeComparativeExpression(
    NodeComparativeExpression *comp_exp, NodeComparativeOperator *comp_operator,
    NodeAdditiveExpression *add_exp) {
  this->add_exp = add_exp;
  this->comp_exp = comp_exp;
  this->comp_operator = comp_operator;
}

NodeAdditiveExpression::NodeAdditiveExpression(
    NodeAdditiveExpression *add_exp, NodeAdditiveOperator *add_operator,
    NodeMultiplicativeExpression *mul_exp) {
  this->add_exp = add_exp;
  this->add_operator = add_operator;
  this->mul_exp = mul_exp;
}

NodeAdditiveExpression::NodeAdditiveExpression(
    NodeMultiplicativeExpression *mul_exp) {
  this->mul_exp = mul_exp;
  this->add_exp = NULL;
  this->add_operator = NULL;
}

NodeMultiplicativeExpression::NodeMultiplicativeExpression(
    NodeMultiplicativeExpression *mul_exp,
    NodeMultiplicativeOperator *mul_operator, NodeExpression *exp) {
  this->mul_exp = mul_exp;
  this->mul_operator = mul_operator;
  this->exp = exp;
}

NodeMultiplicativeExpression::NodeMultiplicativeExpression(
    NodeExpression *exp) {
  this->exp = exp;
  this->mul_exp = NULL;
  this->mul_operator = NULL;
}

NodeExpression::NodeExpression(NodeIdentifier *identifier) {
  this->INT = NULL;
  this->identifier = identifier;
}

NodeExpression::NodeExpression(NodeINT *INT) {
  this->INT = INT;
  this->identifier = NULL;
}

NodeComparativeOperator::NodeComparativeOperator(string op) { this->op = op; }

NodeAdditiveOperator::NodeAdditiveOperator(char op) { this->op = op; }

NodeMultiplicativeOperator::NodeMultiplicativeOperator(char op) {
  this->op = op;
}

NodeIdentifier::NodeIdentifier(string value) { name = value; }

NodeINT::NodeINT(int value) { this->value = value; }

NodeCHAR::NodeCHAR(int value) { this->value = value; }

string NodeProgram::print(NodeProgram *program, int indent) {
  string res = "[ 'program': \n";
  res += NodeStatementList::print(program->stmt_list, indent + 1);
  // for (auto stmt : program->stmts) {
  //   res += NodeStatement::print(stmt, indent + 1);
  // }
  res += "]";
  return res;
}

string NodeStatementList::print(NodeStatementList *stmt_list, int indent) {
  string res = "";
  for (auto stmt : stmt_list->stmts) {
    res += NodeStatement::print(stmt, indent);
  }
  return res;
}

string NodeStatement::print(NodeStatement *stmt, int indent) {
  if (stmt->debug)
    if (stmt->debug) {
      return tab(indent) + "[ 'statement': \n" +
             NodeDebug::print(stmt->debug, indent + 1) + tab(indent) + "]\n";
    }
  if (stmt->let) {
    return tab(indent) + "[ 'statement': \n" +
           NodeLet::print(stmt->let, indent + 1) + tab(indent) + "]\n";
  }
  if (stmt->IF) {
    return tab(indent) + "[ 'statment': \n" +
           NodeIf::print(stmt->IF, indent + 1) + tab(indent) + "]\n";
  }
  if (stmt->assign) {
    return tab(indent) + "[ 'statment': \n" +
           NodeAssign::print(stmt->assign, indent + 1) + tab(indent) + "]\n";
  }
  if (stmt->FOR) {
    return tab(indent) + "[ 'statment': \n" +
           NodeFor::print(stmt->FOR, indent + 1) + tab(indent) + "]\n";
  }
  return "";
}

string NodeLet::print(NodeLet *let, int indent) {
  return tab(indent) + "[ 'let': \n" +
         NodeIdentifier::print(let->identifier, indent + 1) + " " +
         NodeComparativeExpression::print(let->comp_exp, indent + 1) +
         tab(indent) + "]\n";
}

string NodeIf::print(NodeIf *IF, int indent) {
  string str = tab(indent) + "[ 'if': \n" +
               NodeComparativeExpression::print(IF->comp_exp, indent + 1) +
               tab(indent + 1) + "[ 'true' : \n" +
               NodeStatementList::print(IF->stmt_list_if, indent + 2) +
               tab(indent + 1) + "]\n";
  if (IF->stmt_list_else) {
    str += tab(indent + 1) + "[ 'false' : \n" +
           NodeStatementList::print(IF->stmt_list_else, indent + 2) +
           tab(indent + 1) + "]\n";
  }
  return str + tab(indent) + "]\n";
}

string NodeAssign::print(NodeAssign *assign, int indent) {
  return tab(indent) + "[ 'assign': \n" +
         NodeIdentifier::print(assign->identifier, indent + 1) + " " +
         NodeComparativeExpression::print(assign->comp_exp, indent + 1) +
         tab(indent) + "]\n";
}

string NodeFor::print(NodeFor *FOR, int indent) {
  return tab(indent) + "[ 'for': \n" + NodeLet::print(FOR->let, indent + 1) +
         NodeComparativeExpression::print(FOR->comp_exp, indent + 1) +
         NodeAssign::print(FOR->assign, indent + 1) + tab(indent + 1) +
         "[ 'statment_list' : \n" +
         NodeStatementList::print(FOR->stmt_list, indent + 2) +
         tab(indent + 1) + "]\n" + tab(indent) + "]\n";
}

string NodeComparativeExpression::print(NodeComparativeExpression *comp_exp,
                                        int indent) {
  if (comp_exp->comp_operator != NULL && comp_exp->add_exp != NULL) {
    return tab(indent) + "[ 'comp_expression': \n" +
           NodeComparativeExpression::print(comp_exp->comp_exp, indent + 1) +
           NodeComparativeOperator::print(comp_exp->comp_operator, indent + 1) +
           NodeAdditiveExpression::print(comp_exp->add_exp, indent + 1) +
           tab(indent) + "]\n";
  } else {
    return tab(indent) + "[ 'comp_expression': \n" +
           NodeAdditiveExpression::print(comp_exp->add_exp, indent + 1) +
           tab(indent) + "]\n";
  }
}

string NodeAdditiveExpression::print(NodeAdditiveExpression *add_exp,
                                     int indent) {
  if (add_exp->add_operator != NULL && add_exp->add_exp != NULL) {
    return tab(indent) + "[ 'add_expression': \n" +
           NodeAdditiveExpression::print(add_exp->add_exp, indent + 1) +
           NodeAdditiveOperator::print(add_exp->add_operator, indent + 1) +
           NodeMultiplicativeExpression::print(add_exp->mul_exp, indent + 1) +
           tab(indent) + "]\n";
  } else {
    return tab(indent) + "[ 'add_expression': \n" +
           NodeMultiplicativeExpression::print(add_exp->mul_exp, indent + 1) +
           tab(indent) + "]\n";
  }
}

string
NodeMultiplicativeExpression::print(NodeMultiplicativeExpression *mul_exp,
                                    int indent) {
  if (mul_exp->mul_operator != NULL && mul_exp->mul_exp != NULL) {
    return tab(indent) + "[ 'mul_expression': \n" +
           NodeMultiplicativeExpression::print(mul_exp->mul_exp, indent + 1) +
           NodeMultiplicativeOperator::print(mul_exp->mul_operator,
                                             indent + 1) +
           NodeExpression::print(mul_exp->exp, indent + 1) + tab(indent) +
           "]\n";
  } else {
    return tab(indent) + "[ 'mul_expression': \n" +
           NodeExpression::print(mul_exp->exp, indent + 1) + tab(indent) +
           "]\n";
  }
}

string NodeExpression::print(NodeExpression *expression, int indent) {
  if (expression->INT) {
    return tab(indent) + "[ 'expression': \n" +
           NodeINT::print(expression->INT, indent + 1) + tab(indent) + "]\n";
  }
  if (expression->identifier) {
    return tab(indent) + "[ 'expression': \n" +
           NodeIdentifier::print(expression->identifier, indent + 1) +
           tab(indent) + "]\n";
  }
  return "";
}

string NodeComparativeOperator::print(NodeComparativeOperator *comp_op,
                                      int indent) {
  return tab(indent) + "[ 'comp_operator': " + (comp_op->op) + " ]\n";
}

string NodeAdditiveOperator::print(NodeAdditiveOperator *add_op, int indent) {
  return tab(indent) + "[ 'add_operator': " + (add_op->op) + " ]\n";
}

string NodeMultiplicativeOperator::print(NodeMultiplicativeOperator *mul_op,
                                         int indent) {
  return tab(indent) + "[ 'mul_operator': " + (mul_op->op) + " ]\n";
}

string NodeIdentifier::print(NodeIdentifier *identifier, int indent) {
  return tab(indent) + "[ 'identifier': " + identifier->name + " ]\n";
}

string NodeINT::print(NodeINT *INT, int indent) {
  return tab(indent) + "[ 'int_lit': " + to_string(INT->value) + " ]\n";
}

string NodeCHAR::print(NodeCHAR *CHAR, int indent) {
  return tab(indent) + "[ 'char_lit': " + to_string(CHAR->value) + " ]\n";
}

string NodeDebug::print(NodeDebug *debug, int indent) {
  if (debug->comp_exp) {
    return tab(indent) + "[ 'debug': \n" +
           NodeComparativeExpression::print(debug->comp_exp, indent + 1) +
           tab(indent) + "]\n";
  } else if (debug->CHAR) {
    return tab(indent) + "[ 'debug': \n" +
           NodeCHAR::print(debug->CHAR, indent + 1) + tab(indent) + "]\n";
  }
  return "";
}

string tab(int n) {
  string s = "";
  for (int i = 0; i < n; i++) {
    s += " ";
  }
  return s;
}