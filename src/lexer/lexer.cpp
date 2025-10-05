#include "lexer.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace lexer {

std::vector<std::string> splitByNewline(const std::string& input) {
    std::vector<std::string> lines;
    std::stringstream ss(input);
    std::string line;

    while (std::getline(ss, line)) {
        lines.push_back(line);
    }

    return lines;
}

void printTokens(std::vector<Token> tokens) {
    for (Token t : tokens) {
        std::cout << "Line Number: " << t.lineNumber << ", "
                  << "Type: " << static_cast<int>(t.type) << ", "
                  << "Value: " << t.value << "\n";
    }
}

std::vector<Token> tokenize(const std::string& code, int& lineNumber) {
    std::vector<Token> tokens;
    Token token = {TokenType::BLOCK, "BLOCK", lineNumber};
    tokens.push_back(token);

    const auto& lines = splitByNewline(code);
    for (const auto& line : lines) {
        auto stmtTokens = tokenize_statement(line, lineNumber);
        tokens.insert(tokens.end(), stmtTokens.begin(), stmtTokens.end());
    }
    return tokens;
}

std::vector<Token> tokenize_statement(const std::string& code, int& lineNumber) {
    std::vector<Token> tokens;
    int len = static_cast<int>(code.size());
    int i = 0;

    // Count leading spaces and emit one INDENT token per 4-space group
    int count = 0;
    while (i < len && code[i] == ' ') {
        ++count;
        ++i;
    }
    int groups = count / 4;
    for (int g = 0; g < groups; ++g) {
        tokens.push_back({TokenType::INDENT, "INDENT", lineNumber});
    }

    // Tokenize rest of the line
    for (; i < len; ++i) {
        char c = code[i];
        if (c == ' ')
            continue;

        if (isalpha(static_cast<unsigned char>(c))) {
            Token token = tokenizeAlpha(c, i, code);
            token.lineNumber = lineNumber;
            tokens.push_back(token);
            continue;
        }

        if (isdigit(static_cast<unsigned char>(c))) {
            Token token = tokenizeDigit(c, i, code);
            token.lineNumber = lineNumber;
            tokens.push_back(token);
            continue;
        }

        // two-char operator '=='
        if (c == '=' && i + 1 < len && code[i + 1] == '=') {
            tokens.push_back({TokenType::EQUALS, "==", lineNumber});
            ++i;  // consumed second '='
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
            default:
                std::cerr << "ERROR LEXING line " << lineNumber << " char '" << c << "'\n";
                break;
        }
    }
    return tokens;
}

Token tokenizeAlpha(char c, int& i, const std::string& code) {
    std::string str;
    str.push_back(c);
    while (i + 1 < code.size() && isalpha(code[i + 1])) {
        i++;
        c = code[i];
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

Token tokenizeDigit(char c, int& i, const std::string& code) {
    std::string str;
    str.push_back(c);
    while (i + 1 < code.size() && isdigit(code[i + 1])) {
        i++;
        c = code[i];
        str.push_back(c);
    }
    Token token;
    token.type = TokenType::NUMBER;
    token.value = str;
    return token;
}
}  // namespace lexer
