#include <iostream>

#include "src/parser/parser.hpp"

using namespace std;

unique_ptr<Node> Parser::parseStatement() {
    // Skip newlines
    while (match(TokenType::NEWLINE));

    if (peek().type == TokenType::_EOF || tokenPosition >= tokenLength) return nullptr;

    // Consume Token and Parse
    const auto& token = peek();
    switch (token.type) {
        case TokenType::PRINT: {
            advance();
            auto printStmt = make_unique<Node>(NodeType::PRINT, token, "PRINT");
            auto statement = parsePrint();
            printStmt->addChild(move(statement));
            return printStmt;
        }

        case TokenType::IDENTIFIER: {
            auto idToken = advance();
            if (peek().type == TokenType::LPAREN) {
                return parseFunctionCall();
            }

            auto identifier = make_unique<Node>(NodeType::ASSIGN, idToken, idToken.value);
            auto statement = parseIdentifier();
            if (statement) {
                identifier->addChild(move(statement));
            }

            return identifier;
        }

        case TokenType::IF: {
            advance();
            auto ifStmt = parseIf();
            return ifStmt;
        }

        case TokenType::DEF: {
            advance();
            return parseFunction();
        }

        case TokenType::RETURN: {
            advance();
            auto returnStmt = make_unique<Node>(NodeType::RETURN, token, "RETURN");

            unique_ptr<Node> expression = parseExpression();
            if (expression) {
                returnStmt->addChild(move(expression));
            }
            return returnStmt;
        }

        default:
            // Unneeded Tokens
            return nullptr;
    }
}

unique_ptr<Node> Parser::parseIf() {
    // If was already consumed so current
    const auto ifToken = current();
    auto ifStmt = make_unique<Node>(NodeType::IF, ifToken, ifToken.value);

    // Consume condition
    auto condition = parseConditional();  // Moves tokenPosition to Colon
    if (!condition) return nullptr;
    ifStmt->addChild(move(condition));

    if (!consume(TokenType::COLON, ":")) return nullptr;

    // Consume any newLines
    while (match(TokenType::NEWLINE));

    auto block = make_unique<Node>(NodeType::BLOCK, "BLOCK");
    bool foundIndent = false;

    // Loops over all concurrent indents
    while (match(TokenType::INDENT)) {
        foundIndent = true;

        auto child = parseStatement();  // Recursive call to parseStatement
        if (child) {
            block->addChild(move(child));
        }

        // Consumes trailing new lines
        while (match(TokenType::NEWLINE));
    }
    if (!foundIndent) {
        cerr << "Warning: Empty if block at line " << ifToken.lineNumber << "\n";
    }

    ifStmt->addChild(move(block));
    return ifStmt;
}

unique_ptr<Node> Parser::parseIdentifier() {
    // If we see a '(' here, it's a function call
    if (peek().type == TokenType::LPAREN) return parseFunctionCall();

    // Otherwise, we expect an assignment
    if (consume(TokenType::ASSIGN, "=")) {
        return parseExpression();  // Parse right side of assignment
    }
    return nullptr;
}

unique_ptr<Node> Parser::parsePrint() {
    // Check if there's anything to parse
    if (peek().type == TokenType::NEWLINE) {
        cerr << "Error: Empty print statement at line " << peek().lineNumber << "\n";
        return nullptr;
    }

    unique_ptr<Node> node = parseExpression();
    return node;
}

unique_ptr<Node> Parser::parseFunctionCall() {
    // Function name token was already consumed
    const Token& funcCallToken = current();
    auto callNode = make_unique<Node>(NodeType::FUNC_CALL, funcCallToken, funcCallToken.value);

    // Consume the '('
    if (!consume(TokenType::LPAREN, "(")) return nullptr;

    // Parse arguments
    bool first = true;
    while (peek().type != TokenType::RPAREN && peek().type != TokenType::_EOF) {
        if (!first) {
            if (!consume(TokenType::COMMA, ",")) return nullptr;
        }

        auto arg = parseExpression();
        if (arg)
            callNode->addChild(move(arg));
        else {
            cerr << "Invalid argument in function call\n";
            return nullptr;
        }
        first = false;
    }
    if (!consume(TokenType::RPAREN, ")")) return nullptr;
    return callNode;
}