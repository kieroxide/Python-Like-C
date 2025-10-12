# Python-Like-C

Lightweight educational interpreter (C++) with Python-like syntax. Intended for learning lexer/parser/interpreter basics.

Highlights

-   Assignments and integer arithmetic (+, -, \*, /)
-   Conditionals (`if`) and comparison (`==`, `<`, `>`)
-   `while` loops
-   `print` statements
-   Functions with parameters and `return`
-   Indentation-based blocks (4 spaces per INDENT) and scoped variables

Quick start (Windows PowerShell)

1. Build (requires `g++` on PATH):

```powershell
./scripts/build_all.ps1
```

2. Run interpreter (optional filename argument):

```powershell
./build/main.exe [path\to\script.txt]
```

3. Run test suite:

```powershell
./build/run_tests.exe
```

Notes

-   Function bodies use `{}` braces; `if`/`while` use a colon and indented blocks.
-   The lexer emits one `INDENT` per 4 spaces and warns on non-multiple-of-4 indentation.

License: MIT (see `LICENSE`)
