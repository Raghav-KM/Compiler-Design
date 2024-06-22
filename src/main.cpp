#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

int main(int argc, char *argv[]) {

  if (argc != 2) {
    cerr << "ERR : Input File Missing" << endl;
    return EXIT_FAILURE;
  }

  cout << "Reading " << argv[1] << endl;

  ifstream input(argv[1]);
  stringstream input_buffer;
  input_buffer << input.rdbuf();

  cout << input_buffer.str() << endl;
  return EXIT_SUCCESS;
}