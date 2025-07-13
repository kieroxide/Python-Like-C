#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include "lexer/lexer.hpp"

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

int main(){
    std::string filename = "C:/Users/bailk/Desktop/Programs/Github-Projects/Python-Like/Python-Like-C/test.txt";
    std::string fileContents = readFile(filename);
    std::cout << fileContents; 

    std::vector<lexer::Token> tokens = lexer::tokenize("hello = 5\nprint x");
    lexer::printTokens(tokens);
}