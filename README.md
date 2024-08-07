# C++ Compiler

This project is a simple compiler written in C++ that can analyze, parse, and generate assembly code from a custom source language.

## Table of Contents

-   [Features](#features)
-   [Getting Started](#getting-started)
-   [Usage](#usage)
-   [Detailed Documentation]()

## Features

1.  **Lexical Analysis**: Tokenization of the source code into meaningful tokens.
2.  **Syntax Analysis**: Parses tokens to construct a parse tree.
3.  **Semantic Analysis**: Checks types and manages scope.
4.  **Code Generation**: Translates the parse tree into assembly code.
5.  Support for arithmetic operations, comparisons, and control flow statements.
6.  Support for function definations and function calls

## Getting Started

### Prerequisites

-   C++ Compiler (g++)
-   Make
-   NASM (Netwide Assembler) for assembling the generated assembly code

### Installation

Clone the repository to your local machine and initialize the required build directories

```bash
git clone https://github.com/Raghav-KM/Compiler-Design.git

cd Compiler-Design

make build_dirs
```

## Usage

### Building the Compiler

Compile the custom compiler source code to produce the executable located at `./build/output`

```bash
make compiler
```

### Compiling Input Source Code

Use the compiler to process your custom-language source code, producing an assembly file at `./build/Assembly/program.asm`

```bash
make run_compiler
```

### Running the Generated Assembly

Assemble the generated assembly code using NASM and execute it:

```bash
make asm
```

## Detailed Documentation

For detailed information on each phase of compilation, please refer to the following files

-   [Lexer](Lexer.md)
-   [Grammar](Grammar.md)
-   [Parser](Parser.md)
-   [Code-Generation](Codegen.md)
