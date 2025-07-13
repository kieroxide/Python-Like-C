#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include "lexer/lexer.hpp"
#include "parser/parser.hpp"

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
        lines.push_back(line);
    }

    return lines;
}

int main(){
    std::string filename = "C:/Users/bailk/Desktop/Programs/Github-Projects/Python-Like/Python-Like-C/test.txt";
    std::string fileContents = readFile(filename);
    std::cout << fileContents; 

    auto lines = splitByNewline(fileContents);
    int lineNumber = 1;
    for(const auto& line: lines){
        std::vector<lexer::Token> tokens = lexer::tokenize(line, lineNumber);
        lexer::printTokens(tokens);
        auto program = parser::parse(tokens);
        parser::printAST(program);
        std::cout << "\n";
        lineNumber++;
    }
}