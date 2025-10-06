#include "parser.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

unique_ptr<Node> Parser::parseProgram(const vector<Token>& tokens) {
    tokenPosition = 0;
    tokenLength = tokens.size();

    // Make program node to be the head of the AST
    unique_ptr<Node> program = make_unique<Node>(NodeType::PROGRAM, "Program");

    // Loop until all tokens are parsed
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
    if (tokenPosition >= tokenLength) {
        return nullptr;
    }

    // Skip newlines
    while (tokenPosition < tokenLength && tokens[tokenPosition].type == TokenType::NEWLINE) {
        ++tokenPosition;
    }

    if (tokenPosition >= tokenLength) {
        return nullptr;
    }

    // Consume Token and Parse
    Token token = tokens[tokenPosition++];
    switch (token.type) {
        case TokenType::PRINT: {
            auto printStmt = make_unique<Node>(NodeType::PRINT, token, "PRINT");
            unique_ptr<Node> statement = parsePrint(tokens);
            printStmt->addChild(move(statement));
            return printStmt;
        }

        case TokenType::IDENTIFIER: {
            auto identifier = make_unique<Node>(NodeType::ASSIGN, token, token.value);
            unique_ptr<Node> statement = parseIdentifier(tokens);
            identifier->addChild(move(statement));
            return identifier;
        }

        case TokenType::IF: {
            auto ifStmt = parseIf(tokens);
            return ifStmt;
        }

        default:
            // Unneeded Tokens
            return nullptr;
    }
}

unique_ptr<Node> Parser::parseIf(const vector<Token>& tokens) {
    // Consume "if" token
    const int ifPosition = tokenPosition - 1;  // Token for if is one behind pos
    const auto ifToken = tokens[ifPosition];
    auto ifStmt = make_unique<Node>(NodeType::IF, ifToken, ifToken.value);

    // Consume condtion
    auto condition = parseConditional(tokens);  // Moves tokenPosition to Colon
    ifStmt->addChild(move(condition));

    if (tokenPosition < tokenLength && tokens[tokenPosition].type == TokenType::COLON) {
        ++tokenPosition;  // Consume Colon
    } else {
        cerr << "Expecting Colon" << "\n";
    }

    // Consume any newLines
    while (tokenPosition < (int)tokens.size() && tokens[tokenPosition].type == TokenType::NEWLINE) ++tokenPosition;

    auto block = make_unique<Node>(NodeType::BLOCK, "BLOCK");
    bool foundIndent = false;

    // Loops over all concurrent indents
    while (tokenPosition < tokens.size() && tokens[tokenPosition].type == TokenType::INDENT) {
        foundIndent = true;
        ++tokenPosition;  // consume indent

        auto child = parseStatement(tokens);  // Recursive call to parseStatement
        if (child) {
            block->addChild(move(child));
        }

        // Consumes trailing new lines
        while (tokenPosition < (int)tokens.size() && tokens[tokenPosition].type == TokenType::NEWLINE) {
            ++tokenPosition;
        }
    }
    if (!foundIndent) {
        cerr << "Warning: Empty if block at line " << ifToken.lineNumber << "\n";
    }

    ifStmt->addChild(move(block));
    return ifStmt;
}

unique_ptr<Node> Parser::parseConditional(const vector<Token>& tokens) {
    auto node = parseExpression(tokens);  // parses and consumes expr
    if (!node) {
        cerr << "Error: Invalid or missing expression in condition\n";
        return nullptr;
    }

    if (tokenPosition >= tokens.size()) {
        cerr << "Error: Unexpected end of tokens while parsing condition\n";
        return nullptr;
    }

    const bool isComparisonOperator =
        (tokens[tokenPosition].type == TokenType::EQUALS || tokens[tokenPosition].type == TokenType::GREATERTHAN ||
         tokens[tokenPosition].type == TokenType::LESSTHAN);

    if (tokenPosition < tokens.size() && isComparisonOperator) {
        // Consume Conditional
        Token condToken = tokens[tokenPosition++];
        auto conditional = make_unique<Node>(NodeType::CONDITIONAL, condToken, condToken.value);

        // Consume Right Operand
        auto right = parseExpression(tokens);

        // Structure as condtional is head and left and right are children
        conditional->addChild(move(node));
        conditional->addChild(move(right));
        node = move(conditional);

    } else {
        cerr << "Expected Condition" << tokens[tokenPosition - 1].lineNumber << "\n";
        return nullptr;
    }

    return node;
}

unique_ptr<Node> Parser::parseIdentifier(const vector<Token>& tokens) {
    // Check if next token is '='
    if (tokenPosition < tokens.size() && tokens[tokenPosition].type == TokenType::ASSIGN) {
        ++tokenPosition;  // consume the '=' token
        return parseExpression(tokens);  // Parse right side of assignment
    } else {
        cerr << "Expected '=' after identifier at line " << tokens[tokenPosition - 1].lineNumber << "\n";
        return nullptr;
    }
}

unique_ptr<Node> Parser::parsePrint(const vector<Token>& tokens) {
    // Check if there's anything to parse
    if (tokenPosition >= tokens.size() || tokens[tokenPosition].type == TokenType::NEWLINE) {
        cerr << "Error: Empty print statement at line " << tokens[tokenPosition - 1].lineNumber << "\n";
        return nullptr;
    }

    unique_ptr<Node> node = parseExpression(tokens);
    return node;
}

unique_ptr<Node> Parser::parseExpression(const vector<Token>& tokens) {
    auto node = parseTerm(tokens);
    if (!node) {
        cerr << "Error: Invalid or missing term in expression\n";
        return nullptr;
    }

    // Loops through all + and - found while consuming each operator and right operand
    while (tokenPosition < tokens.size() &&
           (tokens[tokenPosition].type == TokenType::PLUS || tokens[tokenPosition].type == TokenType::SUBTRACT)) {
        Token token = tokens[tokenPosition++];

        // Consume operator and right term
        auto op = make_unique<Node>(NodeType::OPERATOR, token, token.value);
        auto right = parseTerm(tokens);
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

unique_ptr<Node> Parser::parseTerm(const vector<Token>& tokens) {
    auto node = parseFactor(tokens);
    if (!node) {
        cerr << "Error: Invalid or missing factor in term\n";
        return nullptr;
    }

    // Loops through all * and / found while consuming each operator and right operand
    while (tokenPosition < tokens.size() &&
           (tokens[tokenPosition].type == TokenType::MULTIPLY || tokens[tokenPosition].type == TokenType::DIVIDE)) {
        Token token = tokens[tokenPosition++];

        auto op = make_unique<Node>(NodeType::OPERATOR, token, token.value);
        auto right = parseFactor(tokens);
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

unique_ptr<Node> Parser::parseFactor(const vector<Token>& tokens) {
    if (tokenPosition >= tokenLength) {
        cerr << "Error: Unexpected end of tokens while parsing factor\n";
        return nullptr;
    }

    Token token = tokens[tokenPosition++];

    if (token.type == TokenType::NUMBER) {
        auto number = make_unique<Node>(NodeType::NUMBER, token, token.value);
        return number;
    } else if (token.type == TokenType::IDENTIFIER) {
        auto variable = make_unique<Node>(NodeType::VARIABLE, token, token.value);
        return variable;
    } else {
        cerr << "Error: Expected number or identifier, found '" << token.value << "' at line " << token.lineNumber
             << "\n";
        return nullptr;
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