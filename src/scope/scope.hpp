#pragma once
#include <string>
#include <unordered_map>

#include "src/scope/value.hpp"

class Scope {
   private:
    std::unordered_map<std::string, Value> variables;
    Scope* parent;

   public:
    Scope(Scope* p = nullptr) : parent(p) {};

    Scope* getParent();

    void updateArr(const std::string& variableName, const int index, const Value& value);
    void update(const std::string& variableName, const Value value);
    std::pair<bool, Value> lookup(const std::string& name);
};