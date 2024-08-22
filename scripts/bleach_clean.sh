#!/bin/bash

# Color codes
GREEN='\033[0;32m'
YELLOW='\033[33m'
NC='\033[0m' # No Color

echo -e "${YELLOW}Cleaning the project... ${NC}"

cd ../src
make clean

echo -e "${GREEN}Project cleaned! ${NC}"
