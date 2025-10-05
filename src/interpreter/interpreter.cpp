#include "interpreter.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

using namespace std;

int Interpreter::evaluate(const unique_ptr<Node>& node) {
    // cout << "\nCurrentNODE VALUE: " << node->value << "\n";
    switch (node->type) {
        case NodeType::BLOCK:
        case NodeType::PROGRAM: {
            for (int i = 0; i < node->children.size(); i++) {
                // printf("\nChild: %d \n", i+1);
                // printf("Children SIZE: %d \n \n", node->children.size());
                evaluate(node->children[i]);
            }
            return 0;
        }
        case NodeType::IF: {
            int condition = evaluate(node->children[0]);
            if (condition != 0) {
                evaluate(node->children[1]);
            }
            return 0;
        }
        case NodeType::CONDITIONAL: {
            int left = evaluate(node->children[0]);
            int right = evaluate(node->children[1]);
            if (node->value == "==") {
                return left == right;
            }
            if (node->value == "<") {
                return left < right;
            }
            if (node->value == ">") {
                return left > right;
            }
            return 0;
        }
        case NodeType::NUMBER:
            return stoi(node->value);
        case NodeType::VARIABLE: {
            auto var = variables.find(node->value);
            if (var != variables.end()) {
                return var->second;
            }
            cout << "VARIABLE NOT FOUND";
            return 0;
        }
        case NodeType::OPERATOR: {
            int left = evaluate(node->children[0]);
            int right = evaluate(node->children[1]);
            if (node->value == "+") {
                return left + right;
            }
            if (node->value == "-") {
                return left - right;
            }
            if (node->value == "*") {
                return left * right;
            }
            if (node->value == "/") {
                return left / right;
            } else {
                return 0;
            }
        }
        case NodeType::PRINT: {
            string result = to_string(evaluate(node->children[0]));
            cout << result << endl;
            return 0;
        }
        case NodeType::ASSIGN: {
            string varName = node->token.value;
            int value = evaluate(node->children[0]);
            variables[varName] = value;
            return value;
        }
        default:
            cout << "ERROR INVALID TOKEN TYPE";
            return 0;
    }
    return 0;
}