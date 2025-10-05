#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "src/executor/executor.hpp"

using namespace std;

int main() {
    string testsDir = "tests/";
    vector<string> tests = {"test_simple_assign.txt", "test_arith.txt", "test_conditionals.txt", "test_nested.txt"};

    for (auto const& t : tests) {
        cout << "=== Running " << t << " ===\n";
        int response = Executor::executeFile(testsDir + t);
        if (response != 0)
            cerr << "Test exited with code " << response << "\n";
        cout << '\n' << flush;
    }
    return 0;
}