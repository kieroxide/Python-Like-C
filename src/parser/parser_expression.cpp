#include <iostream>

#include "src/parser/parser.hpp"

using namespace std;

unique_ptr<Node> Parser::parseExpression() {
    auto node = parseTerm();
    if (!node) {
        cerr << "Error: Invalid or missing term in expression\n";
        return nullptr;
    }

    // Loops through all + and - found while consuming each operator and right operand
    while (peek().type == TokenType::PLUS || peek().type == TokenType::SUBTRACT) {
        Token token = advance();

        // Consume operator and right term
        auto op = make_unique<Node>(NodeType::OPERATOR, token, token.value);
        auto right = parseTerm();
        if (!right) {
            cerr << "Error: Missing right operand after '" << token.value << "' at line " << token.lineNumber << "\n";
            return nullptr;
        }

        // Structure as condtional is head and left and right are children
        op->addChild(move(node));
        op->addChild(move(right));
        node = move(op);
    }
    return node;
}

unique_ptr<Node> Parser::parseTerm() {
    auto node = parseFactor();
    if (!node) {
        cerr << "Error: Invalid or missing factor in term\n";
        return nullptr;
    }

    // Loops through all * and / found while consuming each operator and right operand
    while (peek().type == TokenType::MULTIPLY || peek().type == TokenType::DIVIDE) {
        Token token = advance();

        auto op = make_unique<Node>(NodeType::OPERATOR, token, token.value);
        auto right = parseFactor();
        if (!right) {
            cerr << "Error: Missing right operand after '" << token.value << "' at line " << token.lineNumber << "\n";
            return nullptr;
        }

        op->addChild(move(node));
        op->addChild(move(right));
        node = move(op);
    }
    return node;
}

unique_ptr<Node> Parser::parseFactor() {
    if (tokenPosition >= tokenLength) {
        return nullptr;
    }

    if (peek().type == TokenType::NUMBER) {
        auto numToken = advance();
        return make_unique<Node>(NodeType::NUMBER, numToken, numToken.value);
    } else if (peek().type == TokenType::IDENTIFIER) {
        Token identifier = advance();
        // Check if this is a function call
        if (peek().type == TokenType::LPAREN) return parseFunctionCall();

        // Otherwise it's a variable
        return make_unique<Node>(NodeType::VARIABLE, identifier, identifier.value);
    } else {
        auto token = peek();
        cerr << "Error: Expected number or identifier, found '" << token.value << "' at line " << token.lineNumber
             << "\n";
        return nullptr;
    }
}

unique_ptr<Node> Parser::parseConditional() {
    auto node = parseExpression();  // parses and consumes expr
    if (!node) {
        cerr << "Error: Invalid or missing expression in condition\n";
        return nullptr;
    }

    if (tokenPosition >= tokenLength) {
        cerr << "Error: Unexpected end of tokens while parsing condition\n";
        return nullptr;
    }

    if (peek().type == TokenType::EQUALS || peek().type == TokenType::GREATERTHAN ||
        peek().type == TokenType::LESSTHAN) {
        // Consume Conditional
        Token condToken = advance();
        auto conditional = make_unique<Node>(NodeType::CONDITIONAL, condToken, condToken.value);

        // Consume Right Operand
        auto right = parseExpression();
        if (!right) return nullptr;

        // Structure as condtional is head and left and right are children
        conditional->addChild(move(node));
        conditional->addChild(move(right));
        node = move(conditional);

    } else {
        cerr << "Expected condition at line " << (tokenPosition > 0 ? current().lineNumber : -1) << "\n";
        return nullptr;
    }

    return node;
}