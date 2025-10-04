#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>

int run(std::string filename);

int main() {
    std::vector<std::string> tests = {
        "tests/test_simple_assign.txt",
        "tests/test_arith.txt",
        "tests/test_conditionals.txt",
        "tests/test_nested.txt"
    };

    for (auto const& t : tests) {
    std::cout << "=== Running " << t << " ===\n";
    int rc = run(t);
    if (rc != 0) std::cerr << "Test exited with code " << rc << "\n";
        std::cout << '\n';
    }
    return 0;
}