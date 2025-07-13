#pragma once
#include <string>
#include <memory>
#include <vector>
#include "lexer/lexer.hpp"
namespace parser{
    enum class NodeType{
            PROGRAM,
            ASSIGN_STATEMENT,
            PRINT_STATEMENT,
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

    std::unique_ptr<Node> parsePrint(const std::vector<lexer::Token>& tokens, int& pos);
    std::unique_ptr<Node> parse(const std::vector<lexer::Token>& tokens, int& pos);
}