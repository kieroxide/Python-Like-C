#include "src/interpreter/interpreter.hpp"

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
void Interpreter::pushScope() { currentScope = new Scope(currentScope); }

// Pop the current scope and its parent become the new current scope
void Interpreter::popScope() {
    if (currentScope != &globalScope) {
        Scope* parent = currentScope->getParent();
        delete currentScope;
        currentScope = parent;
    }
}

Value Interpreter::evaluate(const unique_ptr<Node>& node) {
    if (!node) {
        cerr << "ERROR: Attempted to evaluate null node" << endl;
        return 0;
    }
    switch (node->type) {
        case NodeType::PROGRAM: {
            for (int i = 0; i < node->children.size(); i++) {
                Value val = evaluate(node->children[i]);
                if (node->children[i]->type == NodeType::RETURN) {
                    return val;
                }
            }
            return 0;
        }

        case NodeType::WHILE: {
            // Node will contain a conditional and a block
            const auto& conditional = node->children[0];
            const auto& block = node->children[1];
            Value last = 0;
            
            
            while (evaluate(conditional).asInt() == 1) {
                last = evaluate(block);
            }
            return last;
        }

        case NodeType::FUNC_CALL: {
            return evaluateFunctionCall(node);
        }

        case NodeType::DEF: {
            string funcName = node->value;
            functionTable[funcName] = unique_ptr<Node>(node->clone());
            return 0;
        }

        case NodeType::RETURN: {
            if (!node->children.empty()) {
                return evaluate(node->children[0]);
            }
            return 0;
        }

        case NodeType::BLOCK: {
            pushScope();
            Value result = 0;

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
            bool condition = evaluate(node->children[0]).asInt();
            if (condition == 1) {
                evaluate(node->children[1]);
            }
            return 0;
        }

        case NodeType::CONDITIONAL: {
            const Value& left = evaluate(node->children[0]);
            const Value& right = evaluate(node->children[1]);

            if (!(left.isInt() && right.isInt())) {
                cerr << "ERROR: Invalid Comparison of Array '" << node->value << "' at line " << node->token.lineNumber
                     << endl;
                return 0;
            }

            const int leftInt = left.asInt();
            const int rightInt = right.asInt();

            if (node->value == "==") {
                return leftInt == rightInt;
            }
            if (node->value == "<") {
                return leftInt < rightInt;
            }
            if (node->value == ">") {
                return leftInt > rightInt;
            }
            return 0;
        }

        case NodeType::NUMBER: {
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
        }

        case NodeType::ARRAY: {
            const int size = node->children.size();
            vector<Value> arr;
            arr.reserve(size);
            for (const auto& child : node->children) {
                arr.push_back(evaluate(child));
            }
            return Value(arr);
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
            const Value& leftValue = evaluate(node->children[0]);
            const Value& rightValue = evaluate(node->children[1]);

            if (!(leftValue.isInt() && rightValue.isInt())) {
                cerr << "ERROR: Invalid Operation of Array '" << node->value << "' at line " << node->token.lineNumber
                     << endl;
                return 0;
            }
            int left = leftValue.asInt();
            int right = rightValue.asInt();

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
            const Value& eval = evaluate(node->children[0]);
            if (eval.isArray()) {
                const auto& arr = eval.asArray();
                cout << "[";
                int lastIndex = arr.size() - 1;
                for (int i = 0; i < arr.size(); i++) {
                    cout << arr[i].asInt();
                    if (!(i == lastIndex)) {
                        cout << ",";
                    }
                }
                cout << "]" << endl;
                return 0;
            }
            string result = to_string(eval.asInt());
            cout << result << endl;
            return 0;
        }

        case NodeType::ASSIGN: {
            // First, determine if this is a regular variable assignment or array index assignment
            auto& target = node->children[0];
            const Value value = evaluate(node->children[1]);

            if (target->type == NodeType::VARIABLE) {
                // Regular variable assignment
                string varName = target->value;
                this->currentScope->update(varName, value);
                return value;
            } else if (target->type == NodeType::INDEX) {
                // Array index assignment
                auto& baseNode = target->children[0];  // Should be a VARIABLE node
                if (baseNode->type != NodeType::VARIABLE) {
                    cerr << "ERROR: Cannot assign to non-variable expression" << endl;
                    return 0;
                }

                string arrayName = baseNode->value;
                auto lookupResult = this->currentScope->lookup(arrayName);

                if (!lookupResult.first) {
                    cerr << "ERROR: Variable '" << arrayName << "' not found at line " << node->token.lineNumber
                         << endl;
                    return 0;
                }

                Value arrayValue = lookupResult.second;
                if (!arrayValue.isArray()) {
                    cerr << "ERROR: '" << arrayName << "' is not an array at line " << node->token.lineNumber << endl;
                    return 0;
                }

                Value indexValue = evaluate(target->children[1]);
                if (!indexValue.isInt()) {
                    cerr << "ERROR: Array index must be an integer at line " << node->token.lineNumber << endl;
                    return 0;
                }

                int index = indexValue.asInt();
                auto& array = arrayValue.asArray();

                if (index < 0 || index >= array.size()) {
                    cerr << "ERROR: Array index out of bounds at line " << node->token.lineNumber << endl;
                    return 0;
                }

                array[index] = value;
                this->currentScope->update(arrayName, arrayValue);  // Write back the modified array
                return value;
            } else {
                cerr << "ERROR: Invalid assignment target at line " << node->token.lineNumber << endl;
                return 0;
            }
        }

        case NodeType::INDEX: {
            const auto& variable = evaluate(node->children[0]);  // Evaluates variable
            const auto& index = evaluate(node->children[1]);  // Evaluates index value
            return variable.asArray()[index.asInt()];
        }

        default:
            cerr << "ERROR: Unknown node type (" << static_cast<int>(node->type) << ") at line "
                 << node->token.lineNumber << endl;
            return 0;
    }
    return 0;
}

Value Interpreter::evaluateFunctionCall(const unique_ptr<Node>& funcNode) {
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
        argValues.push_back(this->evaluate(argNode).asInt());
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