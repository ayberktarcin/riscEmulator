// src/machine_test.cpp
#include "../src/machine.hpp"
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
