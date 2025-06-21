# RiscEmulator

A simple RISC machine simulator written in modern C++ (C++17). This project emulates a minimal Reduced Instruction Set Computer (RISC) with support for basic instructions such as `LOAD`, `STORE`, `ADD`, `SUB`, `CMP`, `JMP`, and `HALT`.

> ✅ Built and tested on **Ubuntu 22.04 LTS** using **Visual Studio Code (VSCode)** with the **CMake Tools** extension.

---

## 🚀 Features

- 8 general-purpose data_registers (R0–R7)
- Unified memory space (default: 1024 instructions/words)
- Simple instruction set
- Zero flag support for conditional jumps
- Safe memory access functions (`setMemoryValue`, `getMemoryValue`)
- Modular, readable, and testable design

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