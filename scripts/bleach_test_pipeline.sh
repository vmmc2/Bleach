#!/bin/bash

# Directories containing Lox files and expected output
VALID_LOX_DIR="path/to/valid_lox_files"
INVALID_LOX_DIR="path/to/invalid_lox_files"
EXPECTED_VALID_OUTPUT_DIR="path/to/expected_valid_outputs"
EXPECTED_INVALID_OUTPUT_DIR="path/to/expected_invalid_outputs"
# Path to your Lox interpreter executable
INTERPRETER="./BleachInterpreter"
# Directory to save output logs
LOG_DIR="path/to/logs"
mkdir -p "$LOG_DIR"  # Create the log directory if it does not exist

# Define color codes
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Counters for tests
total_valid=0
passed_valid=0
total_invalid=0
passed_invalid=0

# Function to run a single valid test
run_valid_test() {
    local file=$1
    local log_file="$LOG_DIR/$(basename "$file").log"
    local expected_file="$EXPECTED_VALID_OUTPUT_DIR/$(basename "$file").expected"

    printf "${YELLOW}Running valid test: $file...${NC}\n"
    if $INTERPRETER "$file" >"$log_file" 2>&1; then
        if diff -q "$log_file" "$expected_file" > /dev/null; then
            printf "${GREEN}Valid test passed: $file${NC}\n"
            ((passed_valid++))
        else
            printf "${RED}Valid test failed (output mismatch): $file${NC}\n"
        fi
    else
        printf "${RED}Valid test failed (interpreter error): $file${NC}\n"
    fi
    ((total_valid++))
}

# Function to run a single invalid test
run_invalid_test() {
    local file=$1
    local log_file="$LOG_DIR/$(basename "$file").log"

    printf "${YELLOW}Running invalid test: $file...${NC}\n"
    if $INTERPRETER "$file" >"$log_file" 2>&1; then
        printf "${RED}Invalid test passed (unexpected success): $file${NC}\n"
    else
        printf "${GREEN}Invalid test failed (as expected): $file${NC}\n"
        ((passed_invalid++))
    fi
    ((total_invalid++))
}

# Run tests for valid Lox files
for file in "$VALID_LOX_DIR"/*.lox; do
    run_valid_test "$file"
done

# Run tests for invalid Lox files
for file in "$INVALID_LOX_DIR"/*.lox; do
    run_invalid_test "$file"
done

# Show summaries
printf "${BLUE}Valid Tests Summary:${NC}\n"
printf "${BLUE}Total valid tests: $total_valid${NC}\n"
printf "${GREEN}Passed valid tests: $passed_valid${NC}\n"
printf "${RED}Failed valid tests: $((total_valid - passed_valid))${NC}\n"

printf "${BLUE}Invalid Tests Summary:${NC}\n"
printf "${BLUE}Total invalid tests: $total_invalid${NC}\n"
printf "${GREEN}Failed invalid tests: $passed_invalid${NC}\n"
printf "${RED}Passed invalid tests (unexpected success): $((total_invalid - passed_invalid))${NC}\n"
