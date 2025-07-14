#include "lexer.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

namespace lexer{
    std::vector<std::string> splitByNewline(const std::string& input) {
        std::vector<std::string> lines;
        std::stringstream ss(input);
        std::string line;

        while (std::getline(ss, line)) {
            lines.push_back(line);
        }

        return lines;
    }

    void printTokens(std::vector<Token> tokens){
        for(Token t : tokens){
            std::cout << "Line Number: " << t.lineNumber << ", " 
            << "Type: "<< static_cast<int>(t.type) << ", " 
            << "Value: " << t.value << "\n";
        }
    }

    std::vector<Token> tokenize(const std::string& code, int& lineNumber){
        std::vector<Token> tokens;
        Token token = {TokenType::BLOCK, "BLOCK", lineNumber};
        tokens.push_back(token);

        const auto& lines = splitByNewline(code);
        for(const auto& line : lines){
            auto stmtTokens = tokenize_statement(line, lineNumber);
            tokens.insert(tokens.end(), stmtTokens.begin(), stmtTokens.end());
        }
        return tokens;
    }
    std::vector<Token> tokenize_statement(const std::string& code, int& lineNumber){
        std::vector<Token> tokens;
        int len = code.size();
        for(int i = 0; i < len; i++){
            char c = code[i];
            if(c == ' '){
                int count = 1;
                while(i+1 < len && code[i+1] == ' '){
                    count++;
                    i++;
                }
                if(count == 4){
                    lineNumber++;
                    Token token = {TokenType::INDENT, "INDENT", lineNumber};
                    tokens.push_back(token);
                }
            }
            else if(isalpha(c)){
                Token token = tokenizeAlpha(c, i, code);
                token.lineNumber = lineNumber;
                tokens.push_back(token);
            }
            else if(isdigit(c)){
                Token token = tokenizeDigit(c, i, code);
                token.lineNumber = lineNumber;
                tokens.push_back(token);
            }
            else if(c == '='){
                std::string str = "=";
                Token token;
                if(code[i+1] == '='){
                    str += "=";
                    token = {TokenType::EQUALS, str, lineNumber};
                    i++;
                } else {
                    token = {TokenType::ASSIGN, str, lineNumber};
                }
                tokens.push_back(token);
            }
            else if(c == '+'){
                std::string str = "+";
                Token token = {TokenType::PLUS, str, lineNumber};
                tokens.push_back(token);
            }
            else if(c == '-'){
                std::string str = "-";
                Token token = {TokenType::SUBTRACT, str, lineNumber};
                tokens.push_back(token);
            }
            else if(c == '*'){
                std::string str = "*";
                Token token = {TokenType::MULTIPLY, str, lineNumber};
                tokens.push_back(token);
            }
            else if(c == '/'){
                std::string str = "/";
                Token token = {TokenType::DIVIDE, str, lineNumber};
                tokens.push_back(token);
            }
            else if(c == '<'){
                std::string str = "<";
                Token token = {TokenType::LESSTHAN, str, lineNumber};
                tokens.push_back(token);
            }
            else if(c == '>'){
                std::string str = ">";
                Token token = {TokenType::GREATERTHAN, str, lineNumber};
                tokens.push_back(token);
            }
        }
        return tokens;
    }
    Token tokenizeAlpha(char c, int& i, const std::string& code){
        std::string str;
        str.push_back(c);
        while(i+1 < code.size() && isalpha(code[i+1])){
            i++;
            c = code[i];
            str.push_back(c);   
        }
        Token token;
        if(str == "if"){
            token.type = TokenType::IF;
        }
        else if(str == "print"){
            token.type = TokenType::PRINT;
        } else {
            token.type = TokenType::IDENTIFIER;
        }
        token.value = str;
        return token;
    }
    Token tokenizeDigit(char c, int& i, const std::string& code){
        std::string str;
        str.push_back(c);
        while(i+1 < code.size() && isdigit(code[i+1])){
            i++;
            c = code[i];
            str.push_back(c);   
        }
        Token token;
        token.type = TokenType::NUMBER;
        token.value = str;
        return token;
    }
}

