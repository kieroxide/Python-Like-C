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
        while (characterPosition < len && code[characterPosition++] == ' ') {
            ++count;
        }

        int groups = count / 4;
        for (int g = 0; g < groups; ++g) {
            tokens.push_back({TokenType::INDENT, "INDENT", lineNumber});
        }
    }

    // Tokenize rest of the line
    while (characterPosition < len) {
        char c = code[++characterPosition];
        if (c == ' ')
            continue;

        if (isalpha(static_cast<unsigned char>(c))) {
            Token token = tokenizeAlpha(code);
            token.lineNumber = lineNumber;
            tokens.push_back(token);
            continue;
        }

        if (isdigit(static_cast<unsigned char>(c))) {
            Token token = tokenizeDigits(code);
            token.lineNumber = lineNumber;
            tokens.push_back(token);
            continue;
        }

        // two-char operator '=='
        if (c == '=' && characterPosition + 1 < len && code[characterPosition + 1] == '=') {
            tokens.push_back({TokenType::EQUALS, "==", lineNumber});
            ++characterPosition;  // consumed second '='
            continue;
        }

        switch (c) {
            case '=':
                tokens.push_back({TokenType::ASSIGN, "=", lineNumber});
                break;
            case '+':
                tokens.push_back({TokenType::PLUS, "+", lineNumber});
                break;
            case '-':
                tokens.push_back({TokenType::SUBTRACT, "-", lineNumber});
                break;
            case '*':
                tokens.push_back({TokenType::MULTIPLY, "*", lineNumber});
                break;
            case '/':
                tokens.push_back({TokenType::DIVIDE, "/", lineNumber});
                break;
            case '<':
                tokens.push_back({TokenType::LESSTHAN, "<", lineNumber});
                break;
            case '>':
                tokens.push_back({TokenType::GREATERTHAN, ">", lineNumber});
                break;
            case ':':
                tokens.push_back({TokenType::COLON, ":", lineNumber});
                break;
            case '\n':
                lineNumber++;
                characterPosition++;
                tokens.push_back({TokenType::NEWLINE, "NEWLINE", lineNumber});
                return tokens;
            default:
                cerr << "ERROR LEXING line " << lineNumber << " char '" << c << "'\n";
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
