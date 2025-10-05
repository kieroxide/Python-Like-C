#include "executor.hpp"

#include "src/interpreter/interpreter.hpp"
#include "src/lexer/lexer.hpp"
#include "src/parser/parser.hpp"
#include "src/utility/utility.hpp"

using namespace std;

namespace Executor {

int executeFile(std::string filePath) {
    string fileContents = utility::readFile(filePath);
    string fileContentsFormatted = utility::convertTabs(fileContents);

    auto lexer = new Lexer();
    auto tokens = lexer->tokenize(fileContentsFormatted);

    auto parser = new Parser();
    auto ast = parser->parseProgram(tokens);

    auto interpreter = new Interpreter();
    interpreter->evaluate(ast);

    return 0;
}
}  // namespace Executor
