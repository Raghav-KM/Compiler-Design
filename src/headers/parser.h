#ifndef PARSER_H
#define PARSER_H
#include "lexer.h"

class NodeINT {
  friend class Parser;
  int value;

public:
  NodeINT(int value);
  static string print_INT(NodeINT *INT);
};

class NodeIdentifier {
  friend class Parser;
  string name;

public:
  NodeIdentifier(string name);
  static string print_identifier(NodeIdentifier *identifier);
};

class NodeLet {
  friend class Parser;
  NodeINT *INT;
  NodeIdentifier *identifier;

public:
  NodeLet(NodeINT *INT, NodeIdentifier *identifier);
  static string print_let(NodeLet *let);
};

class NodeDebug {
  friend class Parser;
  NodeINT *INT;

public:
  NodeDebug(NodeINT *INT);
  static string print_debug(NodeDebug *debug);
};

class NodeStatement {
  friend class Parser;
  NodeDebug *debug;
  NodeLet *let;

public:
  NodeStatement(NodeDebug *debug);
  NodeStatement(NodeLet *let);

  static string print_statement(NodeStatement *stmt);
};

class NodeProgram {
  friend class Parser;
  vector<NodeStatement *> stmts;

public:
  static string print_program(NodeProgram *program);
};

class Parser {
private:
  vector<Token> token_stream;
  int ptr;
  NodeStatement *parse_statement();
  NodeDebug *parse_debug();
  NodeLet *parse_let();
  NodeINT *parse_int();
  NodeIdentifier *parse_identifier();

public:
  Parser(Lexical_Analyzer &lexer);
  Token look_ahead();
  void consume();

  NodeProgram *parse_program();
};

#endif