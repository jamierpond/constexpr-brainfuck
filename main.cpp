#include <iostream>
#include "constexpr_brainfuck.hpp"

int main() {
    std::cout << interpretBrainfuck(helloWorld) << std::endl;
    std::cout << interpretBrainfuck(fib) << std::endl;
    return 0;
}
