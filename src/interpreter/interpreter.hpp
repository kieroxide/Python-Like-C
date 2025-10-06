#pragma once
#include <unordered_map>

#include "../Parser/Parser.hpp"
#include "../Scope/Scope.hpp"

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