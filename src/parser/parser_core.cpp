#include <iostream>

#include "src/parser/parser.hpp"

using namespace std;

/* Checks current token for provided type. Returns true/false if so */
bool Parser::match(TokenType type) {
    if (tokenPosition < tokenLength && getTokens()[tokenPosition].type == type) {
        ++tokenPosition;
        return true;
    }
    return false;
}

/* Checks current token for provided type. Errors if type is not what is expected */
bool Parser::consume(TokenType type, const string& what) {
    if (tokenPosition < tokenLength && getTokens()[tokenPosition].type == type) {
        ++tokenPosition;
        return true;
    }
    cerr << "Expected " << what << " at line " << (tokenPosition > 0 ? current().lineNumber : -1) << "\n";
    return false;
}

/* Returns Reference to current current without advancing position*/
Token const& Parser::peek() const {
    static const Token eofToken{TokenType::_EOF, "EOF"};
    if (tokenPosition < tokenLength) {
        return getTokens()[tokenPosition];
    }
    return eofToken;
}

/* Returns last consumed token or EOF */
Token const& Parser::current() const {
    static const Token eofToken{TokenType::_EOF, "EOF"};
    if (tokenPosition == 0) return eofToken;  // nothing consumed yet
    size_t tokenIndex = tokenPosition - 1;
    if (tokenIndex < tokenLength) return getTokens()[tokenIndex];  // last consumed token
    return eofToken;
}

/* Returns current token then advances position*/
Token Parser::advance() {
    static const Token eofToken{TokenType::_EOF, "EOF"};
    if (tokenPosition < tokenLength) {
        return getTokens()[tokenPosition++];  // return current token then advance
    }
    return eofToken;  // safe EOF token
}

unique_ptr<Node> Parser::parseProgram(const vector<Token>& tokens) {
    tokensRef = &tokens;
    tokenPosition = 0;
    tokenLength = tokens.size();

    // Make program node to be the head of the AST
    auto program = make_unique<Node>(NodeType::PROGRAM, "Program");

    // Loop until all tokens are parsed
    while (peek().type != TokenType::_EOF) {
        auto statement = parseStatement();

        // ignore nullptr statements these will be tokens that are not needed in AST
        if (statement) {
            program->addChild(move(statement));

        } else {
            // if statement returned nullptr, try to advance to avoid infinite loop
            if (tokenPosition < tokenLength) {
                ++tokenPosition;
            } else {
                break;
            }
        }
    }
    return program;
}

void Parser::printAST(const unique_ptr<Node>& node, int indent) {
    if (!node) return;

    string indentation(indent, ' ');

    cout << indentation << "NodeType: " << static_cast<int>(node->type) << ", Token: " << node->token.value
         << ", Value: " << node->value << "\n";

    for (const auto& child : node->children) {
        printAST(child, indent + 2);
    }
}