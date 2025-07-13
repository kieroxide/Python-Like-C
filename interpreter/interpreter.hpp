#pragma once
#include "../parser/parser.hpp"
#include <unordered_map>

class Interpreter {
    std::unordered_map<std::string, int> variables;

public:
    int evaluate(const std::unique_ptr<parser::Node>& node);
};