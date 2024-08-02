#pragma once

#include <any>
#include <string>
#include <vector>


class Interpreter;
class Token;

class BleachCallable{
  public:
    virtual int arity() = 0;
    virtual std::any call(Interpreter& interpreter, std::vector<std::any> arguments) = 0; // To deal with Bleach Classes, Bleach Functions and Bleach Lambda Functions.
    virtual std::any call(Interpreter& interpreter, Token paren, std::vector<std::any> arguments) = 0; // To deal with Bleach Native Functions.
    virtual std::string toString() = 0;
    virtual ~BleachCallable() = default;
};
