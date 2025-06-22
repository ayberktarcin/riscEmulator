// src/machine_test.cpp
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

TEST_F(RiscMachineTest, SumArrayProgramWorks) {
    RiscMachine machine(512, 512);

    // Setup array in RAM: [10, 20, 30, 40]
    machine.setMemoryValue(100, 200);  // array pointer
    machine.setMemoryValue(101, 4);    // array length
    machine.setMemoryValue(103, 0);    // result address (initial value)

    machine.setMemoryValue(200, 10);
    machine.setMemoryValue(201, 20);
    machine.setMemoryValue(202, 30);
    machine.setMemoryValue(203, 40);

    std::vector<Instruction> program = createSumListProgram(100, 101, 103);
    machine.loadProgram(program);
    machine.run();

    EXPECT_EQ(machine.getMemoryValue(103), 100);;
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
    
        void SetUp() override {
            machine.reset();
            machine.setMemoryValue(303, 1); // constant 1
        }
    
        void runSumTest(const std::vector<uint32_t>& values, uint32_t expected, bool expect_overflow = false) {
            uint32_t base = 400;
            for (size_t i = 0; i < values.size(); ++i) {
                machine.setMemoryValue(base + i, values[i]);
            }
    
            machine.setMemoryValue(300, base);               // pointer
            machine.setMemoryValue(301, values.size());      // length
            machine.setMemoryValue(302, 0);                  // result
    
            auto program = createSumListProgram(300, 301, 302, 303);
            machine.loadProgram(program);
            machine.run();
    
            EXPECT_EQ(machine.getMemoryValue(302), expected);
            EXPECT_EQ(machine.getStatusRegister().OF, expect_overflow);
        }
    };
    

TEST_F(SumListTest, OverflowSum) {
    runSumTest({UINT32_MAX - 10, 20}, 9, true);
}