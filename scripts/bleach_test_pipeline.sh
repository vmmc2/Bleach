#!/bin/bash

# Directories containing Bleach files and expected output
VALID_BLEACH_PROGRAMS_DIR="../tests/valid_bleach_programs"
INVALID_BLEACH_PROGRAMS_DIR="../to/tests/invalid_bleach_programs"
EXPECTED_VALID_BLEACH_PROGRAMS_OUTPUT_DIR="../tests/valid_bleach_programs_output"
EXPECTED_INVALID_BLEACH_PROGRAMS_OUTPUT_DIR="../tests/invalid_bleach_programs_output"

# Path to the Bleach Interpreter executable
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


show_file_name(){
    echo -e $1
}

# Run tests for valid Bleach files
for subdir in "lexer" "parser" "resolver" "runtime"; do
    for file in "$VALID_BLEACH_PROGRAMS_DIR/$subdir"/*.bch; do
        show_file_name "$file"
    done
done

# Run tests for invalid Bleach files
for subdir in "lexer" "parser" "resolver" "runtime"; do
    for file in "$INVALID_BLEACH_PROGRAMS_DIR/$subdir"/*.bch; do
        show_file_name "$file"
    done
done

# Show summaries
echo ""
printf "${BLUE}Valid Tests Summary:${NC}\n"
printf "${BLUE}Total valid tests: $total_valid${NC}\n"
printf "${GREEN}Passed valid tests: $passed_valid${NC}\n"
printf "${RED}Failed valid tests: $((total_valid - passed_valid))${NC}\n"
echo ""
printf "${BLUE}Invalid Tests Summary:${NC}\n"
printf "${BLUE}Total invalid tests: $total_invalid${NC}\n"
printf "${GREEN}Failed invalid tests: $passed_invalid${NC}\n"
printf "${RED}Passed invalid tests (unexpected success): $((total_invalid - passed_invalid))${NC}\n"
