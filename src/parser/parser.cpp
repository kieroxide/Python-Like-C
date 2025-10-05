#include "parser.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

unique_ptr<Node> Parser::parseProgram(const vector<Token>& tokens) {
    tokenPosition = 0;
    tokenLength = tokens.size();

    const Token start = tokens[tokenPosition++];
    if (tokenPosition >= tokenLength && start.type != TokenType::BLOCK) {
        cerr << "Invalid Start to Program -- No Block Token";
    }

    unique_ptr<Node> program = make_unique<Node>();
    program->type = NodeType::PROGRAM;
    program->token = start;
    program->value = "Program";

    while (tokenPosition < tokenLength) {
        unique_ptr<Node> statement = parseStatement(tokens);
        // ignore nullptr statements these will be tokens that are not needed in AST
        if (statement) {
            program->addChild(move(statement));
        }
    }
    return program;
}

unique_ptr<Node> Parser::parseStatement(const vector<Token>& tokens) {
    Token token = tokens[tokenPosition++];
    switch (token.type) {
        case TokenType::PRINT: {
            auto printStmt = make_unique<Node>();

            printStmt->type = NodeType::PRINT;
            printStmt->token = token;
            printStmt->value = "PRINT";

            unique_ptr<Node> statement = parsePrint(tokens);
            printStmt->addChild(move(statement));
            return printStmt;
        }
        case TokenType::IDENTIFIER: {
            auto identifier = make_unique<Node>();
            identifier->token = token;
            identifier->type = NodeType::ASSIGN;
            unique_ptr<Node> statement = parseIdentifier(tokens);
            identifier->addChild(move(statement));
            return identifier;
        }
        case TokenType::IF: {
            auto ifStmt = parseIf(tokens);
            return ifStmt;
        }
        default:
            return nullptr;
    }
}

unique_ptr<Node> Parser::parseIf(const vector<Token>& tokens) {
    auto ifStmt = make_unique<Node>();
    const int ifPosition = tokenPosition - 1;  // Token for if is one behind pos
    ifStmt->token = tokens[ifPosition];
    ifStmt->type = NodeType::IF;
    ifStmt->value = ifStmt->token.value;

    auto condition = parseConditional(tokens);
    ifStmt->addChild(move(condition));

    if(tokens[tokenPosition++].type != TokenType::COLON){
        cerr << "Expecting Colon";
    }
    while (tokenPosition < (int)tokens.size() && tokens[tokenPosition].type == TokenType::NEWLINE) ++tokenPosition;

    auto block = make_unique<Node>();
    block->token = tokens[tokenPosition];
    block->type = NodeType::BLOCK;
    block->value = "BLOCK";

    // Loops over all concurrent indents
    while (tokenPosition < tokens.size() && tokens[tokenPosition].type == TokenType::INDENT) {
        ++tokenPosition;  // consume indent

        auto child = parseStatement(tokens);
        if (child) block->addChild(move(child));

        // Consumes trailing new lines
        while (tokenPosition < (int)tokens.size() && tokens[tokenPosition].type == TokenType::NEWLINE) {
            ++tokenPosition;
        }
    }
    ifStmt->addChild(move(block));
    return ifStmt;
}

unique_ptr<Node> Parser::parseConditional(const vector<Token>& tokens) {
    auto node = parseExpression(tokens);
    if (tokenPosition < tokens.size() &&
        (tokens[tokenPosition].type == TokenType::EQUALS || tokens[tokenPosition].type == TokenType::GREATERTHAN ||
         tokens[tokenPosition].type == TokenType::LESSTHAN)) {
        auto conditional = make_unique<Node>();
        conditional->token = tokens[tokenPosition++];
        conditional->type = NodeType::CONDITIONAL;
        conditional->value = conditional->token.value;

        auto right = parseExpression(tokens);
        conditional->addChild(move(node));
        conditional->addChild(move(right));
        node = move(conditional);
    }
    return node;
}
unique_ptr<Node> Parser::parseIdentifier(const vector<Token>& tokens) {
    Token token = tokens[tokenPosition++];
    return parseExpression(tokens);
}
unique_ptr<Node> Parser::parsePrint(const vector<Token>& tokens) {
    unique_ptr<Node> node = parseExpression(tokens);
    ++tokenPosition; // consumes colon
    return node;
}

unique_ptr<Node> Parser::parseExpression(const vector<Token>& tokens) {
    auto node = parseTerm(tokens);
    while (tokenPosition < tokens.size() &&
           (tokens[tokenPosition].type == TokenType::PLUS || tokens[tokenPosition].type == TokenType::SUBTRACT)) {
        Token token = tokens[tokenPosition++];

        auto op = make_unique<Node>();
        op->token = token;
        op->type = NodeType::OPERATOR;
        op->value = token.value;

        auto right = parseTerm(tokens);

        op->addChild(move(node));
        op->addChild(move(right));
        node = move(op);
    }
    return node;
}

unique_ptr<Node> Parser::parseTerm(const vector<Token>& tokens) {
    auto node = parseFactor(tokens);
    while (tokenPosition < tokens.size() &&
           (tokens[tokenPosition].type == TokenType::MULTIPLY || tokens[tokenPosition].type == TokenType::DIVIDE)) {
        Token token = tokens[tokenPosition++];

        auto op = make_unique<Node>();
        op->token = token;
        op->type = NodeType::OPERATOR;
        op->value = token.value;

        auto right = parseFactor(tokens);

        op->addChild(move(node));
        op->addChild(move(right));
        node = move(op);
    }
    return node;
}

unique_ptr<Node> Parser::parseFactor(const vector<Token>& tokens) {
    Token token = tokens[tokenPosition++];
    if (token.type == TokenType::NUMBER) {
        auto number = make_unique<Node>();
        number->token = token;
        number->type = NodeType::NUMBER;
        number->value = token.value;
        return number;
    } else if (token.type == TokenType::IDENTIFIER) {
        auto variable = make_unique<Node>();
        variable->token = token;
        variable->type = NodeType::VARIABLE;
        variable->value = token.value;
        return variable;
    }
}

void Parser::printAST(const unique_ptr<Node>& node, int indent) {
    if (!node)
        return;

    string indentation(indent, ' ');

    cout << indentation << "NodeType: " << static_cast<int>(node->type) << ", Token: " << node->token.value
         << ", Value: " << node->value << "\n";

    for (const auto& child : node->children) {
        printAST(child, indent + 2);
    }
}