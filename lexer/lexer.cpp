#include "lexer.hpp"

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

namespace lexer{
    void printTokens(std::vector<Token> tokens){
        for(Token t : tokens){
            std::cout << "\nLine Number: " << t.line_number << ", " 
            << "Type: "<< static_cast<int>(t.type) << ", " 
            << "Value: " << t.value ;
        }
    }

    std::vector<Token> tokenize(const std::string& code){
        std::vector<Token> tokens;
        int len = code.size();
        int line_number = 1;

        for(int i = 0; i < len; i++){
            char c = code[i];
            if(c == '\n'){
                line_number++;
                continue;
            }
            else if(isalpha(c)){
                Token token = tokenizeAlpha(c, i, code);
                token.line_number = line_number;
                tokens.push_back(token);
            }
            else if(isdigit(c)){
                Token token = tokenizeDigit(c, i, code);
                token.line_number = line_number;
                tokens.push_back(token);
            }
            else if(c == '='){
                std::string str = "=";
                Token token = {TokenType::ASSIGN, str, line_number};
                tokens.push_back(token);
            }
            
        }
        return tokens;
    }

    Token tokenizeAlpha(char c, int& i, const std::string& code){
        std::string str;
        str.push_back(c);
        do{
            if(i == code.size()){
                break;
            }
            i++;
            c = code[i];
            str.push_back(c);   
        }
        while(i < code.size() && isalpha(c));
        i--;
        Token token;
        if(str == "print"){
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
        do{
            if(i == code.size()){
                break;
            }
            i++;
            c = code[i];
            str.push_back(c);   
        }
        while(i < code.size() && isdigit(c));
        i--;
        
        Token token;
        token.type = TokenType::NUMBER;
        token.value = str;
        return token;
    }
}

