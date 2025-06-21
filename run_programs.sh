#!/bin/bash

mkdir -p outputs.txt

# Run the tests and write output to markdown
./build/MachineTest >  tests/test_output.txt
./build/RiscEmulator > src/main_output.txt

# Append a summary
echo -e "\n✅ Tests completed and output written to tests/test_output.txt"
echo -e "✅ Main program output written to src/main_output.txt\n"