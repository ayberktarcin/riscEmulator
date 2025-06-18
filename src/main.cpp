// src/main.cpp
#include "machine.hpp"
#include <iostream>
#include <vector>
#include <cstdint>
#include "instruction.hpp"
#include "algorithms.hpp"

int main() {
    RiscMachine machine(256 /*program*/, 256 /*data*/);

    // Example array: [10, 20, 30] at RAM[200, 201, 202]
    machine.setMemoryValue(200, 3);    // length
    machine.setMemoryValue(201, 10);
    machine.setMemoryValue(202, 20);
    machine.setMemoryValue(203, 30);


    // Define sum list program
    std::vector<Instruction> program = createSumListProgram(200, 300, 350);

    // Load initial memory values
    // We'll use instruction memory for both program and data
    machine.loadProgram(program);

    // Run the program
    machine.run();

    // Display result
    std::cout << "Sum of list = "
              << machine.getMemoryValue(350)
              << std::endl;

    // Reset the machine for future use
    machine.reset();
    std::cout << "Machine reset. Ready for next operation." << std::endl;

    return 0;
}
