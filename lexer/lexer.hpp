#pragma once
#include <string>
#include <vector>
#include <cctype>

namespace lexer {
    enum class TokenType{
        //Reserved Words
        PRINT,
        //USER DEFINED/LITERALS
        IDENTIFIER,
        NUMBER,
        //OPERATORS
        ASSIGN,
        PLUS,
        SUBTRACT,
        MULTIPLY,
        DIVIDE,
    };

    struct Token{
        TokenType type;
        std::string value;
        int line_number;
    };
    Token tokenizeAlpha(char c, int& i, const std::string& code);
    Token tokenizeDigit(char c, int& i, const std::string& code);
    void printTokens(std::vector<Token> tokens);
    std::vector<Token> tokenize(const std::string& code);
}