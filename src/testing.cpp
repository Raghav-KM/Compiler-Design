#include "./headers/testing.h"

vector<pair<string, string>> Test_Lexical_Analyser::test_file_names = {
    {"./test/Lexer/input/1.ds", "./test/Lexer/output/1.txt"},
    {"./test/Lexer/input/2.ds", "./test/Lexer/output/2.txt"},
};

void Test_Lexical_Analyser::run_all_tests() {
  for (auto test : Test_Lexical_Analyser::test_file_names) {
    run_test(test.first, test.second);
  }
}

void Test_Lexical_Analyser::run_test(string input_file_path,
                                     string output_file_path) {
  ifstream f_input(input_file_path);
  ifstream f_output(output_file_path);

  if (f_input.fail() || f_output.fail()) {
    cerr << "ERR : Error while reading file" << endl;
    return;
  }
  stringstream input_buffer, output_buffer;
  input_buffer << f_input.rdbuf();
  output_buffer << f_output.rdbuf();
  f_input.close();
  f_output.close();

  string input = input_buffer.str();
  string expected_output = output_buffer.str();

  lexer.set_input_stream(input);
  cout << "Tescase " + input_file_path.substr(19, 1) << " : ";
  string output = lexer.get_token_stream();
  if (output == expected_output) {
    cout << "PASS\n";

  } else {
    cout << "FAIL\n";
    cout << "Expected Output : " << expected_output << "\n";
    cout << "Output          : " << output << "\n";
    cout << "\n";
  }
  lexer.reset();
}