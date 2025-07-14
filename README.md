# Python-Like-C

A simple scripting language inspired by Python, built from scratch in C++. This project demonstrates how to build a programming language interpreter from the ground up, featuring a custom lexer, parser, and interpreter with Python-style syntax and indentation-based blocks.

---

## 🧠 Features

- **Python-like Syntax**: Supports `if`, `print`, variable assignments, and arithmetic expressions.
- **Indentation-based Blocks**: Uses indentation for code blocks, just like Python.
- **Lexer**: Performs lexical analysis and tokenization.
- **Parser**: Builds an Abstract Syntax Tree (AST) using recursive descent parsing.
- **Interpreter**: Walks the AST to evaluate expressions and execute statements.
- **Modular Design**: Easily extendable to add new language features.

---

## 📦 Project Structure

```
.
├── main.cpp                     # Entry point: reads file, runs lexer, parser, and interpreter
├── lexer/
│   ├── lexer.hpp
│   └── lexer.cpp                # Token types and lexer implementation
├── parser/
│   ├── parser.hpp
│   └── parser.cpp               # AST nodes and parser logic
├── interpreter/
│   ├── interpreter.hpp
│   └── interpreter.cpp          # Tree-walking interpreter
└── test.txt                     # Sample script input
```

---

## ✅ Example Code (`test.txt`)

```python
if 1 < 2:
    print 4

x = 5
y = 6
print x

if x == y:
    print x + y

if x < y:
    print 2 * x
```
## Running the Program

The program takes an optional command-line argument specifying the input filename. If no filename is provided, it defaults to `test.txt` in the current working directory.

### Usage

```bash
./main.exe [filename]

## 🚧 Planned Features

- Functions and parameters
- Loops (`while`, `for`)
- Variable scope (environments)
- Error handling (syntax/runtime)

---

## 💡 Inspiration

This project is designed for learning how programming languages work internally — from text parsing to AST evaluation. It’s a hands-on way to explore the fundamentals of interpreters and compilers.

---

## 📜 License

MIT — free to use, modify, and share.