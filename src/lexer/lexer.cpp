#include "Lexer.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "src/Utility/Utility.hpp"

using namespace std;

void Lexer::printTokens(vector<Token> tokens) {
    for (Token t : tokens) {
        cout << "Line Number: " << t.lineNumber << ", "
             << "Type: " << static_cast<int>(t.type) << ", "
             << "Value: " << t.value << "\n";
    }
}

/**
 * Takes string and returns a vector of the whole tokenized string
 */
vector<Token> Lexer::tokenize(const string& code) {
    characterPosition = 0;
    lineNumber = 1;

    vector<Token> tokens;

    const size_t len = code.size();
    while (characterPosition < len) {
        auto stmtTokens = tokenize_statement(code);
        tokens.insert(tokens.end(), stmtTokens.begin(), stmtTokens.end());
    }

    return tokens;
}

/* Tokenizes a Single Statement token Vector which is defined when a newline character is hit*/
vector<Token> Lexer::tokenize_statement(const string& code) {
    vector<Token> tokens;
    size_t len = code.size();

    const bool isAtStartOfNewLine = characterPosition == 0 || code[characterPosition - 1] == '\n';
    if (isAtStartOfNewLine) {
        size_t count = 0;
        // Count all whitespace to check total number of indents
        while (characterPosition < len && code[characterPosition] == ' ') {
            ++count;
            ++characterPosition;
        }
        // Emit indent token per 4 spaces
        size_t groups = count / 4;

        if (count % 4 != 0) {
            cerr << "WARNING: Indentation on line " << lineNumber << " is " << count
                 << " spaces, not a multiple of 4\n";
        }

        for (size_t g = 0; g < groups; ++g) {
            tokens.push_back({TokenType::INDENT, "INDENT", lineNumber});
        }
    }

    // Tokenize rest of the line
    while (characterPosition < len) {
        const char c = code[characterPosition];

        if (isalpha(static_cast<unsigned char>(c))) {
            Token token = tokenizeAlpha(code);
            token.lineNumber = lineNumber;
            tokens.push_back(token);
            ++characterPosition;  // advance past the last char consumed by helper
            continue;
        }

        if (isdigit(static_cast<unsigned char>(c))) {
            Token token = tokenizeDigits(code);
            tokens.push_back(token);
            ++characterPosition;  // advance past last digit consumed by helper
            continue;
        }

        switch (c) {
            case '\r':
                ++characterPosition;
                break;

            case ' ':
                ++characterPosition;  // main loop consumes spaces
                break;

            case '(':
                tokens.push_back({TokenType::LPAREN, "(", lineNumber});
                ++characterPosition;
                break;

            case '{':
                tokens.push_back({TokenType::LBRACE, "{", lineNumber});
                ++characterPosition;
                break;

            case ')':
                tokens.push_back({TokenType::RPAREN, ")", lineNumber});
                ++characterPosition;
                break;

            case '}':
                tokens.push_back({TokenType::RBRACE, "}", lineNumber});
                ++characterPosition;
                break;

            case ',':
                tokens.push_back({TokenType::COMMA, ",", lineNumber});
                ++characterPosition;
                break;

            // Tokenizes '/' or '//'
            case '/':
                if (characterPosition + 1 < len && code[characterPosition + 1] == '/') {
                    while (characterPosition < len && code[characterPosition] != '\n') {
                        ++characterPosition;
                    }
                } else {
                    tokens.push_back({TokenType::DIVIDE, "/", lineNumber});
                    ++characterPosition;
                }
                break;

            // Tokenizes '=' or '=='
            case '=':
                if (c == '=' && characterPosition + 1 < len && code[characterPosition + 1] == '=') {
                    // two-char operator '=='
                    tokens.push_back({TokenType::EQUALS, "==", lineNumber});
                    characterPosition += 2;  // consume both '=' chars
                } else {
                    // one-char assignment
                    tokens.push_back({TokenType::ASSIGN, "=", lineNumber});
                    ++characterPosition;
                }
                break;

            case '+':
                tokens.push_back({TokenType::PLUS, "+", lineNumber});
                ++characterPosition;
                break;

            case '-':
                tokens.push_back({TokenType::SUBTRACT, "-", lineNumber});
                ++characterPosition;
                break;

            case '*':
                tokens.push_back({TokenType::MULTIPLY, "*", lineNumber});
                ++characterPosition;
                break;

            case '<':
                tokens.push_back({TokenType::LESSTHAN, "<", lineNumber});
                ++characterPosition;
                break;

            case '>':
                tokens.push_back({TokenType::GREATERTHAN, ">", lineNumber});
                ++characterPosition;
                break;

            case ':':
                tokens.push_back({TokenType::COLON, ":", lineNumber});
                ++characterPosition;
                break;

            case '\n':
                tokens.push_back({TokenType::NEWLINE, "NEWLINE", lineNumber});
                ++lineNumber;
                ++characterPosition;
                return tokens;

            default:
                cerr << "ERROR LEXING line " << lineNumber << " char '" << c << "'\n";
                ++characterPosition;  // avoid infinite loop on unknown char
                break;
        }
    }
    return tokens;
}

/* Creates new token of a word ensuring to take the entire word if has multiple chars*/
Token Lexer::tokenizeAlpha(const string& code) {
    char c = code[characterPosition];
    string str;
    str.push_back(c);

    // Look over the string of chars and push to the string
    while (characterPosition + 1 < code.size() && isalpha(code[characterPosition + 1])) {
        c = code[++characterPosition];
        str.push_back(c);
    }

    // Check for special reserved words
    Token token;
    if (str == "if") {
        token.type = TokenType::IF;
    } else if (str == "print") {
        token.type = TokenType::PRINT;
    } else if (str == "def") {
        token.type = TokenType::DEF;
    } else {
        token.type = TokenType::IDENTIFIER;
    }

    token.value = str;
    return token;
}

/* Creates new token of a digit ensuring to take the entire number if has multiple digits*/
Token Lexer::tokenizeDigits(const string& code) {
    string str;

    char c = code[characterPosition];
    str.push_back(c);

    // Looks for all digits of ahead of first digit
    while (characterPosition + 1 < code.size() && isdigit(code[characterPosition + 1])) {
        c = code[++characterPosition];
        str.push_back(c);
    }

    Token token;
    token.type = TokenType::NUMBER;
    token.value = str;
    token.lineNumber = lineNumber;
    return token;
}
