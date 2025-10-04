#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include "lexer/lexer.hpp"
#include "parser/parser.hpp"
#include "interpreter/interpreter.hpp"

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
        if(!line.empty()){
            lines.push_back(line);
        }
    }

    return lines;
}

bool isIndent(const std::string& line){
    if(line.size() < 4) return false;
    
    for(int i = 0; i < 4; i++){
        if(line[i] != ' '){
            return false;
        }
    }
    return true;
}

std::string getBlock(std::vector<std::string>& lines, int& i, int& lineNumber ){
    std::string block = lines[i];
    while(i+1 < lines.size() && isIndent(lines[i+1])){
        block += "\n" + lines[i+1];
        i++;
    }
    return block;
}

int run(std::string filename){
    std::string fileContents = readFile(filename);
    auto lines = splitByNewline(fileContents);

    Interpreter* interpreter = new Interpreter(); 

    int lineNumber = 1;
    std::unique_ptr<parser::Node> main = std::make_unique<parser::Node>();
    main->type = parser::NodeType::PROGRAM;
    main->value = "MAIN";
    for(int i = 0; i < lines.size(); i++){  
        auto block = getBlock(lines, i, lineNumber);

        std::vector<lexer::Token> tokens = lexer::tokenize(block, lineNumber);
        auto program = parser::parseStatement(tokens);
        main->addChild(std::move(program));
        
        lineNumber++;
    }
    interpreter->evaluate(main);
    delete interpreter;
    return 0;
}

#ifndef RUN_TESTS
int main(int argc, char* argv[]){
    std::string filename = "tests/test_arith.txt";
    if(argc > 1){
        filename = argv[1];
    }
    run(filename);
}
#endif