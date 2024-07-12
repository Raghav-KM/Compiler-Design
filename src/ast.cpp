#include "./headers/ast.h"

NodeStatement::NodeStatement(NodeDebug *debug) {
  this->let = NULL;
  this->debug = debug;
  this->IF = NULL;
}
NodeStatement::NodeStatement(NodeLet *let) {
  this->let = let;
  this->debug = NULL;
  this->IF = NULL;
}
NodeStatement::NodeStatement(NodeIf *IF) {
  this->let = NULL;
  this->debug = NULL;
  this->IF = IF;
}

NodeDebug::NodeDebug(NodeAdditiveExpression *add_exp) {
  this->add_exp = add_exp;
}

NodeLet::NodeLet(NodeIdentifier *identifier, NodeAdditiveExpression *add_exp) {
  this->identifier = identifier;
  this->add_exp = add_exp;
}
NodeIf::NodeIf(NodeAdditiveExpression *add_exp, NodeStatementList *stmt_list_if,
               NodeStatementList *stmt_list_else) {
  this->add_exp = add_exp;
  this->stmt_list_if = stmt_list_if;
  this->stmt_list_else = stmt_list_else;
}

NodeExpression::NodeExpression(NodeIdentifier *identifier) {
  this->INT = NULL;
  this->identifier = identifier;
}
NodeExpression::NodeExpression(NodeINT *INT) {
  this->INT = INT;
  this->identifier = NULL;
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

NodeIdentifier::NodeIdentifier(string value) { name = value; }

NodeINT::NodeINT(int value) { this->value = value; }

NodeAdditiveOperator::NodeAdditiveOperator(char op) { this->op = op; }

NodeMultiplicativeOperator::NodeMultiplicativeOperator(char op) {
  this->op = op;
}

string NodeINT::print_INT(NodeINT *INT, int indent) {
  return tab(indent) + "[ 'int_lit': " + to_string(INT->value) + " ]\n";
}

string NodeIdentifier::print_identifier(NodeIdentifier *identifier,
                                        int indent) {
  return tab(indent) + "[ 'identifier': " + identifier->name + " ]\n";
}

string NodeExpression::print_expression(NodeExpression *expression,
                                        int indent) {
  if (expression->INT) {
    return tab(indent) + "[ 'expression': \n" +
           NodeINT::print_INT(expression->INT, indent + 1) + tab(indent) +
           "]\n";
  }
  if (expression->identifier) {
    return tab(indent) + "[ 'expression': \n" +
           NodeIdentifier::print_identifier(expression->identifier,
                                            indent + 1) +
           tab(indent) + "]\n";
  }
  return "";
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

string NodeAdditiveOperator::print(NodeAdditiveOperator *add_op, int indent) {
  return tab(indent) + "[ 'add_operator': " + (add_op->op) + " ]\n";
}

string
NodeMultiplicativeExpression::print(NodeMultiplicativeExpression *mul_exp,
                                    int indent) {
  if (mul_exp->mul_operator != NULL && mul_exp->mul_exp != NULL) {
    return tab(indent) + "[ 'mul_expression': \n" +
           NodeMultiplicativeExpression::print(mul_exp->mul_exp, indent + 1) +
           NodeMultiplicativeOperator::print(mul_exp->mul_operator,
                                             indent + 1) +
           NodeExpression::print_expression(mul_exp->exp, indent + 1) +
           tab(indent) + "]\n";
  } else {
    return tab(indent) + "[ 'mul_expression': \n" +
           NodeExpression::print_expression(mul_exp->exp, indent + 1) +
           tab(indent) + "]\n";
  }
}
string NodeMultiplicativeOperator::print(NodeMultiplicativeOperator *mul_op,
                                         int indent) {
  return tab(indent) + "[ 'mul_operator': " + (mul_op->op) + " ]\n";
}

string NodeLet::print_let(NodeLet *let, int indent) {
  return tab(indent) + "[ 'let': \n" +
         NodeIdentifier::print_identifier(let->identifier, indent + 1) + " " +
         NodeAdditiveExpression::print(let->add_exp, indent + 1) + tab(indent) +
         "]\n";
}

string NodeDebug::print_debug(NodeDebug *debug, int indent) {
  return tab(indent) + "[ 'debug': \n" +
         NodeAdditiveExpression::print(debug->add_exp, indent + 1) +
         tab(indent) + "]\n";
}

string NodeIf::print_if(NodeIf *IF, int indent) {
  return tab(indent) + "[ 'if': \n" +
         NodeAdditiveExpression::print(IF->add_exp, indent + 1) +
         tab(indent + 1) + "[ 'true' : \n" +
         NodeStatementList::print_statement_list(IF->stmt_list_if, indent + 2) +
         tab(indent + 1) + "]\n" + tab(indent + 1) + "[ 'false' : \n" +
         NodeStatementList::print_statement_list(IF->stmt_list_else,
                                                 indent + 2) +
         tab(indent + 1) + "]\n" + tab(indent) + "]\n";
}

string NodeStatement::print_statement(NodeStatement *stmt, int indent) {
  if (stmt->debug)
    if (stmt->debug) {
      return tab(indent) + "[ 'statement': \n" +
             NodeDebug::print_debug(stmt->debug, indent + 1) + tab(indent) +
             "]\n";
    }
  if (stmt->let) {
    return tab(indent) + "[ 'statement': \n" +
           NodeLet::print_let(stmt->let, indent + 1) + tab(indent) + "]\n";
  }
  if (stmt->IF) {
    return tab(indent) + "[ 'statment': \n" +
           NodeIf::print_if(stmt->IF, indent + 1) + tab(indent) + "]\n";
  }
  return "";
}
string NodeStatementList::print_statement_list(NodeStatementList *stmt_list,
                                               int indent) {
  string res = "";
  for (auto stmt : stmt_list->stmts) {
    res += NodeStatement::print_statement(stmt, indent);
  }
  return res;
}

string NodeProgram::print_program(NodeProgram *program, int indent) {
  string res = "[ 'program': \n";
  res +=
      NodeStatementList::print_statement_list(program->stmt_list, indent + 1);
  // for (auto stmt : program->stmts) {
  //   res += NodeStatement::print_statement(stmt, indent + 1);
  // }
  res += "]";
  return res;
}

string tab(int n) {
  string s = "";
  for (int i = 0; i < n; i++) {
    s += " ";
  }
  return s;
}