#include "interpreter.hpp"
#include <iostream>
#include <string>
#include <unordered_map>
#include <memory> 

int Interpreter::evaluate(const std::unique_ptr<parser::Node>& node) {
    //std::cout << "\nCurrentNODE VALUE: " << node->value << "\n";
    switch(node->type) {
        case parser::NodeType::BLOCK:
        case parser::NodeType::PROGRAM:{
            for(int i = 0; i < node->children.size(); i++){
                //printf("\nChild: %d \n", i+1);
                //printf("Children SIZE: %d \n \n", node->children.size());
                evaluate(node->children[i]);
            }
            return 0;
        }
        case parser::NodeType::IF:{
            int condition = evaluate(node->children[0]);
            if(condition != 0){
                evaluate(node->children[1]);
            }
            return 0;
        }
        case parser::NodeType::CONDITIONAL:{
            int left = evaluate(node->children[0]);
            int right = evaluate(node->children[1]);
            if(node->value == "=="){return left == right;}
            if(node->value == "<"){return left < right;}
            if(node->value == ">"){return left > right;}
            return 0;
        }
        case parser::NodeType::NUMBER:
            return std::stoi(node->value);
        case parser::NodeType::VARIABLE:{
            auto var = variables.find(node->value);
            if(var != variables.end()){
                return var->second;
            }
            std::cout << "VARIABLE NOT FOUND";
            return 0;
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
        default:
            std::cout << "ERROR INVALID TOKEN TYPE";
            return 0;
    }
    return 0;
}