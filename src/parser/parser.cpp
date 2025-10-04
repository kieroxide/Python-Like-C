#include "parser.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

namespace parser {
    
    std::unique_ptr<Node> parseStatement(const std::vector<lexer::Token>& tokens){
        int len = tokens.size();
        int pos = 0;

        while(pos < len){
            lexer::Token token = tokens[pos++];
            if(token.type == lexer::TokenType::PRINT){
                auto printStmt = std::make_unique<Node>();
                
                printStmt->type = NodeType::PRINT;
                printStmt->token = token;
                printStmt->value = "PRINT";
                
                auto statement = parsePrint(tokens, pos);
                printStmt->addChild(std::move(statement));
                return printStmt;
            }
            else if(token.type == lexer::TokenType::IDENTIFIER){
                auto identifier = std::make_unique<Node>();
                identifier->token = token;
                identifier->type = NodeType::ASSIGN;
                auto statement = parseIdentifier(tokens, pos);
                identifier->addChild(std::move(statement));
                return identifier;
            } else if(token.type == lexer::TokenType::IF){
                auto ifStmt = parseIf(tokens, pos);
                return ifStmt;
            }
        }
    }

    std::unique_ptr<Node> parseIf(const std::vector<lexer::Token>& tokens, int& pos){
        pos--;
        auto ifStmt = std::make_unique<Node>();
        ifStmt->token = tokens[pos++];
        ifStmt->type = NodeType::IF;
        ifStmt->value = ifStmt->token.value;

        auto condition = parseConditional(tokens, pos);
        ifStmt->addChild(std::move(condition));

        ++pos; // for colon

        auto block = std::make_unique<Node>();
        block->token = tokens[pos++];
        block->type = NodeType::BLOCK;
        block->value = "BLOCK";

        pos--;
        while(pos < tokens.size() && tokens[pos].type == lexer::TokenType::INDENT){
            pos++;
            std::vector<lexer::Token> tokenLine;
            // Collects tokens until next INDENT or end
            while(pos < tokens.size() && tokens[pos].type != lexer::TokenType::INDENT){
                tokenLine.push_back(tokens[pos++]);
            }
            if(!tokenLine.empty()){
                auto child = parseStatement(tokenLine);
                block->addChild(std::move(child));

            }
        }
        ifStmt->addChild(std::move(block));
        return ifStmt;
    }
    std::unique_ptr<Node> parseConditional(const std::vector<lexer::Token>& tokens, int& pos){
        auto node = parseExpression(tokens, pos);
        if( pos < tokens.size() && (tokens[pos].type == lexer::TokenType::EQUALS ||
            tokens[pos].type == lexer::TokenType::GREATERTHAN ||
            tokens[pos].type == lexer::TokenType::LESSTHAN )){
                auto conditional = std::make_unique<Node>();
                conditional->token = tokens[pos++];
                conditional->type = NodeType::CONDITIONAL;
                conditional->value = conditional->token.value;
                auto right = parseExpression(tokens, pos);
                conditional->addChild(std::move(node));
                conditional->addChild(std::move(right));
                node = std::move(conditional);
            }
        return node;
    }
    std::unique_ptr<Node> parseIdentifier(const std::vector<lexer::Token>& tokens, int& pos){
        lexer::Token token = tokens[pos++];
        return parseExpression(tokens, pos);
    }
    std::unique_ptr<Node> parsePrint(const std::vector<lexer::Token>& tokens, int& pos){
        std::unique_ptr<Node> node = parseExpression(tokens, pos);
        return node;
    }

    std::unique_ptr<Node> parseExpression(const std::vector<lexer::Token>& tokens, int& pos){
        auto node = parseTerm(tokens, pos);
        while(pos < tokens.size() && (tokens[pos].type == lexer::TokenType::PLUS || tokens[pos].type == lexer::TokenType::SUBTRACT)){
            lexer::Token token = tokens[pos++];
            auto op = std::make_unique<Node>();
            op->token = token;
            op->type = NodeType::OPERATOR;
            op->value = token.value;
            auto right = parseTerm(tokens, pos);

            op->addChild(std::move(node));
            op->addChild(std::move(right));
            node = std::move(op);
        }
        return node;
    }

    std::unique_ptr<Node> parseTerm(const std::vector<lexer::Token>& tokens, int& pos){
        auto node = parseFactor(tokens, pos);
        while(pos < tokens.size() && (tokens[pos].type == lexer::TokenType::MULTIPLY || tokens[pos].type == lexer::TokenType::DIVIDE)){
            lexer::Token token = tokens[pos++];
            auto op = std::make_unique<Node>();
            op->token = token;
            op->type = NodeType::OPERATOR;
            op->value = token.value;
            auto right = parseFactor(tokens, pos);

            op->addChild(std::move(node));
            op->addChild(std::move(right));
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