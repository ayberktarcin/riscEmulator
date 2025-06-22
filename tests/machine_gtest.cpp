/**
 * @file machine_gtest.cpp
 * @brief Unit tests for the RISC Emulator.
 *
 * This file contains a suite of Google Test cases to verify the functionality
 * of the RISC emulator, including arithmetic operations, memory handling,
 * control flow, and specific programs like factorial, sum of a list, and Fibonacci.
 */

#include "../src/machine.hpp"
#include "../src/algorithms.hpp"
#include "../src/instruction.hpp"
#include <gtest/gtest.h>

class RiscMachineTest : public ::testing::Test {
protected:
    RiscMachine machine;

    void SetUp() override {
        machine = RiscMachine();
    }
};

TEST_F(RiscMachineTest, LOADandSTORE) {
    machine.setMemoryValue(100, 42);
    std::vector<Instruction> program = {
        {Opcode::LOAD, 0, 100, 0},
        {Opcode::STORE, 101, 0, 0},
        {Opcode::HALT, 0, 0, 0}
    };
    machine.loadProgram(program);
    machine.run();
    EXPECT_EQ(machine.getMemoryValue(101), 42);
}

TEST_F(RiscMachineTest, LOAD_ImmediateValue) {
    std::vector<Instruction> program = {
        {Opcode::LOAD, 0, 123, 2},   // R0 = 123 (immediate)
        {Opcode::STORE, 100, 0, 0},
        {Opcode::HALT, 0, 0, 0}
    };
    machine.loadProgram(program);
    machine.run();
    EXPECT_EQ(machine.getMemoryValue(100), 123);
}

TEST_F(RiscMachineTest, ADD) {
    machine.setMemoryValue(100, 10);
    machine.setMemoryValue(101, 20);
    std::vector<Instruction> program = {
        {Opcode::LOAD, 0, 100, 0},
        {Opcode::LOAD, 1, 101, 0},
        {Opcode::ADD, 2, 0, 1},
        {Opcode::STORE, 102, 2, 0},
        {Opcode::HALT, 0, 0, 0}
    };
    machine.loadProgram(program);
    machine.run();
    EXPECT_EQ(machine.getMemoryValue(102), 30);
}

TEST_F(RiscMachineTest, SUB) {
    machine.setMemoryValue(100, 50);
    machine.setMemoryValue(101, 20);
    std::vector<Instruction> program = {
        {Opcode::LOAD, 0, 100, 0},
        {Opcode::LOAD, 1, 101, 0},
        {Opcode::SUB, 2, 0, 1},
        {Opcode::STORE, 102, 2, 0},
        {Opcode::HALT, 0, 0, 0}
    };
    machine.loadProgram(program);
    machine.run();
    EXPECT_EQ(machine.getMemoryValue(102), 30);
}

TEST_F(RiscMachineTest, MUL) {
    machine.setMemoryValue(100, 7);
    machine.setMemoryValue(101, 6);
    std::vector<Instruction> program = {
        {Opcode::LOAD, 0, 100, 0},
        {Opcode::LOAD, 1, 101, 0},
        {Opcode::MUL, 2, 0, 1},
        {Opcode::STORE, 102, 2, 0},
        {Opcode::HALT, 0, 0, 0}
    };
    machine.loadProgram(program);
    machine.run();
    EXPECT_EQ(machine.getMemoryValue(102), 42);
}

TEST_F(RiscMachineTest, DIV) {
    machine.setMemoryValue(100, 21);
    machine.setMemoryValue(101, 7);
    std::vector<Instruction> program = {
        {Opcode::LOAD, 0, 100, 0},
        {Opcode::LOAD, 1, 101, 0},
        {Opcode::DIV, 2, 0, 1},
        {Opcode::STORE, 102, 2, 0},
        {Opcode::HALT, 0, 0, 0}
    };
    machine.loadProgram(program);
    machine.run();
    EXPECT_EQ(machine.getMemoryValue(102), 3);
}

