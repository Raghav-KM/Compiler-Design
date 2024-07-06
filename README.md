# Compiler-Design
g++ -o ../build/output main.cpp && ../build/output ../input/source.dc

**TODO : Add regex to check the type tokens**
**TODO : Add Support for negative integers**
**TODO : Add Test-CASE support**
**TODO : Create Destructors for all AST_NODE classes**
<!-- **TODO : Create a Error Class** -->
<!-- **TODO : Get DBG to compile to ASM** -->
**TODO : Create destructor for singleton-SymbolTable class**
**TODO : Add FLAGS to allow compiler till [LEXER, PARSER, CODEGEN]**
**TODO : Change lexer to be able to recognize operators even without space**

# 22-06-2024
- Read a file that is passed as an argument 

# 24-06-2024
- TOKENIZE the input string
- Create MakeFile

# 25-06-2024
- Store value of a token
- Add let,'=' tokens
- Begin with parser
- Begin with some basic-grammar construction

# 26-06-2024
- Create a AST (for the existing grammer)
- Add print functionality to ast nodes

# 27-06-2024
- improve ast-print functionality
- Add Error Class
- Add Symbol table
- Add Undeclared/Redeclaration Errors

# 28-06-2024
- Singleton Symbol table
- NASM installation and ASM-Make File

# 29-06-2024
- Codegen class
- let and debug working as per current grammar

# 30-06-2024
- Added Expression
- Re-Wrote the parser

# 1-07-2024
- Revert back to the old parser
- Lexer Testcase Class


# 2-07-2023
- Lexer Testcase Class Completed
- +,-,*,/ tokens added
- Grammar for Arithemetic operations

# 4-07-2023
- Implement Arithemetic operations grammar

# 6-07-2023
- Refactored Lexer, Parse, Error
- 3 Address Code generation
