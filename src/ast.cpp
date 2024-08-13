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
  this->function = NULL;
  this->function_call = NULL;
}

NodeStatement::NodeStatement(NodeLet *let) {
  this->let = let;
  this->debug = NULL;
  this->IF = NULL;
  this->assign = NULL;
  this->FOR = NULL;
  this->function = NULL;
  this->function_call = NULL;
}

NodeStatement::NodeStatement(NodeIf *IF) {
  this->let = NULL;
  this->debug = NULL;
  this->IF = IF;
  this->assign = NULL;
  this->FOR = NULL;
  this->function = NULL;
  this->function_call = NULL;
}

NodeStatement::NodeStatement(NodeAssign *assign) {
  this->let = NULL;
  this->debug = NULL;
  this->IF = NULL;
  this->assign = assign;
  this->FOR = NULL;
  this->function = NULL;
  this->function_call = NULL;
}

NodeStatement::NodeStatement(NodeFor *FOR) {
  this->let = NULL;
  this->debug = NULL;
  this->IF = NULL;
  this->assign = NULL;
  this->FOR = FOR;
  this->function = NULL;
  this->function_call = NULL;
}

NodeStatement::NodeStatement(NodeFunction *function) {
  this->let = NULL;
  this->debug = NULL;
  this->IF = NULL;
  this->assign = NULL;
  this->FOR = NULL;
  this->function = function;
  this->function_call = NULL;
}

