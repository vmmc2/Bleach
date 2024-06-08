#pragma once

#include <stdexcept>

#include "../utils/Token.hpp"


/**
 * @class BleachRuntimeError
 * 
 * @brief Performs the role of being the native runtime error class of the Bleach language.
 *
 * The BleachRuntimeError class is responsible for representing the runtime error class of the Bleach language.
 * Whenever an error that can happen only during the code execution occurs, an instance of this class will be
 * thrown.
**/
class BleachRuntimeError : public std::runtime_error{
  public:
    const Token& token; /**< Variable that represents the token that triggered the runtime error. */
    
    /**
     * @brief Constructs a BleachRuntimeError with the token that was responsible for causing the runtime error. 
     *
     * This constructor initializes a BleachRuntimeError object with the token that was the cause to trigger such
     * an error, represented as a const Token&.
     *
     * @param token: The token at which the runtime error has happened.
     * @param runtimeErrorMessage: The error message related to the runtime error that has happened.
    **/
    BleachRuntimeError(const Token& token, std::string_view runtimeErrorMessage)
      : std::runtime_error{runtimeErrorMessage.data()}, token{token}
    {}
};