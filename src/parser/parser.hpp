#pragma once
#include <memory>
#include <string>
#include <vector>

#include "src/Lexer/Lexer.hpp"

enum class NodeType {
    PROGRAM,
    DEF,
    PARAM,
    FUNC_CALL,
    ASSIGN,
    PRINT,
    BLOCK,
    IF,
    CONDITIONAL,
    OPERATOR,
    VARIABLE,
    NUMBER
};

struct Node {
    NodeType type;
    Token token;
    std::string value;

    std::vector<std::unique_ptr<Node>> children;

    void addChild(std::unique_ptr<Node> child) {
        children.push_back(std::move(child));
    }

    Node() {
    }

    Node(NodeType type, std::string value) {
        this->type = type;
        this->value = value;
    }

    Node(NodeType type, Token token, std::string value) {
        this->type = type;
        this->token = token;
        this->value = value;
    }
};

class Parser {
   private:
    size_t tokenPosition = 0;
    size_t lineNumber = 0;
    size_t tokenLength = 0;

   public:
    static void printAST(const std::unique_ptr<Node>& node, int indent = 0);
    std::unique_ptr<Node> parseProgram(const std::vector<Token>& tokens);

   private:
    std::unique_ptr<Node> Parser::parseFunction(const std::vector<Token>& tokens);
    std::unique_ptr<Node> Parser::parseFunctionCall(const std::vector<Token>& tokens);
    std::unique_ptr<Node> Parser::parseParams(const std::vector<Token>& tokens, std::unique_ptr<Node>& funcNode);
    std::unique_ptr<Node> parseStatement(const std::vector<Token>& tokens);
    std::unique_ptr<Node> parsePrint(const std::vector<Token>& tokens);
    std::unique_ptr<Node> parseIdentifier(const std::vector<Token>& tokens);
    std::unique_ptr<Node> parseIf(const std::vector<Token>& tokens);
    std::unique_ptr<Node> parseConditional(const std::vector<Token>& tokens);
    std::unique_ptr<Node> parseExpression(const std::vector<Token>& tokens);
    std::unique_ptr<Node> parseTerm(const std::vector<Token>& tokens);
    std::unique_ptr<Node> parseFactor(const std::vector<Token>& tokens);
};