/**
 * @file algorithms.cpp
 * @brief Implements functions to generate instruction sequences for various algorithms in the RISC emulator.
 */
#include "algorithms.hpp"

/**
 * @brief Generates a program to compute the factorial of a number using a custom instruction set.
 * 
 * This function creates a sequence of instructions that calculates the factorial of a number
 * stored at a specific memory address (`input_addr`) and stores the result at another memory
 * address (`result_addr`). The program uses a loop to multiply numbers from the input value
 * down to 1, storing the intermediate results in a register.
 * 
 * @param input_addr The memory address where the input number (n) is stored.
 * @param result_addr The memory address where the computed factorial result will be stored.
 * @return A vector of instructions representing the factorial computation program.
 * 
 * Instructions:
 * - Initializes registers:
 *   - R0: Stores the result (initialized to 1).
 *   - R1: Stores the input value (n).
 *   - R2: Constant value 1.
 *   - R3: Constant value 0.
 * - Checks if the input value is 0 and directly stores 0 as the result if true.
 * - Implements a loop to multiply the result by the current value of R1 (n) and decrement R1
 *   until it reaches 1.
 * - Stores the final result in memory at `result_addr`.
 * - Halts the program.
 */
std::vector<Instruction> createFactorialProgram(uint32_t input_addr, uint32_t result_addr) {
    return {
        {Opcode::LOAD, 0, 1, 2},                // R0 = 1 (result)
        {Opcode::LOAD, 1, input_addr, 0},       // R1 = n
        {Opcode::LOAD, 2, 1, 2},                // R2 = 1
        {Opcode::LOAD, 3, 0, 2},                // R3 = 0

        // if n == 0 → result = 0
        {Opcode::CMP, 0, 1, 3},                 // if R0 == R2 (0)
        {Opcode::JMP, 11, 1, 0},                // jump to store if ZF

        // loop_start
        {Opcode::CMP, 0, 1, 2},                 // if R1 == 1
        {Opcode::JMP, 11, 1, 0},                 // if ZF == 1 → skip MUL/SUB

        {Opcode::MUL, 0, 0, 1},                 // result *= i
        {Opcode::SUB, 1, 1, 2},                 // i--
        {Opcode::JMP, 6, 0, 0},                 // jump to loop_start

        // loop_end
        {Opcode::STORE, result_addr, 0, 0},     // RAM[result_addr] = result
        {Opcode::HALT, 0, 0, 0}
    };
}


/**
 * @brief Generates a program to compute the sum of elements in an array.
 * 
 * This function creates a sequence of instructions for a RISC-like emulator to compute
 * the sum of all elements in an array. The result is stored in the specified memory address.
 * 
 * @param array_addr The memory address where the array starts.
 * @param length_addr The memory address where the length of the array is stored.
 * @param result_addr The memory address where the computed sum will be stored.
 * @return A vector of instructions representing the program to compute the sum of the array.
 * 
 * @details
 * - The program iterates through the array, adding each element to a running total.
 * - If an overflow occurs during addition, the program halts execution.
 * - The result is stored in the memory address specified by `result_addr`.
 */
std::vector<Instruction> createSumListProgram(uint32_t array_addr, uint32_t length_addr, uint32_t result_addr) {
    return {
        {Opcode::LOAD, 0, array_addr, 0},      // R0 = array address
        {Opcode::LOAD, 1, length_addr, 0},     // counter = length
        {Opcode::LOAD, 2, result_addr, 0},     // R2 = result pointer
        {Opcode::LOAD, 3, 1, 2},               // R3 = 1 (for pointer++ and counter--)
        
        // loop_start @ pc = 4
        {Opcode::LOAD, 4, 0, 1},              // R4 = RAM[R0]
        {Opcode::ADD, 2, 2, 4},               // sum += R4
        
        {Opcode::CHECK_FLAG, 5, 1, 0},         // check if sum overflowed
        {Opcode::CMP, 0, 5, 3},               // if overflow
        {Opcode::JMP, 15, 1, 0},              // if ZF, jump to end

        {Opcode::ADD, 0, 0, 3},               // array_ptr++

        {Opcode::CMP, 0, 1, 3},               // if counter == 1
        {Opcode::JMP, 14, 1, 0},              // if ZF, jump to end

        {Opcode::SUB, 1, 1, 3},               // counter--
        {Opcode::JMP, 4, 0, 0},               // loop

        {Opcode::STORE, result_addr, 2, 0},   // store sum
        {Opcode::HALT, 0, 0, 0}
    };
}


/**
 * @brief Generates a program to compute the Fibonacci sequence up to a given number.
 * 
 * This function creates a sequence of instructions for a RISC-like emulator to compute
 * the Fibonacci number for a given input `n`. The result is stored in the specified 
 * memory address.
 * 
 * @param input_addr The memory address where the input value `n` is stored.
 * @param result_addr The memory address where the computed Fibonacci result will be stored.
 * @return A vector of instructions representing the program to compute the Fibonacci sequence.
 * 
 * @details
 * - If `n == 0`, the result is `0`.
 * - If `n == 1`, the result is `1`.
 * - For `n > 1`, the program iteratively computes the Fibonacci sequence using a loop.
 * - The program handles potential overflow during addition and halts execution if detected.
 * - The result is stored in the memory address specified by `result_addr`.
 */
std::vector<Instruction> createFibonacciProgram(uint32_t input_addr, uint32_t result_addr) {
    return {
        {Opcode::LOAD, 0, input_addr, 0},       // R0 = n
        {Opcode::LOAD, 1, 1, 2},                // R1 = 1 (constant 1)
        {Opcode::LOAD, 2, 0, 2},                // R2 = 0 (fib_prev)
        {Opcode::LOAD, 3, 1, 2},                // R3 = 1 (fib_curr)

        // if n == 0 → result = 0
        {Opcode::CMP, 0, 2, 0},                 // if R0 == R2 (0)
        {Opcode::JMP, 21, 1, 0},                // jump to store if ZF

        // if n == 1 → result = 1
        {Opcode::CMP, 0, 1, 0},                 // if R0 == R1 (1)
        {Opcode::JMP, 19, 1, 0},                // jump to store if ZF

        // Loop setup
        {Opcode::LOAD, 4, 1, 2},                // R4 = R1 = R1
 
        // loop_start
        {Opcode::CMP, 0, 4, 0},                 // if i == n
        {Opcode::JMP, 19, 1, 0},                // exit if ZF

        {Opcode::ADD, 5, 2, 3},                 // R5 = fib_prev + fib_curr

        {Opcode::CHECK_FLAG, 6, 1, 0},         // check if sum overflowed
        {Opcode::CMP, 0, 6, 1},               // if overflow
        {Opcode::JMP, 20, 1, 0},              // if ZF, jump to end
        
        {Opcode::MOV, 2, 3, 0},                 // fib_prev = fib_curr
        {Opcode::MOV, 3, 5, 0},                 // fib_curr = next
        
        {Opcode::ADD, 4, 4, 1},                 // i++
        {Opcode::JMP, 9, 0, 0},                 // jump to loop_start

        // store result
        {Opcode::STORE, result_addr, 3, 0},
        {Opcode::HALT, 0, 0, 0},

        // if n = 0 store result coming from control flow
        {Opcode::MOV, 3, 2, 0},                 // R3 = R2 = 0  (fib_curr)
        {Opcode::JMP, 19, 1, 0},                // exit if ZF
        {Opcode::HALT, 0, 0, 0},                
    };
}