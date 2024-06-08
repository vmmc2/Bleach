#pragma once

#include <stdexcept>

#include "../utils/Token.hpp"

class BleachRuntimeError : public std::runtime_error{
  public:
    const Token& token;
    
    BleachRuntimeError(const Token& token, std::string_view runtimeErrorMessage)
      : std::runtime_error{runtimeErrorMessage.data()}, token{token}
    {}
};