#pragma once
#include <variant>
#include <vector>

struct Value;
using Array = std::vector<Value>;

struct Value {
    std::variant<int, Array> v;
    Value() : v(0) {}
    Value(int i) : v(i) {}
    Value(const Array& a) : v(a) {}

    bool isInt() const { return std::holds_alternative<int>(v); }
    bool isArray() const { return std::holds_alternative<Array>(v); }
    int asInt() const { return std::get<int>(v); }
    Array& asArray() { return std::get<Array>(v); }
    const Array& asArray() const { return std::get<Array>(v); }
};