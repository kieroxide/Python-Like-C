#include "parser.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

namespace parser {
    
    std::unique_ptr<Node> parse(const std::vector<lexer::Token>& tokens){
        int len = tokens.size();
        int pos = 0;

        while(pos < len){
            lexer::Token token = tokens[pos++];
            if(token.type == lexer::TokenType::PRINT){
                auto printStmt = std::make_unique<Node>();
                
                printStmt->type = NodeType::PRINT_STATEMENT;
                printStmt->token = token;
                
                auto statement = parsePrint(tokens, pos);
                printStmt->addChild(statement);
                return printStmt;
            }
            //else if(token.type == lexer::TokenType::IDENTIFIER){
            //    auto identifier = std::make_unique<Node>();
            //    identifier->token = token;
            //    identifier->type = NodeType::ASSIGN_STATEMENT;
            //    auto statement = parse(tokens, pos);
            //    identifier->addChild(statement);
            //    stmt = std::move(identifier);
            //}
            //else if(token.type == lexer::TokenType::ASSIGN){
            //    auto assign = std::make_unique<Node>();
            //    assign->token = token;
            //    assign->type = NodeType::OPERATOR;
            //}

        }
    }

    std::unique_ptr<Node> parsePrint(const std::vector<lexer::Token>& tokens, int& pos){
        lexer::Token token = tokens[pos];
        std::unique_ptr<Node> node;
        node = parseExpression(tokens, pos);
        return node;
    }

    std::unique_ptr<Node> parseExpression(const std::vector<lexer::Token>& tokens, int& pos){
        auto node = parseTerm(tokens, pos);
        while(pos < tokens.size() && tokens[pos].type == lexer::TokenType::PLUS){
            lexer::Token token = tokens[pos++];
            auto op = std::make_unique<Node>();
            op->token = token;
            op->type = NodeType::OPERATOR;
            op->value = token.value;
            auto right = parseTerm(tokens, pos);

            op->addChild(node);
            op->addChild(right);
            node = std::move(op);
        }
        return node;
    }

    std::unique_ptr<Node> parseTerm(const std::vector<lexer::Token>& tokens, int& pos){
        auto node = parseFactor(tokens, pos);
        while(pos < tokens.size() && tokens[pos].type == lexer::TokenType::MULTIPLY){
            lexer::Token token = tokens[pos++];
            auto op = std::make_unique<Node>();
            op->token = token;
            op->type = NodeType::OPERATOR;
            op->value = token.value;
            auto right = parseFactor(tokens, pos);

            op->addChild(node);
            op->addChild(right);
            node = std::move(op);
        }
        return node;
    }
    std::unique_ptr<Node> parseFactor(const std::vector<lexer::Token>& tokens, int& pos){
        lexer::Token token = tokens[pos++];
        if(token.type == lexer::TokenType::NUMBER){
            auto number = std::make_unique<Node>();
            number->token = token;
            number->type = NodeType::NUMBER;
            number->value = token.value;
            return number;
        } 
        else if(token.type == lexer::TokenType::IDENTIFIER){
            auto variable = std::make_unique<Node>();
            variable->token = token;
            variable->type = NodeType::VARIABLE;
            variable->value = token.value;
            return variable;
        }
    }

    void printAST(const std::unique_ptr<Node>& node, int indent) {
        if (!node) return;

        std::string indentation(indent, ' ');

        std::cout << indentation 
                  << "NodeType: " << static_cast<int>(node->type)
                  << ", Token: " << node->token.value
                  << ", Value: " << node->value << "\n";

        for (const auto& child : node->children) {
            printAST(child, indent + 2); 
        }
    }
}