#pragma once
#include <unordered_map>

#include "../parser/parser.hpp"
#include "../scope/scope.hpp"

class Interpreter {
   public:
    Scope globalScope;
    Scope* currentScope;

    std::unordered_map<std::string, std::unique_ptr<Node>> functionTable;

   private:
    void pushScope();
    void popScope();

   public:
    Interpreter() : currentScope(&globalScope) {};
    ~Interpreter();
    Value evaluate(const std::unique_ptr<Node>& node);
    Value evaluateFunctionCall(const std::unique_ptr<Node>& node);
};
