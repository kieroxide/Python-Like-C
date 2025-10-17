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
            bool allowAssignment = true;
            return parseIdentifier(allowAssignment);
        }

        case TokenType::IF: {
            advance();
            return parseIf();
            ;
        }

        case TokenType::DEF: {
            advance();
            return parseFunction();
        }

        case TokenType::WHILE: {
            advance();
            return parseWhile();
        }

        case TokenType::RETURN: {
            return parseReturn();
        }

        default:
            // Unneeded Tokens
            return nullptr;
    }
}

unique_ptr<Node> Parser::parseWhile() {
    // we need to parse (conditional): (indent) -> block
    const auto whileToken = current();  // Already consumed
    auto whileStmt = make_unique<Node>(NodeType::WHILE, whileToken, whileToken.value);

    if (!consume(TokenType::LPAREN, "(")) return nullptr;

    // Consume condition
    auto condition = parseConditional();
    if (!condition) return nullptr;
    whileStmt->addChild(move(condition));

    if (!consume(TokenType::RPAREN, ")")) return nullptr;

    if (!consume(TokenType::COLON, ":")) return nullptr;

    // Consume any newLines
    while (match(TokenType::NEWLINE));

    auto block = parseIndentedBlock();

    whileStmt->addChild(move(block));
    return whileStmt;
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

    auto block = parseIndentedBlock();

    ifStmt->addChild(move(block));
    return ifStmt;
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

unique_ptr<Node> Parser::parseIndexExpr() {
    if (!consume(TokenType::LSQUARE, "[")) return nullptr;
    auto index = parseExpression();
    if (!consume(TokenType::RSQUARE, "]")) return nullptr;
    return move(index);
}

// Parses a index access node
unique_ptr<Node> Parser::parseIndex(unique_ptr<Node>& varNode) {
    // parse indexExpr and create index node and return
    auto indexNode = make_unique<Node>(NodeType::INDEX, "index");
    auto index = parseIndexExpr();

    indexNode->addChild(move(varNode));
    indexNode->addChild(move(index));

    return move(indexNode);
}

unique_ptr<Node> Parser::parseIdentifier(bool allowAssignment) {
    auto idToken = advance();
    if (peek().type == TokenType::LPAREN) {
        return parseFunctionCall();
    } else if (peek().type == TokenType::LSQUARE) {
        return parseIndexAccess(allowAssignment);
    }

    // Plain identifier
    if (!allowAssignment) {
        // Expression context -> just a VARIABLE node
        return make_unique<Node>(NodeType::VARIABLE, idToken, idToken.value);
    }

    // Else we just assign the variable to a regular assignment
    auto assignNode = make_unique<Node>(NodeType::ASSIGN, idToken.value);
    auto variable = make_unique<Node>(NodeType::VARIABLE, idToken, idToken.value);
    assignNode->addChild(move(variable));

    if (consume(TokenType::ASSIGN, "=")) {
        auto statement = parseExpression();  // Parse right side of assignment
        if (statement) {
            assignNode->addChild(move(statement));
        }
    }

    return move(assignNode);
}

unique_ptr<Node> Parser::parseReturn() {
    advance();
    auto returnStmt = make_unique<Node>(NodeType::RETURN, "RETURN");

    unique_ptr<Node> expression = parseExpression();
    if (expression) {
        returnStmt->addChild(move(expression));
    }
    return returnStmt;
}

unique_ptr<Node> Parser::parseIndexAccess(bool allowAssignment) {
    auto idToken = current();
    // Create a variable node first
    auto varNode = make_unique<Node>(NodeType::VARIABLE, idToken, idToken.value);

    // Parse the index to create index node
    auto indexNode = parseIndex(varNode);

    // If assignment follows, create assign node with index as left child
    if (allowAssignment && peek().type == TokenType::ASSIGN) {
        advance();  // consume =
        auto assignNode = make_unique<Node>(NodeType::ASSIGN, idToken, "ASSIGN");
        assignNode->addChild(move(indexNode));
        auto rhs = parseExpression();  // RHS = expression
        assignNode->addChild(move(rhs));
        return move(assignNode);
    }

    // Just returning the index expression for array access
    return move(indexNode);
}