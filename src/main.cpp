#include <iostream>
#include <string>


void runFile(std::string_view filePath){

  return;
}

void runPrompt(){

  return;
}

int main(int argc, char* argv[]){
  if(argc == 2){
    runFile(argv[1]);
  }else if(argc == 1){
    runPrompt();
  }else{
    std::cout << "Incorrect use of the Bleach Interpreter." << std::endl;
    std::cout << "There are two options for you to run the interprter:" << std::endl;
    std::cout << " 1) Starting up the interactive interpreter through the command: ./BleachInterpreter" << std::endl;
    std::cout << " 2) Passing a Bleach file to the interpreter so it can execute it through the command: ./BleachInterpreter file_name.bah" << std::endl;
    std::exit(64);
  }

  return 0;
}