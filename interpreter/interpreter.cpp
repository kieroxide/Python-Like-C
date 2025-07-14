#include "interpreter.hpp"
#include <iostream>
#include <string>
#include <unordered_map>
#include <memory> 

int Interpreter::evaluate(const std::unique_ptr<parser::Node>& node) {
    switch(node->type) {
        case parser::NodeType::NUMBER:
            return std::stoi(node->value);
        case parser::NodeType::VARIABLE:{
            auto var = variables.find(node->value);
            if(var != variables.end()){
                return var->second;
            }
        }
        case parser::NodeType::OPERATOR: {
            int left = evaluate(node->children[0]);
            int right = evaluate(node->children[1]);
            if(node->value == "+"){return left + right;}
            if(node->value == "-"){return left - right;}
            if(node->value == "*"){return left * right;}
            if(node->value == "/"){return left / right;}
            else{ return 0;} 
        }
        case parser::NodeType::PRINT: {
            std::string result = std::to_string(evaluate(node->children[0]));
            std::cout << result << std::endl;
            return 0;
        }
        case parser::NodeType::ASSIGN: {
            std::string varName = node->token.value;
            int value = evaluate(node->children[0]);
            variables[varName] = value;
            return value;
        }
    }
    return 0;
}