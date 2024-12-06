#!/bin/bash

# Directories containing Bleach files and expected output
VALID_BLEACH_PROGRAMS_DIR="../tests/valid_bleach_programs"
INVALID_BLEACH_PROGRAMS_DIR="../tests/invalid_bleach_programs"
EXPECTED_VALID_BLEACH_PROGRAMS_OUTPUT_DIR="../tests/valid_bleach_programs_output"
EXPECTED_INVALID_BLEACH_PROGRAMS_OUTPUT_DIR="../tests/invalid_bleach_programs_output"

# Path to the Bleach Interpreter executable
BLEACH_BUILD="./bleach_build.sh"
INTERPRETER="../src/BleachInterpreter"

# Directory to save output logs
LOG_DIR="../logs"
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

$BLEACH_BUILD

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

# Function to run a single valid test
run_valid_test() {
    bleach_file=$1 # file to be executed
    log_file=$2 # file with the produced result by the executed file
    expected_result_file=$3 # file with the expected result to be generated the executed file

    printf "${YELLOW}Running valid test: $bleach_file${NC}\n"
    if $INTERPRETER "$bleach_file" > "$log_file" 2>&1; then
        if diff -q "$log_file" "$expected_result_file" > /dev/null; then
            printf "${GREEN}Valid test passed: $bleach_file${NC}\n"
            ((passed_valid++))
        else
            printf "${RED}Valid test failed (output mismatch): $bleach_file${NC}\n"
        fi
    else
        printf "${RED}Valid test failed (interpreter error): $bleach_file${NC}\n"
    fi
    ((total_valid++))
}

# Run tests for valid Bleach files
for subdir in "expressions" "native_functions" "statements"; do
    for file in "$VALID_BLEACH_PROGRAMS_DIR/$subdir"/*.bch; do
        run_valid_test "$file" "$LOG_DIR/valid_bleach_programs/$subdir/$(basename "$file").log" "$EXPECTED_VALID_BLEACH_PROGRAMS_OUTPUT_DIR/$subdir/$(basename "$file").expected"
    done
done

# Show summaries
echo ""
printf "${BLUE}Bleach Test Suite Execution Summary${NC}\n"
printf "${BLUE}Total valid tests: $total_valid${NC}\n"
printf "${GREEN}Passed valid tests: $passed_valid${NC}\n"
printf "${RED}Failed valid tests: $((total_valid - passed_valid))${NC}\n"
