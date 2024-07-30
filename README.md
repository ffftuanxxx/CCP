# Compiler Project

A C-based compiler implementation for a custom language, developed in two phases: Lexical Analysis and Syntax Analysis.

## Project Structure

- `func.c`: Main entry point for the compiler
- `lexer.h` & `lexer.c`: Lexical analyzer implementation
- `symbol_table.h` & `symbol_table.c`: Symbol table management
- `parser.h` & `parser.c`: Syntax analyzer implementation
- `parse_tree.h` & `parse_tree.c`: Parse tree data structure

## Phase 1: Lexical Analysis

### Features
- DFA-based lexer implementation using a transition table
- Token recognition for identifiers, literals, and language-specific symbols
- Symbol table management for identifiers
- Error reporting for lexical errors

### Implementation Details
- Transition table defined in `lexer.h`
- `next_token` function in `lexer.c` for token generation
- Symbol table implemented as a data structure (AVL tree used)

## Phase 2: Syntax Analysis

### Features
- SLR(1) parser implementation
- Parse tree construction
- Syntax error detection and reporting

### Implementation Details
- Parsing table defined in `parser.h`
- Step-by-step parsing process with configuration output
- Parse tree modification during reduction steps

## Grammar
This project implements a custom language grammar with 19 production rules, including function declarations, conditional statements, and expressions.

## Usage

1. Compile the project using the provided makefile or `make.bat`
2. Run the compiler: `./func <source_file>`
3. Output will be displayed on screen and written to `<source_file>_out.txt`

## Output
- Token information and attributes (Phase 1)
- Parsing steps and configurations (Phase 2)
- Symbol table contents
- Syntax analysis completion status
- Error messages (if any)

## Requirements
- Standard C compiler (GCC used)
- MinGW for Windows users, or use online GDB (https://www.onlinegdb.com/)

## Contributors
- Semantic Analysis stage is still undone for this compiler, please feel free to pull request and make contribution to this project.
