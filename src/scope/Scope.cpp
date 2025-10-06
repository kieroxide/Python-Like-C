#include "Scope.hpp"

/**
 * Returns parent of scope
 */
Scope* Scope::getParent() {
    return parent;
}

/**
 *  Updates variable in the scope. if not found adds new variable to map
 */
void Scope::update(const std::string& variableName, const int value) {
    // Check if variable exists in this scope (direct access)
    auto it = variables.find(variableName);
    if (it != variables.end()) {
        // Variable exists in current scope, update it
        it->second = value;
    } else if (parent) {
        // Check if variable exists in any parent scope
        std::pair<bool, int> result = parent->lookup(variableName);
        bool found = result.first;
        
        if (found) {
            // Variable exists in parent, update there instead of shadowing
            parent->update(variableName, value);
        } else {
            // Variable doesn't exist anywhere, create in current scope
            variables[variableName] = value;
        }
    } else {
        // No parent and not in current scope, create it here
        variables[variableName] = value;
    }
}

/**
 *  Looks up a variable in this scope or parents
 */
std::pair<bool, int> Scope::lookup(const std::string& name) {
    auto it = variables.find(name);
    if (it != variables.end()) {
        return {true, it->second};
    }
    if (parent) {
        return parent->lookup(name);
    }
    return {false, 0};
}