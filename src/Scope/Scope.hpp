#pragma once
#include <string>
#include <unordered_map>

class Scope {
   private:
    std::unordered_map<std::string, int> variables;
    Scope* parent;

   public:
    Scope(Scope* p = nullptr) : parent(p) {};

    Scope* getParent();

    void update(const std::string& variableName, const int value);
    std::pair<bool, int> lookup(const std::string& name);
};