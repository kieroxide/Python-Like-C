#include "interpreter.hpp"
#include <iostream>
#include <string>
#include <unordered_map>
#include <memory> 

int Interpreter::evaluate(const std::unique_ptr<parser::Node>& node) {
    switch(node->type) {
        case parser::NodeType::NUMBER:
            return std::stoi(node->value);
        case parser::NodeType::VARIABLE:
            return stoi(node->value);
        case parser::NodeType::OPERATOR: {
            int left = evaluate(node->children[0]);
            int right = evaluate(node->children[1]);
            if(node->value == "+"){return left + right;}
            else{ return 0;} 
        }
        case parser::NodeType::PRINT_STATEMENT: {
            std::string result = std::to_string(evaluate(node->children[0]));
            std::cout << result << std::endl;
            return 0;
        }
    }
    return 0;
}