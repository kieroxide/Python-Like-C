#pragma once
#include <memory>
#include <string>
#include <vector>

#include "src/lexer/Lexer.hpp"

enum class NodeType {
    PROGRAM,
    DEF,
    RETURN,
    PARAM,
    WHILE,
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

    void addChild(std::unique_ptr<Node> child) { children.push_back(std::move(child)); }

    Node() {}

    Node(NodeType type, std::string value) {
        this->type = type;
        this->value = value;
    }

    Node(NodeType type, Token token, std::string value) {
        this->type = type;
        this->token = token;
        this->value = value;
    }

    Node* clone() const {
        // Create a new node with the same properties
        Node* newNode = new Node();
        newNode->type = this->type;
        newNode->value = this->value;
        newNode->token = this->token;

        // Clone all children recursively
        for (const auto& child : this->children) {
            if (child) {
                newNode->children.push_back(std::unique_ptr<Node>(child->clone()));
            }
        }

        return newNode;
    }
};

class Parser {
   private:
    const std::vector<Token>* tokensRef = nullptr;

    const std::vector<Token>& getTokens() const {
        static const std::vector<Token> empty{};
        return tokensRef ? *tokensRef : empty;
    }

    size_t tokenLength = 0;
    size_t tokenPosition = 0;

   public:
    static void printAST(const std::unique_ptr<Node>& node, int indent = 0);
    std::unique_ptr<Node> parseProgram(const std::vector<Token>& tokens);

   private:
    bool parseParams(std::unique_ptr<Node>& funcNode);
    bool match(TokenType t);
    bool consume(TokenType t, const std::string& what);
    Token const& peek() const;
    Token const& current() const;
    Token advance();
    std::unique_ptr<Node> parseIndentedBlock();
    std::unique_ptr<Node> parseBlockUntil(TokenType terminator);
    std::unique_ptr<Node> parseFunction();
    std::unique_ptr<Node> parseWhile();
    std::unique_ptr<Node> parseFunctionCall();
    std::unique_ptr<Node> parseStatement();
    std::unique_ptr<Node> parsePrint();
    std::unique_ptr<Node> parseIdentifier();
    std::unique_ptr<Node> parseIf();
    std::unique_ptr<Node> parseConditional();
    std::unique_ptr<Node> parseExpression();
    std::unique_ptr<Node> parseTerm();
    std::unique_ptr<Node> parseFactor();
};
