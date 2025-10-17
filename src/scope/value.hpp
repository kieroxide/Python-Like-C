#pragma once
#include <variant>
#include <vector>

struct Value;
using Array = std::vector<Value>;

struct Value {
    std::variant<int, Array, bool> v;
    Value() : v(0) {}
    Value(int i) : v(i) {}
    Value(bool b) : v(b) {}
    Value(const Array& a) : v(a) {}

    bool isInt() const { return std::holds_alternative<int>(v); }
    bool isBool() const { return std::holds_alternative<bool>(v); }
    bool isArray() const { return std::holds_alternative<Array>(v); }
    int asInt() const { return std::get<int>(v); }
    bool asBool() const { return std::get<bool>(v); }
    Array& asArray() { return std::get<Array>(v); }
    const Array& asArray() const { return std::get<Array>(v); }
};