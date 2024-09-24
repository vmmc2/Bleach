# Check if a file path is provided as an argument
if [ $# -eq 0 ]; then
    # No arguments provided, start the REPL
    echo -e "${GREEN}Starting the Bleach REPL... ${NC}"
    ../src/BleachInterpreter
else
    # Argument provided, execute the specified Bleach file
    echo -e "${GREEN}Executing the Bleach file (.bch): '$1' ${NC}"
    ../src/BleachInterpreter "$1"
fi