TEST_F(RiscMachineTest, CMPandJMP) {
    machine.setMemoryValue(100, 5);
    machine.setMemoryValue(101, 5);
    std::vector<Instruction> program = {
        {Opcode::LOAD, 0, 100, 0},
        {Opcode::LOAD, 1, 101, 0},
        {Opcode::CMP, 0, 0, 1},
        {Opcode::JMP, 6, 1, 0},
        {Opcode::LOAD, 2, 100, 0},
        {Opcode::HALT, 0, 0, 0},
        {Opcode::STORE, 102, 0, 0},
        {Opcode::HALT, 0, 0, 0}
    };
    machine.loadProgram(program);
    machine.run();
    EXPECT_EQ(machine.getMemoryValue(102), 5);
}

TEST_F(RiscMachineTest, DIVbyZero) {
    machine.setMemoryValue(100, 10);
    machine.setMemoryValue(101, 0);
    std::vector<Instruction> program = {
        {Opcode::LOAD, 0, 100, 0},
        {Opcode::LOAD, 1, 101, 0},
        {Opcode::DIV, 2, 0, 1},
        {Opcode::STORE, 102, 2, 0},
        {Opcode::HALT, 0, 0, 0}
    };
    machine.loadProgram(program);
    machine.run();
    EXPECT_EQ(machine.getStatusRegister().DF, 1); // Division by zero flag should be set
}

TEST_F(RiscMachineTest, JMPUnconditional) {
    machine.setMemoryValue(100, 99);
    std::vector<Instruction> program = {
        {Opcode::JMP, 3, 0, 0},
        {Opcode::LOAD, 0, 100, 0}, // Should be skipped
        {Opcode::HALT, 0, 0, 0},   // Should be skipped
        {Opcode::STORE, 101, 0, 0},
        {Opcode::HALT, 0, 0, 0}
    };
    machine.loadProgram(program);
    machine.run();
    EXPECT_EQ(machine.getMemoryValue(101), 0);  // R0 was never modified
}

TEST_F(RiscMachineTest, MOV) {
    machine.setMemoryValue(100, 99);
    std::vector<Instruction> program = {
        {Opcode::LOAD, 0, 100, 0},      // R0 = 99
        {Opcode::MOV, 1, 0, 0},         // R1 = R0
        {Opcode::STORE, 101, 1, 0},     // RAM[101] = R1
        {Opcode::HALT, 0, 0, 0}
    };
    machine.loadProgram(program);
    machine.run();
    EXPECT_EQ(machine.getMemoryValue(101), 99);
}

TEST_F(RiscMachineTest, CHECK_FLAG_Overflow) {
    machine.setMemoryValue(100, UINT32_MAX);
    machine.setMemoryValue(101, 2);
    std::vector<Instruction> program = {
        {Opcode::LOAD, 0, 100, 0},
        {Opcode::LOAD, 1, 101, 0},
        {Opcode::ADD, 2, 0, 1},              // R2 = UINT32_MAX + 2 → overflow
        {Opcode::HALT, 0, 0, 0}
    };
    machine.loadProgram(program);
    machine.run();
    EXPECT_EQ(machine.getStatusRegister().CF, 1);  // Carry should be set
}

// Factorial Test Region

class FactorialTest : public ::testing::Test {
    protected:
        RiscMachine machine{512, 512};
  
        void runFactorial(uint32_t n, uint32_t expected) {
            machine.reset();
            machine.setMemoryValue(100, n);   // input n
            machine.setMemoryValue(101, 0);   // result
    
            std::vector<Instruction>  program = createFactorialProgram(100, 101);
            machine.loadProgram(program);
            machine.run();
    
            EXPECT_EQ(machine.getMemoryValue(101), expected);
        }
    };


TEST_F(FactorialTest, FactorialOf0) {
    runFactorial(0, 1);
}

TEST_F(FactorialTest, FactorialOf1) {
    runFactorial(1, 1);
}

TEST_F(FactorialTest, FactorialOf2) {
    runFactorial(2, 2);
}

