#include "parser.hpp"

namespace parser {
    
    std::unique_ptr<Node> parse(const std::vector<lexer::Token>& tokens, int& pos){
        auto stmt = std::make_unique<Node>();
        int len = tokens.size();
        pos = pos;

        lexer::Token token = tokens[pos];
        pos++;
        if(token.type == lexer::TokenType::PRINT){
            auto printStmt = std::make_unique<Node>();

            printStmt->type = NodeType::PRINT_STATEMENT;
            printStmt->token = token;

            auto statement = parsePrint(tokens, pos, printStmt);
            printStmt->addChild(statement);
            stmt = std::move(printStmt);
        }
        else if(token.type == lexer::TokenType::IDENTIFIER){
            auto identifier = std::make_unique<Node>();
            identifier->token = token;
            identifier->type = NodeType::ASSIGN_STATEMENT;
            auto statement = parse(tokens, pos);
            identifier->addChild(statement);
            stmt = std::move(identifier);
        }
        else if(token.type == lexer::TokenType::ASSIGN){
            auto assign = std::make_unique<Node>();
            assign->token = token;
            assign->type = NodeType::OPERATOR;
            assign->
        }
        return stmt;
    }

    std::unique_ptr<Node> parsePrint(const std::vector<lexer::Token>& tokens, int& pos, std::unique_ptr<Node>& printStmt){
        lexer::Token token = tokens[pos];
        if(token.type == lexer::TokenType::NUMBER){
            auto number = std::make_unique<Node>();
            number->token = token;
            number->type = NodeType::NUMBER;
            printStmt->addChild(number);
        }
        else if(token.type == lexer::TokenType::IDENTIFIER){
            auto identifier = std::make_unique<Node>();
            identifier->token = token;
            identifier->type = NodeType::VARIABLE;
            identifier->value = token.value;
            printStmt->token;
        }
    }

    std::unique_ptr<Node> parseExpression(const std::vector<lexer::Token>& tokens, int& pos, std::unique_ptr<Node>& printStmt){}
        lexer::Token token = tokens[pos];
}