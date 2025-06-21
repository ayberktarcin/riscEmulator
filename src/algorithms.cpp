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
        {Opcode::LOAD, 4, 0, 1},              // R4 = RAM[R0]
        {Opcode::ADD, 2, 2, 4},               // sum += R4
        {Opcode::ADD, 0, 0, 3},               // array_ptr++

        {Opcode::CMP, 0, 1, 3},               // if counter == 1
        {Opcode::JMP, 11, 1, 0},              // if ZF, jump to end

        {Opcode::SUB, 1, 1, 3},               // counter--
        {Opcode::JMP, 4, 0, 0},               // loop

        {Opcode::STORE, result_addr, 2, 0},   // store sum
        {Opcode::HALT, 0, 0, 0}
    };
}


std::vector<Instruction> createFibonacciProgram(uint32_t input_addr, uint32_t result_addr) {
    return {
        {Opcode::LOAD, 0, input_addr, 0},       // R0 = n
        {Opcode::LOAD, 1, 1, 2},                // R1 = 1 (constant 1)
        {Opcode::LOAD, 2, 0, 2},                // R2 = 0 (fib_prev)
        {Opcode::LOAD, 3, 1, 2},                // R3 = 1 (fib_curr)

        // if n == 0 → result = 0
        {Opcode::CMP, 0, 2, 0},                 // if R0 == R2 (0)
        {Opcode::JMP, 18, 1, 0},                // jump to store if ZF

        // if n == 1 → result = 1
        {Opcode::CMP, 0, 1, 0},                 // if R0 == R1 (1)
        {Opcode::JMP, 16, 1, 0},                // jump to store if ZF

        // Loop setup
        {Opcode::LOAD, 4, 1, 2},                // R4 = R1 = R1
 
        // loop_start
        {Opcode::CMP, 0, 4, 0},                 // if i == n
        {Opcode::JMP, 16, 1, 0},                // exit if ZF

        {Opcode::ADD, 5, 2, 3},                 // R5 = fib_prev + fib_curr
        {Opcode::MOV, 2, 3, 0},                 // fib_prev = fib_curr
        {Opcode::MOV, 3, 5, 0},                 // fib_curr = next
        
        {Opcode::ADD, 4, 4, 1},                 // i++
        {Opcode::JMP, 9, 0, 0},                 // jump to loop_start

        // store result
        {Opcode::STORE, result_addr, 3, 0},
        {Opcode::HALT, 0, 0, 0},

        // if n = 0 store result coming from control flow
        {Opcode::MOV, 3, 2, 0},                 // R3 = R2 = 0  (fib_curr)
        {Opcode::JMP, 16, 1, 0},                // exit if ZF
        {Opcode::HALT, 0, 0, 0},                
    };
}