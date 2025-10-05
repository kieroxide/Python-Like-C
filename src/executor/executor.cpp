#include "executor.hpp"

#include "src/interpreter/interpreter.hpp"
#include "src/lexer/lexer.hpp"
#include "src/parser/parser.hpp"
#include "src/utility/utility.hpp"

using namespace std;

namespace Executor {

int executeFile(std::string filePath) {
    std::string fileContents = utility::readFile(filePath);
    auto lines = utility::splitByNewline(fileContents);

    auto* interpreter = new Interpreter();

    int lineNumber = 1;

    auto main = std::make_unique<parser::Node>();
    main->type = parser::NodeType::PROGRAM;
    main->value = "MAIN";

    // Loops through all lines and adds to main AST tree for interpreting
    for (int i = 0; i < lines.size(); i++) {
        auto block = utility::getBlock(lines, i, lineNumber);

        vector<lexer::Token> tokens = lexer::tokenize(block, lineNumber);
        unique_ptr<parser::Node> statement = parser::parseStatement(tokens);
        main->addChild(std::move(statement));

        lineNumber++;
    }

    interpreter->evaluate(main);
    
    delete interpreter;
    return 0;
}
}  // namespace Executor
