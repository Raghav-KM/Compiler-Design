#include "./headers/parser.h"

// --- Parser --- //

// --- Private Member Functions --- //

// --- Public Member Functions --- //

Parser::Parser(Lexical_Analyzer &lexer) {
  token_stream = lexer.token_stream;
  ptr = 0;
}

Token Parser::look_ahead() {
  if (ptr == token_stream.size())
    return Token(END_FILE);
  return token_stream[ptr];
}

void Parser::consume() { ptr++; }

NodeProgram *Parser::parse_program() {
  NodeProgram *program = new NodeProgram();

  while (look_ahead().get_type() != END_FILE) {
    NodeStatement *stmt = parse_statement();
    if (!stmt)
      return NULL;
    program->stmts.push_back(stmt);
  }
  return program;
}

NodeStatement *Parser::parse_statement() {

  if (look_ahead().get_type() == DEBUG) {
    NodeDebug *debug = parse_debug();
    if (!debug) {
      return NULL;
    }
    return new NodeStatement(debug);
  } else if (look_ahead().get_type() == LET) {
    NodeLet *let = parse_let();
    if (!let) {
      return NULL;
    }
    return new NodeStatement(let);
  }
  return NULL;
}

NodeDebug *Parser::parse_debug() {
  consume();
  if (look_ahead().get_type() == INT_LIT) {
    NodeINT *INT = parse_int();
    if (!INT) {
      return NULL;
    }
    if (look_ahead().get_type() == SEMICOLON) {
      consume();
      return new NodeDebug(INT);
    } else {
      return NULL;
    }
  }
  return NULL;
}

NodeLet *Parser::parse_let() {
  consume();
  if (look_ahead().get_type() == IDENTIFIER) {
    NodeIdentifier *identifier = parse_identifier();
    if (!identifier)
      return NULL;
    if (look_ahead().get_type() == EQUALS) {
      consume();
      if (look_ahead().get_type() == INT_LIT) {
        NodeINT *INT = parse_int();
        if (!INT) {
          return NULL;
        }
        if (look_ahead().get_type() == SEMICOLON) {
          consume();
          return new NodeLet(INT, identifier);
        } else {
          return NULL;
        }
      }
    } else {
      return NULL;
    }
  }
  return NULL;
}

NodeINT *Parser::parse_int() {
  NodeINT *INT = new NodeINT(look_ahead().get_value());
  consume();
  return INT;
}
NodeIdentifier *Parser::parse_identifier() {
  NodeIdentifier *identifier = new NodeIdentifier(look_ahead().get_body());
  consume();
  return identifier;
}

// --- AST-Node --- //

NodeStatement::NodeStatement(NodeDebug *debug) {
  this->let = NULL;
  this->debug = debug;
}
NodeStatement::NodeStatement(NodeLet *let) {
  this->let = let;
  this->debug = NULL;
}

NodeDebug::NodeDebug(NodeINT *INT) { this->INT = INT; }

NodeLet::NodeLet(NodeINT *INT, NodeIdentifier *identifier) {
  this->INT = INT;
  this->identifier = identifier;
}
NodeIdentifier::NodeIdentifier(string value) { name = value; }
NodeINT::NodeINT(int value) { this->value = value; }

string NodeINT::print_INT(NodeINT *INT) {
  return "{ int_lit: " + to_string(INT->value) + "}";
}

string NodeIdentifier::print_identifier(NodeIdentifier *identifier) {
  return "{ identifier: " + identifier->name + "}";
}

string NodeLet::print_let(NodeLet *let) {
  return "{ let: " + NodeINT::print_INT(let->INT) +
         NodeIdentifier::print_identifier(let->identifier) + "}";
}

string NodeDebug::print_debug(NodeDebug *debug) {
  return "{ debug: " + NodeINT::print_INT(debug->INT) + "}";
}

string NodeStatement::print_statement(NodeStatement *stmt) {
  if (stmt->debug)
    if (stmt->debug) {
      return "{ statement: " + NodeDebug::print_debug(stmt->debug) + "}";
    }
  if (stmt->let) {
    return "{ statement: " + NodeLet::print_let(stmt->let) + "}";
  }
  return "";
}

string NodeProgram::print_program(NodeProgram *program) {
  string res = "{ program: ";
  for (auto stmt : program->stmts) {
    res += NodeStatement::print_statement(stmt);
  }
  res += "}\n";
  return res;
}