#pragma once
#include <string>
#include <memory>
#include <vector>
#include "../lexer/lexer.hpp"

namespace parser{
    enum class NodeType{
            PROGRAM,
            ASSIGN,
            PRINT,
            IF,
            OPERATOR,
            VARIABLE,
            NUMBER
    };
    
    struct Node{
        NodeType type;
        lexer::Token token;

        std::vector<std::unique_ptr<Node>> children;

        std::string value;

        void addChild(std::unique_ptr<Node>& child) {
            children.push_back(std::move(child));
        }
    };

    std::unique_ptr<Node> parse(const std::vector<lexer::Token>& tokens);

    std::unique_ptr<Node> parsePrint(const std::vector<lexer::Token>& tokens, int& pos);
    std::unique_ptr<Node> parseIdentifier(const std::vector<lexer::Token>& tokens, int& pos);
    std::unique_ptr<Node> parseIf(const std::vector<lexer::Token>& tokens, int& pos);

    std::unique_ptr<Node> parseExpression(const std::vector<lexer::Token>& tokens, int& pos);
    std::unique_ptr<Node> parseTerm(const std::vector<lexer::Token>& tokens, int& pos);
    std::unique_ptr<Node> parseFactor(const std::vector<lexer::Token>& tokens, int& pos);
    
    void printAST(const std::unique_ptr<Node>& node, int indent = 0);
}