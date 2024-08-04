#!/bin/bash

# Directory containing Bleach files
BLEACH_DIR="path/to/bch_files"
# Path to the Bleach Tree-Walk Interpreter executable
INTERPRETER="../src/BleachInterpreter"

# Define color codes
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Counter for tests
total_tests=0
passed_tests=0

# Function to run a single test
run_test() {
    local file=$1
    printf "${YELLOW}Running test: '$file'...${NC}\n"
    if $INTERPRETER "$file"; then
        printf "${GREEN}Test passed: '$file'${NC}\n"
        ((passed_tests++))
    else
        printf "${RED}Test failed: '$file'${NC}\n"
    fi
    ((total_tests++))
}

# Iterate over all .bch files in the directory
for file in "$BLEACH_DIR"/*.bch; then
    run_test "$file"
done

# Show summary
printf "${BLUE}Total tests: $total_test${NC}\n"
printf "${GREEN}Passed: $passed_tests${NC}\n"
printf "${RED}Failed: $((total_tests - passed_tests))${NC}\n"
