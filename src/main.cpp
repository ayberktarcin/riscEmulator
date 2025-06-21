// src/main.cpp
#include "machine.hpp"
#include <iostream>
#include <vector>
#include <cstdint>
#include "instruction.hpp"
#include "algorithms.hpp"

int main() {
    RiscMachine machine(512 /*program*/, 512 /*data*/);

    machine.setMemoryValue(100, 0);     // n = 4
    machine.setMemoryValue(101, 0);     // result addr

    // Define sum list program
    std::vector<Instruction> program = createFibonacciProgram(100, 101);

    // Load initial memory values
    // We'll use instruction memory for both program and data
    machine.loadProgram(program);

    // Run the program
    machine.run();

    // Display result
    std::cout << "Fibonnacci Num = "
              << machine.getMemoryValue(101) // Read from result_addr
              << std::endl;

    // Reset the machine for future use
    machine.reset();
    std::cout << "Machine reset. Ready for next operation." << std::endl;

    return 0;
}
