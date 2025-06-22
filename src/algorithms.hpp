/**
 * @file algorithms.hpp
 * @brief Declares functions to generate instruction sequences for various algorithms in the RISC emulator.
 */
#pragma once

#include <vector>
#include "instruction.hpp"

// Returns a vector of instructions to compute the factorial of a number.
// input_addr: Address where the input number is stored.
// result_addr: Address where the result will be stored.
std::vector<Instruction> createFactorialProgram(uint32_t input_addr, uint32_t result_addr);

// Returns a vector of instructions to compute the sum of a list of numbers.
// array_addr: Address of the array containing the numbers.
// length_addr: Address where the length of the array is stored.
// result_addr: Address where the result will be stored.
std::vector<Instruction> createSumListProgram(uint32_t array_addr, uint32_t length_addr, uint32_t result_addr);

// Returns a vector of instructions to compute the nth Fibonacci number.
// input_addr: Address where the input number (n) is stored.
// result_addr: Address where the result will be stored.
std::vector<Instruction> createFibonacciProgram(uint32_t input_addr, uint32_t result_addr);