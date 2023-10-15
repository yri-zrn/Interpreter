#include <iostream>

#include "Interpreter.hpp"

using namespace Interpreter;

int main(int, char**) {
    const char* expression = "( 1.0509 + 2,1567 ) * ( 3,456 / ( 4.33443 - 5,3996 ) ) ";

    std::cout << "Expression:\n" << expression << "\n\n";
    std::cout << "Result: " << Interpretate(expression) << "\n\n";

    return 0;
}