NodeStatement::NodeStatement(NodeFunctionCall *function_call) {
  this->let = NULL;
  this->debug = NULL;
  this->IF = NULL;
  this->assign = NULL;
  this->FOR = NULL;
  this->function = NULL;
  this->function_call = function_call;
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

NodeFunction::NodeFunction(string function_identifier,
                           NodeStatementList *stmt_list) {
  this->function_identifier = function_identifier;
  this->stmt_list = stmt_list;
}

NodeFunctionCall::NodeFunctionCall(string function_identifier) {
  this->function_identifier = function_identifier;
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
    NodeNegativeExpression *neg_exp) {
  this->add_exp = add_exp;
  this->add_operator = add_operator;
  this->neg_exp = neg_exp;
  this->type = max_datatype(add_exp->type, neg_exp->type);
}

NodeAdditiveExpression::NodeAdditiveExpression(
    NodeNegativeExpression *neg_exp) {
  this->neg_exp = neg_exp;
  this->add_exp = NULL;
  this->add_operator = NULL;
  this->type = neg_exp->type;
}

NodeNegativeExpression::NodeNegativeExpression(
    NodeNegativeExpression *neg_exp) {
  this->neg_exp = neg_exp;
  this->mul_exp = NULL;
  this->type = max_datatype(neg_exp->type, INT);
}

NodeNegativeExpression::NodeNegativeExpression(
    NodeMultiplicativeExpression *mul_exp) {
  this->neg_exp = NULL;
  this->mul_exp = mul_exp;
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
  this->comp_exp = NULL;
  this->type = identifier->type;
}

NodeExpression::NodeExpression(NodeLiteral *literal) {
  this->literal = literal;
  this->identifier = NULL;
  this->comp_exp = NULL;
  this->type = literal->type;
}

NodeExpression::NodeExpression(NodeComparativeExpression *comp_exp) {
  this->literal = NULL;
  this->identifier = NULL;
  this->comp_exp = comp_exp;
  this->type = comp_exp->type;
}

NodeComparativeOperator::NodeComparativeOperator(string op) { this->op = op; }

NodeAdditiveOperator::NodeAdditiveOperator(char op) { this->op = op; }

NodeMultiplicativeOperator::NodeMultiplicativeOperator(char op) {
  this->op = op;
}

NodeIdentifier::NodeIdentifier(string name) { this->name = name; }
NodeIdentifier::NodeIdentifier(string name, int scope) {
  this->name = name;
  this->scope = scope;
}

NodeLiteral::NodeLiteral(int value, DATA_TYPES type) {
  this->value = value;
  this->type = type;
}

string NodeProgram::print(NodeProgram *program, int indent) {
  string res = "{\"program\": {";
  res += NodeStatementList::print(program->stmt_list, indent + 1);
  // for (auto stmt : program->stmts) {
  //   res += NodeStatement::print(stmt, indent + 1);
  // }
  res += "}}";
  return res;
}

string NodeStatementList::print(NodeStatementList *stmt_list, int indent) {
  string res = "\"statement_list\":[";
  for (int i = 0; i < stmt_list->stmts.size(); i++) {
    res += NodeStatement::print(stmt_list->stmts[i], indent);
    if (i < stmt_list->stmts.size() - 1)
      res += ",";
  }
  res += "]";
  return res;
}

string NodeStatement::print(NodeStatement *stmt, int indent) {
  if (stmt->debug)
    if (stmt->debug) {
      return "{\"statement\": {" + NodeDebug::print(stmt->debug, indent + 1) +
             "}}";
    }
  if (stmt->let) {
    return "{\"statement\":{" + NodeLet::print(stmt->let, indent + 1) + "}}";
  }
  if (stmt->IF) {
    return "{\"statement\":{" + NodeIf::print(stmt->IF, indent + 1) + +"}}";
  }
  if (stmt->assign) {
    return "{\"statement\":{" + NodeAssign::print(stmt->assign, indent + 1) +
           +"}}";
  }
  if (stmt->FOR) {
    return "{\"statement\":{" + NodeFor::print(stmt->FOR, indent + 1) + +"}}";
  }
  if (stmt->function) {
    return "{\"statement\":{" +
           NodeFunction::print(stmt->function, indent + 1) + "}}";
  }
  if (stmt->function_call) {
    return "{\"statement\":" +
           NodeFunctionCall::print(stmt->function_call, indent + 1) + "}}";
  }
  return "";
}

string NodeLet::print(NodeLet *let, int indent) {
  return " \"let\":{" + NodeIdentifier::print(let->identifier, indent + 1) +
         "," + NodeComparativeExpression::print(let->comp_exp, indent + 1) +
         "}";
}

string NodeIf::print(NodeIf *IF, int indent) {
  string str = " \"if-else\":{" +
               NodeComparativeExpression::print(IF->comp_exp, indent + 1) +
               ", \"true\" :{" +
               NodeStatementList::print(IF->stmt_list_if, indent + 2) + "}";
  if (IF->stmt_list_else) {
    str += ", \"false\" :{" +
           NodeStatementList::print(IF->stmt_list_else, indent + 2) + "}";
  }
  return str + "}";
}

string NodeAssign::print(NodeAssign *assign, int indent) {
  return " \"assign\": {" +
         NodeIdentifier::print(assign->identifier, indent + 1) + "," +
         NodeComparativeExpression::print(assign->comp_exp, indent + 1) + "}";
}

string NodeFor::print(NodeFor *FOR, int indent) {
  return " \"for\":{" + NodeLet::print(FOR->let, indent + 1) + "," +
         NodeComparativeExpression::print(FOR->comp_exp, indent + 1) + "," +
         NodeAssign::print(FOR->assign, indent + 1) + "," +
         NodeStatementList::print(FOR->stmt_list, indent + 2) + "}";
}

string NodeFunction::print(NodeFunction *function, int indent) {
  return "\"function\" :{ \"name\": \"" + function->function_identifier +
         "\"," + NodeStatementList::print(function->stmt_list, indent + 2) +
         "}";
}

string NodeFunctionCall::print(NodeFunctionCall *function_call, int indent) {
  return "\"function_call\" :{\"name\": \"" +
         function_call->function_identifier + "\"}";
}

string NodeComparativeExpression::print(NodeComparativeExpression *comp_exp,
                                        int indent) {
  string type = "\"undefined\"";
  if (comp_exp->type == INT) {
    type = "\"integer\"";
  } else if (comp_exp->type == CHAR) {
    type = "\"character\"";
  } else if (comp_exp->type == UNDEFINED) {
    type = "\"undefined\"";
  }

  if (comp_exp->comp_operator != NULL && comp_exp->add_exp != NULL) {
    return " \"comp_expression\": {\"type\":" + type + "," +
           NodeComparativeExpression::print(comp_exp->comp_exp, indent + 1) +
           "," +
           NodeComparativeOperator::print(comp_exp->comp_operator, indent + 1) +
           "," + NodeAdditiveExpression::print(comp_exp->add_exp, indent + 1) +
           "}";
  } else {
    return " \"comp_expression\": {\"type\":" + type + "," +
           NodeAdditiveExpression::print(comp_exp->add_exp, indent + 1) + "}";
  }
}

string NodeAdditiveExpression::print(NodeAdditiveExpression *add_exp,
                                     int indent) {

  string type = "";
  if (add_exp->type == INT) {
    type = "\"integer\"";
  } else if (add_exp->type == CHAR) {
    type = "\"character\"";
  } else if (add_exp->type == UNDEFINED) {
    type = "\"undefined\"";
  }

  if (add_exp->add_operator != NULL && add_exp->add_exp != NULL) {
    return " \"add_expression\": {\"type\":" + type + "," +
           NodeAdditiveExpression::print(add_exp->add_exp, indent + 1) + "," +
           NodeAdditiveOperator::print(add_exp->add_operator, indent + 1) +
           "," + NodeNegativeExpression::print(add_exp->neg_exp, indent + 1) +
           +"}";
  } else {
    return "\"add_expression\": {\"type\":" + type + "," +
           NodeNegativeExpression::print(add_exp->neg_exp, indent + 1) + "}";
  }
}

string NodeNegativeExpression::print(NodeNegativeExpression *neg_exp,
                                     int indent) {
  string type = "";
  if (neg_exp->type == INT) {
    type = "\"integer\"";
  } else if (neg_exp->type == CHAR) {
    type = "\"character\"";
  } else if (neg_exp->type == UNDEFINED) {
    type = "\"undefined\"";
  }

  if (neg_exp->neg_exp != NULL) {
    return " \"neg_expression\": {\"type\":" + type + "," +
           NodeNegativeExpression::print(neg_exp->neg_exp, indent + 1) + "}";
  } else {
    return " \"neg_expression\": {\"type\":" + type + "," +
           NodeMultiplicativeExpression::print(neg_exp->mul_exp, indent + 1) +
           "}";
  }
}

string
NodeMultiplicativeExpression::print(NodeMultiplicativeExpression *mul_exp,
                                    int indent) {
  string type = "";
  if (mul_exp->type == INT) {
    type = "\"integer\"";
  } else if (mul_exp->type == CHAR) {
    type = "\"character\"";
  } else if (mul_exp->type == UNDEFINED) {
    type = "\"undefined\"";
  }

  if (mul_exp->mul_operator != NULL && mul_exp->mul_exp != NULL) {
    return " \"mul_expression\": {\"type\":" + type + "," +
           NodeMultiplicativeExpression::print(mul_exp->mul_exp, indent + 1) +
           "," +
           NodeMultiplicativeOperator::print(mul_exp->mul_operator,
                                             indent + 1) +
           "," + NodeExpression::print(mul_exp->exp, indent + 1) + "}";
  } else {
    return " \"mul_expression\": {\"type\":" + type + "," +
           NodeExpression::print(mul_exp->exp, indent + 1) + "}";
  }
}

string NodeExpression::print(NodeExpression *expression, int indent) {
  string type = "";
  if (expression->type == INT) {
    type = "\"integer\"";
  } else if (expression->type == CHAR) {
    type = "\"character\"";
  } else if (expression->type == UNDEFINED) {
    type = "\"undefined\"";
  }

  if (expression->literal) {
    return " \"expression\": {\"type\":" + type + "," +
           NodeLiteral::print(expression->literal, indent + 1) + "}";
  }
  if (expression->identifier) {
    return " \"expression\": {\"type\":" + type + "," +
           NodeIdentifier::print(expression->identifier, indent + 1) + "}";
  }
  if (expression->comp_exp) {
    return " \"expression\": {\"type\":" + type + "," +
           NodeComparativeExpression::print(expression->comp_exp, indent + 1) +
           "}";
  }
  return "";
}

string NodeComparativeOperator::print(NodeComparativeOperator *comp_op,
                                      int indent) {
  return "\"comp_operator\": \"" + comp_op->op + "\"";
}

string NodeAdditiveOperator::print(NodeAdditiveOperator *add_op, int indent) {
  return "\"add_operator\": \"" + string(1, (add_op->op)) + "\"";
}

string NodeMultiplicativeOperator::print(NodeMultiplicativeOperator *mul_op,
                                         int indent) {
  return "\"mul_operator\": \"" + string(1, (mul_op->op)) + "\"";
}

string NodeIdentifier::print(NodeIdentifier *identifier, int indent) {
  string str =
      " \"identifier\": {\"name\":\"" + identifier->name + "\",\"type\":";
  if (identifier->type == CHAR) {
    str += " \"character\"";
  }
  if (identifier->type == INT) {
    str += " \"integer\"";
  }
  str += ",\"scope\":";
  str += to_string(identifier->scope);
  str += "}";
  return str;
}

string NodeLiteral::print(NodeLiteral *literal, int indent) {
  if (literal->type == CHAR) {
    return "\"char_literal\": " + to_string(literal->value);
  }
  if (literal->type == INT) {
    return " \"int_literal\": " + to_string(literal->value);
  }
  return "";
}

string NodeDebug::print(NodeDebug *debug, int indent) {
  if (debug->comp_exp) {
    return " \"debug\": {" +
           NodeComparativeExpression::print(debug->comp_exp, indent + 1) + "}";
  }
  return "";
}
