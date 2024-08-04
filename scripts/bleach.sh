#!/bin/bash

# Color codes
RED='\033[31m'
GREEN='\033[0;32m'
YELLOW='\033[33m'
NC='\033[0m' # No Color

# Navigate to the subdirectory where the Makefile is located
cd ../src

# Clean and build the project
echo -e "${YELLOW}Cleaning the project... ${NC}"
if ! make clean; then
  echo -e "${RED}Cleaning failed. Exiting... ${NC}"
  exit 1
fi
echo -e "${GREEN}Project cleaned! ${NC}"


echo -e "${YELLOW}Building the project... ${NC}"
if ! make; then
  echo -e "${RED}Build failed. Exiting... ${NC}"
  exit 1
fi
echo -e "${GREEN}Project built successfully! ${NC}"

# Check if a file path is provided as an argument
if [ $# -eq 0 ]; then
    # No arguments provided, start the REPL
    echo -e "${GREEN}Starting the Bleach REPL... ${NC}"
    ./BleachInterpreter
else
    # Argument provided, execute the specified Bleach file
    echo -e "${GREEN}Executing the Bleach file (.bch): '$1' ${NC}"
    ./BleachInterpreter "$1"
fi
