#include "Interpreter.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

using namespace std;

// Deconstructor
Interpreter::~Interpreter() {
    // Clean up any remaining scopes
    while (currentScope != &globalScope) {
        popScope();
    }
}

// Adds a new scope to the scope stack
void Interpreter::pushScope() {
    currentScope = new Scope(currentScope);
}

// Pop the current scope and its parent become the new current scope
void Interpreter::popScope() {
    if (currentScope != &globalScope) {
        Scope* parent = currentScope->getParent();
        delete currentScope;
        currentScope = parent;
    }
}

int Interpreter::evaluate(const unique_ptr<Node>& node) {
    if (!node) {
        cerr << "ERROR: Attempted to evaluate null node" << endl;
        return 0;
    }
    switch (node->type) {
        case NodeType::PROGRAM: {
            for (int i = 0; i < node->children.size(); i++) {
                evaluate(node->children[i]);
            }
            return 0;
        }

        case NodeType::FUNC_CALL:
            return evaluateFunctionCall(node);

        case NodeType::DEF: {
            string funcName = node->value;
            functionTable[funcName] = unique_ptr<Node>(node->clone());
            return 0;
        }

        case NodeType::RETURN:
            if (!node->children.empty()) {
                return evaluate(node->children[0]);
            }
            return 0;

        case NodeType::BLOCK: {
            pushScope();
            int result = 0;

            // Evaluate all statements in the block
            for (const auto& child : node->children) {
                result = evaluate(child);
                if (child->type == NodeType::RETURN) {
                    popScope();
                    return result;
                }
            }

            popScope();
            return result;
        }

        case NodeType::IF: {
            if (node->children.size() < 2) {
                cerr << "ERROR: Malformed IF node at line " << node->token.lineNumber << endl;
                return 0;
            }
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
            try {
                return stoi(node->value);
            } catch (const std::invalid_argument&) {
                cerr << "ERROR: Invalid number format '" << node->value << "' at line " << node->token.lineNumber
                     << endl;
                return 0;
            } catch (const std::out_of_range&) {
                cerr << "ERROR: Number out of range '" << node->value << "' at line " << node->token.lineNumber << endl;
                return 0;
            }

        case NodeType::VARIABLE: {
            auto varName = node->value;
            auto var = this->currentScope->lookup(varName);
            if (var.first) {
                return var.second;
            }
            cerr << "ERROR: Variable '" << node->value << "' not found at line " << node->token.lineNumber << endl;
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
                if (right == 0) {
                    cerr << "ERROR: Division by zero at line " << node->token.lineNumber << endl;
                    return 0;
                }
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
            this->currentScope->update(varName, value);
            return value;
        }

        default:
            cerr << "ERROR: Unknown node type (" << static_cast<int>(node->type) << ") at line "
                 << node->token.lineNumber << endl;
            return 0;
    }
    return 0;
}

int Interpreter::evaluateFunctionCall(const unique_ptr<Node>& funcNode) {
    auto functionInterpreter = std::make_unique<Interpreter>();

    // Check if function exists before accessing it
    if (functionTable.find(funcNode->value) == functionTable.end()) {
        cerr << "ERROR: Function '" << funcNode->value << "' not defined at line " << funcNode->token.lineNumber
             << endl;
        return 0;
    }
    auto functionDef = functionTable[funcNode->value]->clone();

    // Get parameter names
    vector<string> paramNames;
    for (int i = 0; i < functionDef->children.size() - 1; i++) {
        paramNames.push_back(functionDef->children[i]->value);
    }

    // Evaluate arguments from the call
    vector<int> argValues;
    for (const auto& argNode : funcNode->children) {
        argValues.push_back(this->evaluate(argNode));
    }

    // Check parameter count
    if (argValues.size() != paramNames.size()) {
        cerr << "ERROR: Function '" << funcNode->value << "' called with wrong number of arguments at line "
             << funcNode->token.lineNumber << endl;
        return 0;
    }

    // Bind arguments to parameters and add to function interpreters global scope
    for (int i = 0; i < paramNames.size(); i++) {
        functionInterpreter->globalScope.update(paramNames[i], argValues[i]);
    }

    // Execute function body
    if (functionDef->children.size() > paramNames.size()) {
        return functionInterpreter->evaluate(functionDef->children.back());
    }

    return 0;
}
