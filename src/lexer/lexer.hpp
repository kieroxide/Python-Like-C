#pragma once
#include <cctype>
#include <string>
#include <vector>

enum class TokenType {
    // Style
    INDENT,
    NEWLINE,
    BLOCK,
    COLON,
    // Reserved Words
    PRINT,
    IF,
    // USER DEFINED/LITERALS
    IDENTIFIER,
    NUMBER,
    // OPERATORS
    ASSIGN,
    PLUS,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    EQUALS,
    LESSTHAN,
    GREATERTHAN
};

struct Token {
    TokenType type;
    std::string value;
    int lineNumber;
};

class Lexer {
   private:
    int lineNumber = 1;
    int characterPosition = 0;

   public:
    static void printTokens(std::vector<Token> tokens);
    std::vector<Token> tokenize(const std::string& code);

   private:
    std::vector<Token> tokenize_statement(const std::string& code);
    Token tokenizeAlpha(const std::string& code);
    Token tokenizeDigits(const std::string& code);
};