#pragma once
#include <string>
#include <vector>

namespace utility {
std::string readFile(const std::string& filename);
std::vector<std::string> splitByNewline(const std::string& input);
bool isIndent(const std::string& line);
std::string getBlock(std::vector<std::string>& lines, int& i, int& lineNumber);
int run(std::string filename);
}  // namespace utility