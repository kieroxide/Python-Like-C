#include "src/executor/executor.hpp"

#include "src/interpreter/interpreter.hpp"
#include "src/lexer/lexer.hpp"
#include "src/parser/parser.hpp"
#include "src/utility/utility.hpp"

using namespace std;

namespace executor {

int executeFile(std::string filePath) {
    string fileContents = utility::readFile(filePath);
    string fileContentsFormatted = utility::convertTabs(fileContents);

    auto lexer = make_unique<Lexer>();
    auto tokens = lexer->tokenize(fileContentsFormatted);

    auto parser = make_unique<Parser>();
    auto ast = parser->parseProgram(tokens);

    auto interpreter = make_unique<Interpreter>();
    int result = interpreter->evaluate(ast);

    return result;
}
}  // namespace executor
