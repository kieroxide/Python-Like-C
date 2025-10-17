# Python-Like-C

Lightweight educational interpreter (C++) with Python-like syntax. Intended for learning lexer/parser/interpreter basics.

## Highlights

-   Assignments and integer arithmetic (+, -, \*, /)
-   Conditionals (`if`) and comparison (`==`, `<`, `>`)
-   `while` loops
-   `print` statements
-   Functions with parameters and `return`
-   Indentation-based blocks (4 spaces per INDENT) and scoped variables
-   **Arrays with indexing and assignment**

## Quick start (Windows PowerShell)

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

## Array Usage

```python
// Create an array with bracket notation
arr = [1, 2, 3, 4, 5]

// Access elements by index (0-based)
x = arr[0]  // x = 1
y = arr[2]  // y = 3

// Modify elements
arr[1] = 10  // arr becomes [1, 10, 3, 4, 5]

// Use in expressions
result = arr[0] + arr[1]  // result = 11

// Print array elements
print arr[3]  // prints 4
```

## Notes

-   Function bodies use `{}` braces; `if`/`while` use a colon and indented blocks.
-   The lexer emits one `INDENT` per 4 spaces and warns on non-multiple-of-4 indentation.
-   Arrays can only contain integers and are passed by value to functions.
-   Array indices must be valid (no bounds checking yet - accessing out of bounds is undefined behavior).

## License

MIT (see `LICENSE`)
