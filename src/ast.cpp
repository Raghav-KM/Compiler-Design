#include "./headers/ast.h"

NodeStatement::NodeStatement(NodeDebug *debug) {
  this->let = NULL;
  this->debug = debug;
}
NodeStatement::NodeStatement(NodeLet *let) {
  this->let = let;
  this->debug = NULL;
}

NodeDebug::NodeDebug(NodeExpression *expression) {
  this->expression = expression;
}

NodeLet::NodeLet(NodeIdentifier *identifier, NodeExpression *expression) {
  this->identifier = identifier;
  this->expression = expression;
}

NodeExpression::NodeExpression(NodeIdentifier *identifier) {
  this->INT = NULL;
  this->identifier = identifier;
}
NodeExpression::NodeExpression(NodeINT *INT) {
  this->INT = INT;
  this->identifier = NULL;
}

NodeIdentifier::NodeIdentifier(string value) { name = value; }

NodeINT::NodeINT(int value) { this->value = value; }

string NodeINT::print_INT(NodeINT *INT, int indent) {
  return tab(indent) + "{ int_lit: " + to_string(INT->value) + " }\n";
}

string NodeIdentifier::print_identifier(NodeIdentifier *identifier,
                                        int indent) {
  return tab(indent) + "{ identifier: [" + identifier->name + "," +
         to_string(SymbolTable::get_instance()->get_value(identifier->name)) +
         "] }\n";
}

string NodeExpression::print_expression(NodeExpression *expression,
                                        int indent) {
  if (expression->INT) {
    return tab(indent) + "{ expression : \n" +
           NodeINT::print_INT(expression->INT, indent + 1) + tab(indent) +
           "}\n";
  }
  if (expression->identifier) {
    return tab(indent) + "{ expression : \n" +
           NodeIdentifier::print_identifier(expression->identifier,
                                            indent + 1) +
           tab(indent) + "}\n";
  }
  return "";
}

string NodeLet::print_let(NodeLet *let, int indent) {
  return tab(indent) + "{ let: \n" +
         NodeIdentifier::print_identifier(let->identifier, indent + 1) +
         NodeExpression::print_expression(let->expression, indent + 1) +
         tab(indent) + "}\n";
}

string NodeDebug::print_debug(NodeDebug *debug, int indent) {
  return tab(indent) + "{ debug: \n" +
         NodeExpression::print_expression(debug->expression, indent + 1) +
         tab(indent) + "}\n";
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