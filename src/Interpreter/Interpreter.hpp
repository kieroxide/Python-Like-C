#pragma once
#include <unordered_map>

#include "../Parser/Parser.hpp"
#include "../Scope/Scope.hpp"

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
    int evaluate(const std::unique_ptr<Node>& node);
    int evaluateFunctionCall(const std::unique_ptr<Node>& node);
};
