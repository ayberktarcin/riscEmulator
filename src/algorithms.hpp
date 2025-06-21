// algorithm.hpp
#pragma once

#include <vector>
#include "instruction.hpp"

// Returns a vector of instructions to compute factorial of n
std::vector<Instruction> createFactorialProgram(uint32_t input_addr, uint32_t result_addr);
std::vector<Instruction> createSumListProgram(uint32_t array_addr, uint32_t length_addr, uint32_t result_addr);
std::vector<Instruction> createFibonacciProgram(uint32_t input_addr, uint32_t result_addr);