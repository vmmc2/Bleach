#pragma once 

#include <iostream>
#include <string_view>


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
  std::cerr << "[BLEACH Interpreter Error]: " << "Error occurred at Line: " << errorLine << " - Error happened at location " << errorLocation << " - Error Message: " << errorMessage << "." << std::endl;
  hadError = true;

  return;
}

/**
 * @brief Calls the "report" function to register the occurrence of a syntax error in the source code file.
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