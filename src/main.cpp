#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "error/Error.hpp"
#include "lexer/Lexer.hpp"


std::string readFile(std::string_view filePath){
  std::ifstream file{filePath.data(), std::ios::in | std::ios::binary | std::ios::ate};

  if(!file){
    std::cerr << "\033[31m [BLEACH Interpreter Error]: Failed to open file '" << filePath << "': " << std::strerror(errno) << std::endl;
    std::exit(74); 
  }

  std::string fileContent;
  fileContent.resize(file.tellg());

  file.seekg(0, std::ios::beg);
  file.read(fileContent.data(), fileContent.size());
  file.close();

  return fileContent;
}

void run(std::string_view sourceCode){
  Lexer lexer{sourceCode};
  std::vector<Token> tokens = lexer.lexTokens();

  for(const Token& token : tokens){
    std::cout << token.toString() << std::endl;
  }

  return;
}

void runFile(std::string_view filePath){
  std::string fileContent = readFile(filePath);
  run(fileContent);

  if(hadError){
    std::exit(65);
  }

  return;
}

void runPrompt(){
  std::string line;

  std::cout << "> ";
  while(std::getline(std::cin, line)){
    run(line);
    hadError = false;
    std::cout << "> ";
  }

  return;
}

int main(int argc, char* argv[]){
  if(argc == 2){
    runFile(argv[1]);
  }else if(argc == 1){
    runPrompt();
  }else{
    std::cerr << "\033[31m [BLEACH Interpreter Error] Incorrect use of the interpreter. \033[0m" << std::endl;
    std::cerr << "\033[31m There are two options for you to run the interprter: \033[0m" << std::endl;
    std::cerr << "\033[31m 1) Starting up the interactive interpreter through the command: ./BleachInterpreter \033[0m" << std::endl;
    std::cerr << "\033[31m 2) Passing a Bleach file to the interpreter so it can execute it through the command: ./BleachInterpreter file_name.bah \033[0m" << std::endl;
    std::exit(64);
  }

  return 0;
}