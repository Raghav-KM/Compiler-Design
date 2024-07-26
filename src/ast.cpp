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
  this->type = add_exp->type;
}

NodeComparativeExpression::NodeComparativeExpression(
    NodeComparativeExpression *comp_exp, NodeComparativeOperator *comp_operator,
    NodeAdditiveExpression *add_exp) {
  this->add_exp = add_exp;
  this->comp_exp = comp_exp;
  this->comp_operator = comp_operator;
  this->type = max_datatype(comp_exp->type, add_exp->type);
}

NodeAdditiveExpression::NodeAdditiveExpression(
    NodeAdditiveExpression *add_exp, NodeAdditiveOperator *add_operator,
    NodeMultiplicativeExpression *mul_exp) {
  this->add_exp = add_exp;
  this->add_operator = add_operator;
  this->mul_exp = mul_exp;
  this->type = max_datatype(add_exp->type, mul_exp->type);
}

NodeAdditiveExpression::NodeAdditiveExpression(
    NodeMultiplicativeExpression *mul_exp) {
  this->mul_exp = mul_exp;
  this->add_exp = NULL;
  this->add_operator = NULL;
  this->type = mul_exp->type;
}

NodeMultiplicativeExpression::NodeMultiplicativeExpression(
    NodeMultiplicativeExpression *mul_exp,
    NodeMultiplicativeOperator *mul_operator, NodeExpression *exp) {
  this->mul_exp = mul_exp;
  this->mul_operator = mul_operator;
  this->exp = exp;
  this->type = max_datatype(mul_exp->type, exp->type);
}

NodeMultiplicativeExpression::NodeMultiplicativeExpression(
    NodeExpression *exp) {
  this->exp = exp;
  this->mul_exp = NULL;
  this->mul_operator = NULL;
  this->type = exp->type;
}

NodeExpression::NodeExpression(NodeIdentifier *identifier) {
  this->literal = NULL;
  this->identifier = identifier;
  this->type = identifier->type;
}

NodeExpression::NodeExpression(NodeLiteral *literal) {
  this->literal = literal;
  this->type = literal->type;
  this->identifier = NULL;
}

NodeComparativeOperator::NodeComparativeOperator(string op) { this->op = op; }

NodeAdditiveOperator::NodeAdditiveOperator(char op) { this->op = op; }

NodeMultiplicativeOperator::NodeMultiplicativeOperator(char op) {
  this->op = op;
}

NodeIdentifier::NodeIdentifier(string value) { name = value; }

NodeLiteral::NodeLiteral(int value, DATA_TYPES type) {
  this->value = value;
  this->type = type;
}

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
         NodeIdentifier::print(let->identifier, indent + 1) +
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
  string type = "undefined";
  if (comp_exp->type == INT) {
    type = "integer";
  } else if (comp_exp->type == CHAR) {
    type = "character";
  } else if (comp_exp->type == UNDEFINED) {
    type = "undefined";
  }

  if (comp_exp->comp_operator != NULL && comp_exp->add_exp != NULL) {
    return tab(indent) + "[ 'comp_expression': " + type + "\n" +
           NodeComparativeExpression::print(comp_exp->comp_exp, indent + 1) +
           NodeComparativeOperator::print(comp_exp->comp_operator, indent + 1) +
           NodeAdditiveExpression::print(comp_exp->add_exp, indent + 1) +
           tab(indent) + "]\n";
  } else {
    return tab(indent) + "[ 'comp_expression': " + type + "\n" +
           NodeAdditiveExpression::print(comp_exp->add_exp, indent + 1) +
           tab(indent) + "]\n";
  }
}

string NodeAdditiveExpression::print(NodeAdditiveExpression *add_exp,
                                     int indent) {

  string type = "";
  if (add_exp->type == INT) {
    type = "integer";
  } else if (add_exp->type == CHAR) {
    type = "character";
  } else if (add_exp->type == UNDEFINED) {
    type = "undefined";
  }

  if (add_exp->add_operator != NULL && add_exp->add_exp != NULL) {
    return tab(indent) + "[ 'add_expression': " + type + "\n" +
           NodeAdditiveExpression::print(add_exp->add_exp, indent + 1) +
           NodeAdditiveOperator::print(add_exp->add_operator, indent + 1) +
           NodeMultiplicativeExpression::print(add_exp->mul_exp, indent + 1) +
           tab(indent) + "]\n";
  } else {
    return tab(indent) + "[ 'add_expression': " + type + "\n" +
           NodeMultiplicativeExpression::print(add_exp->mul_exp, indent + 1) +
           tab(indent) + "]\n";
  }
}

string
NodeMultiplicativeExpression::print(NodeMultiplicativeExpression *mul_exp,
                                    int indent) {
  string type = "";
  if (mul_exp->type == INT) {
    type = "integer";
  } else if (mul_exp->type == CHAR) {
    type = "character";
  } else if (mul_exp->type == UNDEFINED) {
    type = "undefined";
  }

  if (mul_exp->mul_operator != NULL && mul_exp->mul_exp != NULL) {
    return tab(indent) + "[ 'mul_expression': " + type + "\n" +
           NodeMultiplicativeExpression::print(mul_exp->mul_exp, indent + 1) +
           NodeMultiplicativeOperator::print(mul_exp->mul_operator,
                                             indent + 1) +
           NodeExpression::print(mul_exp->exp, indent + 1) + tab(indent) +
           "]\n";
  } else {
    return tab(indent) + "[ 'mul_expression': " + type + "\n" +
           NodeExpression::print(mul_exp->exp, indent + 1) + tab(indent) +
           "]\n";
  }
}

string NodeExpression::print(NodeExpression *expression, int indent) {
  string type = "";
  if (expression->type == INT) {
    type = "integer";
  } else if (expression->type == CHAR) {
    type = "character";
  } else if (expression->type == UNDEFINED) {
    type = "undefined";
  }

  if (expression->literal) {
    return tab(indent) + "[ 'expression': " + type + "\n" +
           NodeLiteral::print(expression->literal, indent + 1) + tab(indent) +
           "]\n";
  }
  if (expression->identifier) {
    return tab(indent) + "[ 'expression': " + type + "\n" +
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
  string str = tab(indent) + "[ 'identifier': " + identifier->name;
  if (identifier->type == CHAR) {
    str += " (character)";
  }
  if (identifier->type == INT) {
    str += " (integer)";
  }

  str += " ]\n";
  return str;
}

string NodeLiteral::print(NodeLiteral *literal, int indent) {
  if (literal->type == CHAR) {
    return tab(indent) + "[ 'char_literal': " + to_string(literal->value) +
           " ]\n";
  }
  if (literal->type == INT) {
    return tab(indent) + "[ 'int_literal': " + to_string(literal->value) +
           " ]\n";
  }
  return "";
}

string NodeDebug::print(NodeDebug *debug, int indent) {
  if (debug->comp_exp) {
    return tab(indent) + "[ 'debug': \n" +
           NodeComparativeExpression::print(debug->comp_exp, indent + 1) +
           tab(indent) + "]\n";
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