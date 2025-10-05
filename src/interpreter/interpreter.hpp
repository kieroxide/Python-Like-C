#pragma once
#include <unordered_map>

#include "../parser/parser.hpp"

class Interpreter {
    std::unordered_map<std::string, int> variables;

   public:
    int evaluate(const std::unique_ptr<parser::Node>& node);
};