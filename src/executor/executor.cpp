#include "Executor.hpp"

#include "src/Interpreter/Interpreter.hpp"
#include "src/Lexer/Lexer.hpp"
#include "src/Parser/Parser.hpp"
#include "src/Utility/Utility.hpp"

using namespace std;

namespace Executor {

int executeFile(std::string filePath) {
    string fileContents = utility::readFile(filePath);
    string fileContentsFormatted = utility::convertTabs(fileContents);

    auto lexer = make_unique<Lexer>();
    auto tokens = lexer->tokenize(fileContentsFormatted);

    auto parser = make_unique<Parser>();
    auto ast = parser->parseProgram(tokens);

    auto interpreter = make_unique<Interpreter>();
    interpreter->evaluate(ast);

    return 0;
}
}  // namespace Executor
