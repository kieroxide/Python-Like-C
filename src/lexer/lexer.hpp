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
    LPAREN,
    RPAREN,
    COMMA,
    LBRACE,
    RBRACE,
    // Reserved Words
    PRINT,
    IF,
    DEF,
    WHILE,
    // USER DEFINED/LITERALS
    IDENTIFIER,
    NUMBER,
    // OPERATORS
    RETURN,
    ASSIGN,
    PLUS,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    EQUALS,
    LESSTHAN,
    GREATERTHAN,
    _EOF
};

struct Token {
    TokenType type;
    std::string value;
    size_t lineNumber;

    Token() {};

    Token(TokenType type, std::string value) {
        this->type = type;
        this->value = value;
    }

    Token(TokenType type, std::string value, size_t lineNumber) {
        this->type = type;
        this->value = value;
        this->lineNumber = lineNumber;
    }
};

class Lexer {
   private:
    size_t lineNumber = 1;
    size_t characterPosition = 0;

   public:
    static void printTokens(std::vector<Token> tokens);
    std::vector<Token> tokenize(const std::string& code);

   private:
    std::vector<Token> tokenize_statement(const std::string& code);
    Token tokenizeAlpha(const std::string& code);
    Token tokenizeDigits(const std::string& code);
};
