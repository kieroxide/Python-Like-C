#include <iostream>
#include <string>
#include <vector>

#include "src/Executor/executor.hpp"

using namespace std;

struct TestCase {
    std::string filename;
    int expected;
};

int main() {
    string testsDir = "tests/";
    vector<TestCase> tests = {{"test_simple_assign.txt", 12}, {"test_arith.txt", 44},
                              {"test_conditionals.txt", 11},  {"test_nested.txt", 102},
                              {"test_functions.txt", 208},    {"test_scope.txt", 660}};

    bool allPassed = true;
    for (const auto& test : tests) {
        cout << "=== Running " << test.filename << " ===\n";
        int result = executor::executeFile(testsDir + test.filename);
        cout << "Returned: " << result << ", Expected: " << test.expected << "\n";
        if (result != test.expected) {
            cout << "Test FAILED!\n";
            allPassed = false;
        } else {
            cout << "Test passed.\n";
        }
        cout << endl;
    }
    if (allPassed) {
        cout << "All tests passed!\n";
    } else {
        cout << "Some tests failed.\n";
    }
    return allPassed ? 0 : 1;
}
