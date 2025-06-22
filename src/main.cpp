/**
 * @file main.cpp
 * @brief Entry point for the RISC Emulator demonstrating Fibonacci, Factorial, and List Sum programs.
 *
 * This file contains the main function, which initializes the RISC emulator,
 * loads various programs into memory, and executes them to demonstrate the
 * functionality of the emulator.
 */

#include "machine.hpp"
#include <iostream>
#include <vector>
#include <cstdint>
#include "instruction.hpp"
#include "algorithms.hpp"

int main() {
    RiscMachine machine(512 /*program*/, 512 /*data*/);

    // ──────────────── Fibonacci(n = 6) ────────────────
    std::cout << "🔷 Starting Fibonacci Program (n = 6)" << std::endl;

    machine.setMemoryValue(100, 8);  // input n
    machine.setMemoryValue(101, 0);  // result addr

    auto fib_program = createFibonacciProgram(100, 101);
    machine.loadProgram(fib_program);
    machine.run();

    std::cout << "✅ Fibonacci(6) = " << machine.getMemoryValue(101) << "\n\n";
    machine.reset();

    // ──────────────── Factorial(n = 5) ────────────────
    std::cout << "🔷 Starting Factorial Program (n = 5)" << std::endl;

    machine.setMemoryValue(200, 5);  // input n
    machine.setMemoryValue(201, 0);  // result addr

    auto fact_program = createFactorialProgram(200, 201);
    machine.loadProgram(fact_program);
    machine.run();

    std::cout << "✅ Factorial(5) = " << machine.getMemoryValue(201) << "\n\n";
    machine.reset();

    // ──────────────── List Sum [10, 20, 30, 40] ────────────────
    std::cout << "🔷 Starting List Sum Program [10, 20, 30, 40]" << std::endl;

    machine.setMemoryValue(300, 400); // pointer to array
    machine.setMemoryValue(301, 4);   // length
    machine.setMemoryValue(302, 0);   // result

    // array values
    machine.setMemoryValue(400, 10);
    machine.setMemoryValue(401, 20);
    machine.setMemoryValue(402, 30);
    machine.setMemoryValue(403, 40);

    auto sum_program = createSumListProgram(300, 301, 302);
    machine.loadProgram(sum_program);
    machine.run();

    std::cout << "✅ Sum of list = " << machine.getMemoryValue(302) << "\n\n";
    machine.reset();

    std::cout << "🎉 All programs completed successfully.\n";
    return 0;
}