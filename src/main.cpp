#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "error/Error.hpp"
#include "lexer/Lexer.hpp"
#include "parser/Parser.hpp"


/**
 * @brief Receives a path to a file, checks whether the file exists and if it is a Bleach file, extracts all of
 * its content and stores it in a string. Then, return such string.
 * 
 * This function is responsible for receiving the relative file path of a supposed Bleach file. Then, it checks
 * whether the file exists and whether it is, indeeed, a Bleach file. Then, it tries to open the said file. If
 * everything went okay, then this function extracts the whole content of the file, stores it inside a string
 * (std::string) variable called "fileContent" and returns it.
 * 
 * @param filePath A string (std::string_view) that stores the relative file path to the Bleach file that will
 * be interpreted.
 * 
 * @return A string (std::string) that contains the content (source code) written inside the file pointed by the
 * provided file path.
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
 * @brief This function lexes, parse, analyze and execute/interpret the source code present inside a Bleach file
 * provided in the "sourceCode" variable.
 * 
 * This function is responsible for instantiating (step-by-step) the lexer, parser, static analyzer and 
 * interpreter/runtime of the Bleach language. This function will perform each step of the interpreter pipeline
 * and, if an error is found during one of these steps, then the whole process will be interrupted.
 * 
 * @param sourceCode A string (std::string_view) that contains the source code of a Bleach file.
 * 
 * @return Nothing (void).
**/
void run(std::string_view sourceCode){
  /* First Step: Lexing */
  Lexer lexer{sourceCode};
  std::vector<Token> tokens = lexer.lexTokens();

  if(hadError){
    std::exit(65);
  }

  for(const Token& token : tokens){
    std::cout << token.toString() << std::endl;
  }

  /* Second Step: Parsing */
  Parser parser{tokens};
  std::shared_ptr<Expr> expression = parser.parse();

  if(hadError){
    std::exit(65);
  }

  /* Third Step: Interpret */


  return;
}

/**
 * @brief Receives the relative file path to a Bleach file, opens such file, reads its content and pass it to
 * another function, so it can be executed/interpreted.
 * 
 * This function is responsible for receiving the relative path to a Bleach file as a string (std::string_view),
 * then it will call the auxiliar function 'readFile' to get the content of such file. Finally, it will call the
 * 'run' function to execute/interpret the contents present inside the file pointed by the received relative
 * file path.
 * 
 * @param filePath A string (std::string_view) that represents the relative path to a Bleach file, whose content
 * will be executed by the BLEACH Interpreter.
 * 
 * @return Nothing (void).
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
 * @brief Starts up the BLEACH Interpreter in the "REPL Mode".
 * 
 * This function is responsible for starting up the BLEACH Interpreter in its "REPL Mode". By doing so, it
 * enters an infinite loop, where it expects Bleach statements. When a statement is provided, it is read by
 * the interpreter, then evaluated (interpreted) and the loop starts again. To interrupt the BLEACH Interpreter
 * in the "REPL Mode", just press Ctrl+C or Ctrl+D.
 * 
 * @return Nothing (void).
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
 * @brief The entry point of the BLEACH Interpreter. Runs the interpreter in the file mode or in the REPL mode,
 * depending on whether or not you have passed parameters to the executable when asking the OS to run it.
 * 
 * This function is responsible for being the entry point of the BLEACH interpreter. It starts up the interpreter
 * in two possible modes ("File Mode" or "REPL Mode").
 * To start up the interpreter in the "File Mode", you must compile the project using the provided Makefile and
 * then provide the relative path of the Bleach file you want to compile as the only argument when executing
 * the generated binary.
 * To start up the interpreter in the "REPL Mode", you must compile the project using the provided Makefile and
 * then just execute the generated binary.
 * 
 * @param argc: The int that represents the number of arguments passed when running the executable.
 * @param argv: The array of strings (char* []) that stores the values of each of the passed arguments.
 * 
 * @return An int that denotes whether the 'main' function executed without problems (0) or with problems (a number different from 0).
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
