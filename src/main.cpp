#include <iostream>

#include "src/executor/executor.hpp"
#include "src/scope/value.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    string filePath = "tests/test_arith.txt";
    if (argc > 1) {
        filePath = argv[1];
    }

    Value response = executor::executeFile(filePath);
    if(response.isArray()){
       cout << "Script returned array of size " << response.asArray().size();
    }
    cout << "Script returned " << response.asInt();
    return response.asInt();
}