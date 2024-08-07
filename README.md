# C++ Compiler

This project is a simple compiler written in C++ that can parse, analyze, and generate assembly code from a custom source language.

## Table of Contents

-   [Features](#features)
-   [Getting Started](#getting-started)
-   [Usage](#usage)

## Features

-   **Lexical Analysis**: Tokenizes the source code into meaningful symbols.
-   **Syntax Analysis**: Parses tokens to construct a parse tree.
-   **Semantic Analysis**: Checks types and manages scope.
-   **Code Generation**: Translates the parse tree into assembly code.
-   Support for basic arithmetic operations, comparisons, and control flow.

## Getting Started

### Prerequisites

-   C++ Compiler (g++)
-   Make
-   NASM (Netwide Assembler) for assembling the generated assembly code

### Installation

Clone the repository to your local machine:

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
