#!/bin/bash

mkdir -p tests/README

# Run the tests and write output to markdown
build/MachineTest > tests/test_output.txt

# Append a summary
echo -e "\n✅ Tests completed and output written to tests/test_output.txt"