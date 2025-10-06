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

    auto lexer = make_unique<Lexer>();
    auto tokens = lexer->tokenize(fileContentsFormatted);

    auto parser = make_unique<Parser>();
    auto ast = parser->parseProgram(tokens);

    auto interpreter = make_unique<Interpreter>();
    interpreter->evaluate(ast);

    return 0;
}
}  // namespace Executor
