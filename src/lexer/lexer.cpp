#include "lexer.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "src/utility/utility.hpp"

using namespace std;

void Lexer::printTokens(vector<Token> tokens) {
    for (Token t : tokens) {
        cout << "Line Number: " << t.lineNumber << ", "
             << "Type: " << static_cast<int>(t.type) << ", "
             << "Value: " << t.value << "\n";
    }
}

vector<Token> Lexer::tokenize(const string& code) {
    characterPosition = 0;
    lineNumber = 1;

    vector<Token> tokens;

    // Generic Block type to denote Start of program
    Token token = {TokenType::BLOCK, "BLOCK", lineNumber};
    tokens.push_back(token);

    const int len = static_cast<int>(code.size());
    while (characterPosition < len) {
        auto stmtTokens = tokenize_statement(code);
        tokens.insert(tokens.end(), stmtTokens.begin(), stmtTokens.end());
    }

    return tokens;
}

/* Tokenizes a Single Statement returning a list of tokens at code end or new line*/
vector<Token> Lexer::tokenize_statement(const string& code) {
    vector<Token> tokens;
    int len = static_cast<int>(code.size());

    // Count leading spaces and emit one INDENT token per 4-space group (only at start of newline)
    const bool isAtStartOfNewLine = characterPosition == 0 || code[characterPosition - 1] == '\n';
    if (isAtStartOfNewLine) {
        int count = 0;
        while (characterPosition < len && code[characterPosition] == ' ') {
            ++count;
            ++characterPosition;
        }

        int groups = count / 4;
        for (int g = 0; g < groups; ++g) {
            tokens.push_back({TokenType::INDENT, "INDENT", lineNumber});
        }
    }

    // Tokenize rest of the line
    while (characterPosition < len) {
        char c = code[characterPosition];
        if (c == ' ') {
            ++characterPosition;  // main loop consumes spaces
            continue;
        }
        if (c == '/' && characterPosition + 1 < len && code[characterPosition + 1] == '/') {
            while (characterPosition < len && code[characterPosition] != '\n') {
                ++characterPosition;
            }
            continue;
        }
        if (c == '\r') {
            ++characterPosition;
            continue;
        }  // handle CR in CRLF

        if (isalpha(static_cast<unsigned char>(c))) {
            Token token = tokenizeAlpha(code);
            token.lineNumber = lineNumber;
            tokens.push_back(token);
            ++characterPosition;  // advance past the last char consumed by helper
            continue;
        }

        if (isdigit(static_cast<unsigned char>(c))) {
            Token token = tokenizeDigits(code);
            token.lineNumber = lineNumber;
            tokens.push_back(token);
            ++characterPosition;  // advance past last digit consumed by helper
            continue;
        }

        // two-char operator '=='
        if (c == '=' && characterPosition + 1 < len && code[characterPosition + 1] == '=') {
            tokens.push_back({TokenType::EQUALS, "==", lineNumber});
            characterPosition += 2;  // consume both '=' chars
            continue;
        }

        switch (c) {
            case '=':
                tokens.push_back({TokenType::ASSIGN, "=", lineNumber});
                ++characterPosition;
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
            case '/':
                tokens.push_back({TokenType::DIVIDE, "/", lineNumber});
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

    while (characterPosition + 1 < code.size() && isalpha(code[characterPosition + 1])) {
        c = code[++characterPosition];
        str.push_back(c);
    }

    Token token;
    if (str == "if") {
        token.type = TokenType::IF;
    } else if (str == "print") {
        token.type = TokenType::PRINT;
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

    while (characterPosition + 1 < code.size() && isdigit(code[characterPosition + 1])) {
        c = code[++characterPosition];
        str.push_back(c);
    }

    Token token;
    token.type = TokenType::NUMBER;
    token.value = str;

    return token;
}
