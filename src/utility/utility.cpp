#include "Utility.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

namespace utility {
std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::vector<std::string> splitByNewline(const std::string& input) {
    std::vector<std::string> lines;
    std::stringstream ss(input);
    std::string line;

    while (std::getline(ss, line)) {
        if (!line.empty()) {
            lines.push_back(line);
        }
    }

    return lines;
}

bool isIndent(const std::string& line) {
    if (line.size() < 4)
        return false;

    for (int i = 0; i < 4; i++) {
        if (line[i] != ' ') {
            return false;
        }
    }
    return true;
}

std::string getBlock(std::vector<std::string>& lines, int& i, int& lineNumber) {
    std::string block = lines[i];
    while (i + 1 < lines.size() && isIndent(lines[i + 1])) {
        block += "\n" + lines[i + 1];
        i++;
    }
    return block;
}

std::string convertTabs(const std::string& str) {
    const int indentSize = 4;
    std::string formatted = "";
    for (const char c : str) {
        if (c == '\t') {
            for (int i = 0; i < indentSize; i++) {
                formatted += " ";
            }
        } else {
            formatted += c;
        }
    }
    
    return formatted;
}
}  // namespace utility
