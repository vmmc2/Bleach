#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "error/Error.hpp"
#include "lexer/Lexer.hpp"

/**
 * @brief 
 * 
 * @param 
 * 
 * @return 
**/
std::string readFile(std::string_view filePath){
  std::ifstream file{filePath.data(), std::ios::in | std::ios::binary | std::ios::ate};

  if(!file){
    std::cerr << RED << "[BLEACH Interpreter Error]: Failed to open file '" << filePath << "': " << std::strerror(errno) << WHITE << std::endl;
    std::exit(74); 
  }

  std::string fileContent;
  fileContent.resize(file.tellg());

  file.seekg(0, std::ios::beg);
  file.read(fileContent.data(), fileContent.size());
  file.close();

  return fileContent;
}

/**
 * @brief 
 * 
 * @param 
 * 
 * @return 
**/
void run(std::string_view sourceCode){
  Lexer lexer{sourceCode};
  std::vector<Token> tokens = lexer.lexTokens();

  for(const Token& token : tokens){
    std::cout << token.toString() << std::endl;
  }

  return;
}

/**
 * @brief 
 * 
 * @param 
 * 
 * @return 
**/
void runFile(std::string_view filePath){
  std::string fileContent = readFile(filePath);
  run(fileContent);

  if(hadError){
    std::exit(65);
  }

  return;
}

/**
 * @brief 
 * 
 * @param 
 * 
 * @return 
**/
void runPrompt() {
  for (;;) {
    std::cout << "> ";
    std::string line;
    if (!std::getline(std::cin, line)) break;
    run(line);
    hadError = false;
  }

  return;
}

/**
 * @brief 
 * 
 * @param 
 * 
 * @return 
**/
int main(int argc, char* argv[]){
  if(argc == 2){
    runFile(argv[1]);
  }else if(argc == 1){
    runPrompt();
  }else{
    std::cout << RED << "[BLEACH Interpreter Error] Incorrect use of the interpreter." << std::endl;
    std::cout << "There are two options for you to run the interprter:" << std::endl;
    std::cout << " 1) Starting up the interactive interpreter through the command: ./BleachInterpreter" << std::endl;
    std::cout << " 2) Passing a Bleach file to the interpreter so it can execute it through the command: ./BleachInterpreter file_name.bah" << WHITE << std::endl;
    std::exit(64);
  }

  return 0;
}