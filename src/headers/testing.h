#ifndef TESTING_H
#define TESTING_H

#include "lexer.h"
#include "parser.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

class TestCompiler {
protected:
  Lexer lexer;
  Parser parser;

public:
  TestCompiler();
};

class Test_Lexical_Analyser : TestCompiler {
private:
  static vector<pair<string, string>> test_file_names;

public:
  Test_Lexical_Analyser();
  bool run_all_tests();
  bool run_test(string input_file, string output_file);
};

class Test_Parser : TestCompiler {
private:
  static vector<pair<string, string>> test_file_names;

public:
  Test_Parser();
  bool run_all_tests();
  bool run_test(string input_file, string output_file);
};

#endif
