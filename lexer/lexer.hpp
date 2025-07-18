#pragma once
#include <string>
#include <vector>
#include <cctype>

namespace lexer {
    enum class TokenType{
        //Style
        INDENT,
        NEWLINE,
        BLOCK,
        //Reserved Words
        PRINT,
        IF,
        //USER DEFINED/LITERALS
        IDENTIFIER,
        NUMBER,
        //OPERATORS
        ASSIGN,
        PLUS,
        SUBTRACT,
        MULTIPLY,
        DIVIDE,
        EQUALS,
        LESSTHAN,
        GREATERTHAN
    };

    struct Token{
        TokenType type;
        std::string value;
        int lineNumber;
    };
    std::vector<Token> tokenize(const std::string& code, int& lineNumber);
    std::vector<Token> tokenize_statement(const std::string& code, int& lineNumber);
    Token tokenizeAlpha(char c, int& i, const std::string& code);
    Token tokenizeDigit(char c, int& i, const std::string& code);
    void printTokens(std::vector<Token> tokens);
    std::vector<Token> tokenize(const std::string& code, int& lineNumber);
}