// algorithm.cpp
#include "algorithms.hpp"

std::vector<Instruction> createFactorialProgram(uint32_t input_addr, uint32_t const_one_addr, uint32_t result_addr) {
    return {
        {Opcode::LOAD, 0, const_one_addr, 0},   // R0 = 1 (result)
        {Opcode::LOAD, 1, input_addr, 0},       // R1 = n
        {Opcode::LOAD, 2, const_one_addr, 0},   // R2 = 1

        // loop_start
        {Opcode::CMP, 0, 1, 2},                 // if R1 == 1
        {Opcode::JMP, 8, 1, 0},                 // if ZF == 1 → skip MUL/SUB

        {Opcode::MUL, 0, 0, 1},                 // result *= i
        {Opcode::SUB, 1, 1, 2},                 // i--
        {Opcode::JMP, 3, 0, 0},                 // jump to loop_start

        // loop_end
        {Opcode::STORE, result_addr, 0, 0},     // RAM[result_addr] = result
        {Opcode::HALT, 0, 0, 0}
    };
}


std::vector<Instruction> createSumListProgram(uint32_t array_addr, uint32_t length_addr, uint32_t result_addr, uint32_t const_one_addr) {
    return {
        {Opcode::LOAD, 0, array_addr, 0},      // R0 = array address
        {Opcode::LOAD, 1, length_addr, 0},      // counter = length
        {Opcode::LOAD, 2, result_addr, 0},       // R3 = result pointer
        {Opcode::LOAD, 3, const_one_addr, 0},   // R5 = 1 (for pointer++ and counter--)

        // loop_start @ pc = 4
        {Opcode::CMP, 0, 1, 3},                 // if counter == 1
        {Opcode::JMP, 11, 1, 0},                // if ZF, jump to end

        {Opcode::LOAD, 4, 0, 0},                 // R4 = RAM[R0]
        {Opcode::ADD, 2, 2, 4},                 // sum += R4
        {Opcode::ADD, 0, 0, 3},                 // array_ptr++
        {Opcode::SUB, 1, 1, 3},                 // counter--

        {Opcode::JMP, 4, 0, 0},                 // loop

        {Opcode::STORE, result_addr, 2, 0},     // store sum
        {Opcode::HALT, 0, 0, 0}
    };
}