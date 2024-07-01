#ifndef TESTING_H
#define TESTING_H

#include "lexer.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

class Test_Lexical_Analyser {
private:
  static vector<pair<string, string>> test_file_names;
  Lexical_Analyzer lexer;

public:
  
  void run_all_tests();

  void run_test(string input_file, string output_file);
};

#endif