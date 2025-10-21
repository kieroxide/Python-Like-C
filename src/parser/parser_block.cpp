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

    // Count the expected indentation level by peeking ahead
    size_t expectedIndentLevel = 0;

    // Look ahead to count indentation without consuming
    while (peek().type == TokenType::INDENT) {
        // Count this indent level but don't consume yet
        size_t tempPos = tokenPosition;
        size_t indentCount = 0;

        while (tempPos < tokenLength && getTokens()[tempPos].type == TokenType::INDENT) {
            indentCount++;
            tempPos++;
        }

        expectedIndentLevel = indentCount;
        break;  // We just need the first line's indent count
    }

    // If no indentation found, return empty block
    if (expectedIndentLevel == 0) {
        return block;
    }

    // Parse statements at this indentation level
    while (peek().type != TokenType::_EOF) {
        // Count current line's indentation level
        size_t currentIndentLevel = 0;

        // Peek ahead to count indents on this line
        size_t tempPos = tokenPosition;
        while (tempPos < tokenLength && getTokens()[tempPos].type == TokenType::INDENT) {
            currentIndentLevel++;
            tempPos++;
        }

        // If we hit a line with less indentation, we're done with this block
        if (currentIndentLevel < expectedIndentLevel) {
            break;
        }

        // If this line is at our level, consume the indents and parse it
        if (currentIndentLevel == expectedIndentLevel) {
            // Consume the INDENT tokens using match()
            for (size_t i = 0; i < expectedIndentLevel; i++) {
                if (!match(TokenType::INDENT)) {
                    // Shouldn't happen, but safety check
                    break;
                }
            }

            // Skip any newlines before the statement
            while (match(TokenType::NEWLINE));

            auto child = parseStatement();
            if (child) {
                block->addChild(move(child));
            } else {
                // recover: try to advance to avoid infinite loop
                advance();
            }

            // consume trailing newlines after the statement
            while (match(TokenType::NEWLINE));
        } else {
            // Line is more indented - this will be handled by nested blocks
            break;
        }
    }

    return block;
}