TEST_F(FactorialTest, FactorialOf5) {
    runFactorial(5, 120);
}

TEST_F(FactorialTest, FactorialOf12) {
    runFactorial(12, 479001600);  // fits
}

TEST_F(FactorialTest, FactorialOf13TriggersOverflow) {
    machine.reset();
    
    // Input: 13! = 6227020800, which overflows uint32_t (max is 4,294,967,295)
    machine.setMemoryValue(100, 13);   // input n
    machine.setMemoryValue(101, 0);    // result output address

    auto program = createFactorialProgram(100, 101);
    machine.loadProgram(program);
    machine.run();

    // Fetch result (it will be incorrect due to overflow)
    uint32_t result = machine.getMemoryValue(101);

    // Assert overflow flag is set
    EXPECT_TRUE(machine.getStatusRegister().OF);
}


// SumList Test Region

class SumListTest : public ::testing::Test {
    protected:
        RiscMachine machine{512, 512};
   
        void runSumTest(const std::vector<uint32_t>& values, uint32_t expected_sum, bool expect_carry = false) {

            machine.reset();

            uint32_t base = 400;
            for (size_t i = 0; i < values.size(); ++i) {
                machine.setMemoryValue(base + i, values[i]);
            }
    
            machine.setMemoryValue(300, base);               // pointer
            machine.setMemoryValue(301, values.size());      // length
            machine.setMemoryValue(302, 0);                  // result
    
            auto program = createSumListProgram(300, 301, 302);
            machine.loadProgram(program);
            machine.run();
    
            if (expect_carry) {
                EXPECT_EQ(machine.getStatusRegister().CF, expect_carry);
            } else {
                EXPECT_EQ(machine.getMemoryValue(302), expected_sum);
            }
        }
    };
    
TEST_F(SumListTest, SumOfSmallArray) {
    runSumTest({10, 20, 30}, 60);
}

TEST_F(SumListTest, SumOfSmallArray2) {
    runSumTest({10, 20, 30, 40}, 100);
}

TEST_F(SumListTest, OverflowSum) {
    runSumTest({UINT32_MAX - 10, 20}, 9, true);
}

TEST_F(SumListTest, ZeroLengthArray) {
    runSumTest({}, 0);
}

TEST_F(SumListTest, AllZeroValues) {
    runSumTest({0, 0, 0, 0}, 0);
}

TEST_F(SumListTest, SingleElementArray) {
    runSumTest({42}, 42);
}


// Fibonacci Test Region

class FibonacciTest : public ::testing::Test {
    protected:
        RiscMachine machine{512, 512};
    
        void runFibonacci(uint32_t n, uint32_t expected_result) {
            machine.reset();
    
            machine.setMemoryValue(100, n);    // input n
            machine.setMemoryValue(101, 0);    // result address
    
            auto program = createFibonacciProgram(100, 101);
            machine.loadProgram(program);
            machine.run();
    
            uint32_t result = machine.getMemoryValue(101);
            EXPECT_EQ(result, expected_result);
        }
    };

TEST_F(FibonacciTest, FibOf0) {
    runFibonacci(0, 0);
}

TEST_F(FibonacciTest, FibOf1) {
    runFibonacci(1, 1);
}

TEST_F(FibonacciTest, FibOf5) {
    runFibonacci(5, 5);
}

TEST_F(FibonacciTest, FibOf6) {
    runFibonacci(6, 8);
}

TEST_F(FibonacciTest, FibOf10) {
    runFibonacci(10, 55);
}
 
TEST_F(FibonacciTest, FibOf47_NoOverflow) {
    runFibonacci(47, 2971215073);
}

TEST_F(FibonacciTest, FibOf48_Overflow) {
    machine.reset();

    machine.setMemoryValue(100, 48);
    machine.setMemoryValue(101, 0); // result address

    auto program = createFibonacciProgram(100, 101);
    machine.loadProgram(program);
    machine.run();

    EXPECT_TRUE(machine.getStatusRegister().CF);
}
