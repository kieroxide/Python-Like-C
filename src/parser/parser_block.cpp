#include <iostream>

#include "src/parser/parser.hpp"

using namespace std;

unique_ptr<Node> Parser::parseBlockUntil(TokenType terminator) {
    auto blockNode = make_unique<Node>(NodeType::BLOCK, "BLOCK");
    while (peek().type != terminator && peek().type != TokenType::_EOF) {
        if (match(TokenType::NEWLINE)) continue;  // skip newlines

        auto stmt = parseStatement();
        if (stmt) {
            blockNode->addChild(move(stmt));
        } else {
            // avoid infinite loop on parse error
            if (tokenPosition < tokenLength) {
                ++tokenPosition;
            } else {
                break;
            }
        }
    }
    return blockNode;
}

unique_ptr<Node> Parser::parseFunction() {
    if (!consume(TokenType::IDENTIFIER, "function name")) return nullptr;

    // Consume and create head function node
    Token funcName = current();
    auto funcNode = make_unique<Node>(NodeType::DEF, funcName, funcName.value);

    if (!consume(TokenType::LPAREN, "(")) return nullptr;

    // Consume Parameters and add to function Node
    if (!parseParams(funcNode)) return nullptr;

    if (!consume(TokenType::LBRACE, "{")) return nullptr;

    // Parse function body as a block
    auto blockNode = parseBlockUntil(TokenType::RBRACE);

    if (!consume(TokenType::RBRACE, "}")) return nullptr;

    funcNode->addChild(move(blockNode));
    return funcNode;
}

bool Parser::parseParams(unique_ptr<Node>& funcNode) {
    // Loop until we see a closing parenthesis
    bool first = true;
    while (peek().type != TokenType::RPAREN && peek().type != TokenType::_EOF) {
        // Handle comma between parameters (but not before first param)
        if (!first) {
            if (!consume(TokenType::COMMA, ",")) return false;
        }

        // Create parameter node and add to func head node
        if (!consume(TokenType::IDENTIFIER, "Parameter")) return false;

        auto paramToken = current();
        auto paramNode = make_unique<Node>(NodeType::PARAM, paramToken, paramToken.value);
        funcNode->addChild(move(paramNode));

        first = false;
    }

    // Consume closing parenthesis
    if (!consume(TokenType::RPAREN, ")")) return false;
    return true;
}

unique_ptr<Node> Parser::parseIndentedBlock() {
    auto block = make_unique<Node>(NodeType::BLOCK, "BLOCK");

    // Each INDENT introduces one statement
    while (match(TokenType::INDENT)) {
        while (match(TokenType::NEWLINE));

        auto child = parseStatement();
        if (child) {
            block->addChild(move(child));
        } else {
            // recover: advance one token to avoid infinite loop
            if (tokenPosition < tokenLength) {
                ++tokenPosition;
            } else {
                break;
            }
        }

        // consume trailing newlines after the statement
        while (match(TokenType::NEWLINE));
    }

    return block;
}
