#pragma once
#include <memory>
#include <string>
#include <vector>

#include "src/lexer/lexer.hpp"

enum class NodeType { PROGRAM, ASSIGN, PRINT, BLOCK, IF, CONDITIONAL, OPERATOR, VARIABLE, NUMBER };

struct Node {
    NodeType type;
    Token token;

    std::vector<std::unique_ptr<Node>> children;

    std::string value;

    void addChild(std::unique_ptr<Node> child) {
        children.push_back(std::move(child));
    }
};

class Parser {
   private:
    int tokenPosition = 0;
    int lineNumber = 0;
    int tokenLength = 0;

   public:
    static void printAST(const std::unique_ptr<Node>& node, int indent = 0);
    std::unique_ptr<Node> parseProgram(const std::vector<Token>& tokens);

   private:
    std::unique_ptr<Node> parseStatement(const std::vector<Token>& tokens);
    std::unique_ptr<Node> parsePrint(const std::vector<Token>& tokens);
    std::unique_ptr<Node> parseIdentifier(const std::vector<Token>& tokens);
    std::unique_ptr<Node> parseIf(const std::vector<Token>& tokens);
    std::unique_ptr<Node> parseConditional(const std::vector<Token>& tokens);
    std::unique_ptr<Node> parseExpression(const std::vector<Token>& tokens);
    std::unique_ptr<Node> parseTerm(const std::vector<Token>& tokens);
    std::unique_ptr<Node> parseFactor(const std::vector<Token>& tokens);
};