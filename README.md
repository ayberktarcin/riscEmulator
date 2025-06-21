# RiscEmulator

A simple RISC machine simulator written in modern C++ (C++17). This project emulates a Reduced Instruction Set Computer (RISC) architecture with support for a variety of instructions such as `LOAD`, `STORE`, `ADD`, `SUB`, `CMP`, `JMP`, `HALT`, and more.

> ✅ Built and tested on **Ubuntu 22.04 LTS** using **Visual Studio Code (VSCode)** with the **CMake Tools** extension.

---

## 🚀 Features

- **16 General-Purpose Registers**:
  - Registers `R0–R15` for storing intermediate values and performing computations.
- **Unified Memory Space**:
  - A single memory space for both program instructions and data.
  - Default configuration:
    - **Program Memory**: 256 instructions.
    - **Data Memory**: 1024 words.
- **Simple Instruction Set**:
  - Supports basic operations:
    - Arithmetic: `ADD`, `SUB`, `MUL`, `DIV`.
    - Memory: `LOAD`, `STORE`.
    - Control Flow: `JMP`, `HALT`.
    - Data Transfer: `MOV`.
    - Comparison: `CMP`.
- **Status Register**:
  - Tracks the state of the processor with the following flags:
    - **Zero Flag (ZF)**: Set if the result of an operation is zero.
    - **Carry Flag (CF)**: Set if a carry occurs during an arithmetic operation.
    - **Negative Flag (NF)**: Set if the result of an operation is negative.
    - **Overflow Flag (OF)**: Set if an arithmetic overflow occurs.
    - **Division by Zero Flag (DF)**: Set if a division by zero is attempted.
- **Safe Memory Access**:
  - Functions like `setMemoryValue` and `getMemoryValue` ensure safe and controlled access to memory.
- **Control Flow**:
  - Conditional and unconditional jumps using the `JMP` instruction.
  - Graceful halting with the `HALT` instruction or on critical errors.
- **Modular and Testable Design**:
  - Clean and extensible architecture for easy testing and future enhancements.

---

## 🛠️ Build Instructions
```bash
mkdir build && cd build
cmake ..
make
./RiscEmulator
./MachineTest
gtest
```

### 🔧 Prerequisites

Ensure the following packages are installed:

```bash
sudo apt update
sudo apt install build-essential cmake libgtest-dev
```
🧪 Note: libgtest-dev only installs the source files. You need to build the GoogleTest libraries manually:
```bash
cd /usr/src/gtest
sudo cmake .
sudo make
sudo cp *.a /usr/lib
```
This enables your CMake project to link against gtest and gtest_main properly.