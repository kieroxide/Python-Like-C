#pragma once
#include <unordered_map>

#include "../parser/parser.hpp"
#include "../scope/Scope.hpp"

class Interpreter {
    Scope globalScope;
    Scope* currentScope;

   private:
    void pushScope();
    void popScope();

   public:
    Interpreter() : currentScope(&globalScope) {};
    ~Interpreter();
    int evaluate(const std::unique_ptr<Node>& node);
};