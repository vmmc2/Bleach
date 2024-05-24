#pragma once 

#include <iostream>
#include <string_view>

// Variable that ensures that the BLEACH Interpreter will not execute code if there's a syntax error in the source code.
inline int hadError = false;

// Reports the occcurrence of a syntax error to the user through the standard error stream (usually is the console).
//
// Args:
//   errorLine: The line in the source code file where the syntax error happened.
//   errorLocation: The location (in terms of tokens) in the source code file where the syntax error happened.
//   errorMessage: The error message related to the syntax error that has happened.  
//
// Returns:
//   Nothing (void).
static void report(int errorLine, std::string_view errorLocation, std::string_view errorMessage){
  std::cerr << "[BLEACH Interpreter Error]: " << "Error occurred at Line: " << errorLine << " - Error happened at location " << errorLocation << " - Error Message: " << errorMessage << "." << std::endl;
  hadError = true;

  return;
}

// Calls the "report" function to register the occurrence of a syntax error in the source code file.
//
// Args:
//   errorLine: The line in the source code file where the syntax error happened.
//   errorMessage: The error message related to the syntax error that has happened.
//
// Returns:
//   Nothing (void).
static void error(int errorLine, std::string_view errorMessage){
  report(errorLine, "", errorMessage);
  return;
}