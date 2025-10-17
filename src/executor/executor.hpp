#pragma once
#include <string>
#include "src/scope/value.hpp"

namespace executor {
Value executeFile(std::string filePath);
}