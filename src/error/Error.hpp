#pragma once 

#include <iostream>
#include <string_view>

#include "../utils/Token.hpp"


const std::string RED = "\033[31m"; /**< Constant that allows the BLEACH Interpreter to display red colored error messages. */
const std::string WHITE = "\033[37m"; /**< Constant that allows the BLEACH Interpreter to reset the terminal output color back to white after displaying an error message. */

inline bool hadError = false; /**< Variable that ensures that the BLEACH Interpreter will not execute code if there's a syntax error in the source code. */

/**
 * @brief Reports the occcurrence of a syntax error to the user through the standard error stream (usually is the console).
 * 
 * @param errorLine: The line in the source code file where the syntax error happened.
 * @param errorLocation: The location (in terms of tokens) in the source code file where the syntax error happened.
 * @param errorMessage: The error message related to the syntax error that has happened.
 * 
 * @return Nothing (void).
**/
static void report(int errorLine, std::string_view errorLocation, std::string_view errorMessage){
  if(errorLocation.length() != 0){
    std::cerr << RED << "[BLEACH Interpreter Error]: " << "Error occurred at Line: " << errorLine << " - Error happened at location " << errorLocation << " - Error Message: " << errorMessage << "." << WHITE << std::endl;
  }else{
    std::cerr << RED << "[BLEACH Interpreter Error]: " << "Error occurred at Line: " << errorLine << " - Error Message: " << errorMessage << "." << WHITE << std::endl;
  }
  hadError = true;

  return;
}

/**
 * @brief Calls the "report" function to register the occurrence of a syntax error (found in the lexer) in the source code file.
 * 
 * @param errorLine: The line in the source code file where the syntax error happened.
 * @param errorMessage: The error message related to the syntax error that has happened.
 * 
 * @return Nothing (void).
**/
static void error(int errorLine, std::string_view errorMessage){
  report(errorLine, "", errorMessage);
  
  return;
}

/**
 * @brief Calls the "report" function to register the occurrence of a syntax error (found in the parser) in the source code file.
 * 
 * @param token: The token in the source code file (from the sequence of tokens) where the syntax (parsing) error has happened.
 * @param errorMessage: The error message related to the syntax (parsing) error that has happened.
 * 
 * @return Nothing (void).
**/
static void error(const Token& token, std::string_view errorMessage){
  if(token.type == TokenType::FILE_END){
    report(token.line, "at the end of the file.", errorMessage);
  }else{
    report(token.line, "'" + token.lexeme + "'.", errorMessage);
  }

  return;
}
