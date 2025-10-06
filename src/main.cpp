#include <iostream>

#include "src\Executor\executor.hpp"


using namespace std;

int main(int argc, char* argv[]) {
    string filePath = "tests/test_arith.txt";
    if (argc > 1) {
        filePath = argv[1];
    }

    int response = executor::executeFile(filePath);
    if (response != 0) {
        cerr << "Test exited with code " << response << "\n";
    }
}