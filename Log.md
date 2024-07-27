# Compiler-Design
----
1.  
2.  **TODO : Add Support for negative integers**
3.  **TODO : Add Test-CASES for Lexer**
4.  **TODO : Add Test-CASES for Parser**
5.  **TODO : Add Test-CASES for Codegen**
6.  **TODO : Create Destructors for all AST_NODE classes**
7.  <!-- **TODO : Create a Error Class** -->
8.  <!-- **TODO : Get DBG to compile to ASM** -->
9.  **TODO : Create destructor for singleton-SymbolTable class**
10. **TODO : Add FLAGS to allow compiler till [LEXER, PARSER, CODEGEN]**
11. <!-- **TODO : Change lexer to be able to recognize operators even without space** -->
12. **TODO : Write ASM for divide operation**
13. <!-- **TODO : Make a ENDLINE** --!>
14. <!-- **TODO : Add Better Error Handling** --!>
15. <!-- **TODO : Add Character support** --!>
16. **TODO : Add String support**
17. **TODO : Add Function support (without arguments)**
18. **Todo : Add Function support (with arguments)**
19. **Todo : Add Function support (with return types)**
19. **Todo : Start execution from the main function**
20. **Todo : Type Casting from INT->CHAR and CHAR->INT**
21. **Todo : Re-Write Parser**
22. **Todo : Re-Write Codegen**
23. <!-- **Todo : Allow IF without ELSE** --!>
----

# 22-06-2024
- Reading input file passed as argument

# 24-06-2024
- TOKENIZED the input file
- Created MakeFile

# 25-06-2024
- Stored value of a token
- Added let,'=' tokens
- Began with the parser
- Began with some basic-grammar construction

# 26-06-2024
- Created a AST (for the existing grammer)
- Added print functionality to ast nodes

# 27-06-2024
- Improved ast-print functionality
- Added Error Class
- Added Symbol table
- Added Undeclared/Redeclaration Errors

# 28-06-2024
- Made Symbol table class Singleton
- Did NASM installation and ASM-Make File

# 29-06-2024
- Began with Codegen class
- let and debug working as per current grammar (executable running)

# 30-06-2024
- Added support Expressions
- Re-Wrote the parser

# 01-07-2024
- Reverted back to the old parser
- Created Lexer Testcase Class


# 02-07-2024
- Lexer Testcase Class Completed
- +,-,*,/ tokens added
- Created Grammar for Arithemetic operations

# 04-07-2024
- Implemented Arithemetic operations grammar

# 06-07-2024
- Refactored Lexer, Parse, Error Classes
- Did 3 Address Code generation

# 09-07-2024
- Implemented Codegen of the expressions
- Found issue in the parse-tree

# 10-07-2024
- Corrected the parser for expresssion

# 11-07-2024
- Started implementation of IF-ELSE

# 12-07-2024
- Completed implementation of IF-ELSE
- Trying to implement a better lexer

# 13-07-2024
- Rewrote the whole lexer

# 14-07-2024
- Wrote Grammar.md
- Added "==" to the lexer

# 15-07-2024
- Completed Comparison Operator "==" implementation

# 16-07-2024
- Added all Comparison Operators ie '>' '<' '>=', '<=' , '!='

# 17-07-2024
- Added Assignment Statment
- Added For loops

# 18-07-2024
- Modified IF-ElSE statements

# 23-07-2024
- Added newline subroutine
- Added dbg support for characters

# 25-07-2024
- Working endline
- If working without else
- Partially re-wrote parser

# 26-07-2024
- Working automatic typecasting from int->char->int
- Working arithematic operations on characters
- re-wrote whole parser

# 27-07-2024
- Improved Error handling
- Working Parser for function and function call (without arguments and return type)