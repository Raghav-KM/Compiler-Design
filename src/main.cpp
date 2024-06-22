#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;


enum class TOKENS{
  tRETURN,
  tINT_LIT,
  tSEMICOLON
};


int main(int argc, char *argv[]) {

  if (argc != 2) {
    cerr << "ERR : Input File Missing" << endl;
    return EXIT_FAILURE;
  }

  string input_file_path = argv[1];
  ifstream f_input(input_file_path);
  if(f_input.fail()){
    cerr<<"ERR : Error while reading file"<<endl;
    return EXIT_FAILURE;
  }
  
  cout << "Reading " << argv[1] << endl;
  stringstream input_buffer;
  input_buffer << f_input.rdbuf();
  f_input.close();

  cout << input_buffer.str() << endl;
  return EXIT_SUCCESS;
}