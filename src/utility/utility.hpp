#pragma once
#include <string>
#include <vector>

namespace utility {
std::string readFile(const std::string& filename);
std::vector<std::string> splitByNewline(const std::string& input);
bool isIndent(const std::string& line);
std::string getBlock(std::vector<std::string>& lines, int& i, int& lineNumber);
std::string convertTabs(const std::string& str);
}  // namespace utility