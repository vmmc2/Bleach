#pragma once

#include <any>
#include <string>
#include <vector>


class Interpreter; // Forward declaration necessary to implement the BleachCallable class.
class Token; // Forward declaration necessary to implement the BleachCallable class.

/**
 * @class BleachCallable
 * 
 * @brief This class is responsible for providing an interface to implement the callables of the Bleach
 * programming language.
 * 
 * The BleachCallable class is responsible for providing an "interface" so the callables of the Bleach language
 * can be properly implemented. Remember that a callable is any entity in a programming language that can be 
 * "called" like a function. This means that such entities can be invoked with a pair of parentheses and 
 * possibly with arguments inside such pair of parentheses . In Bleach's case, this includes: functions, methods,
 * lambda (anonymous) functions, native functions and classes (when instantiating an object). It doesn't have 
 * any attributes. Instead, it just has virtual methods that must be implemented by any class that inherits from
 * this class.
**/
class BleachCallable{
  public:
    virtual int arity() = 0; // Returns the expected number of arguments of the callable.
    virtual std::any call(Interpreter& interpreter, std::vector<std::any> arguments) = 0; // To deal with Bleach Classes, Bleach Functions and Bleach Lambda Functions.
    virtual std::any call(Interpreter& interpreter, Token paren, std::vector<std::any> arguments) = 0; // To deal with Bleach Native Functions.
    virtual std::string toString() = 0; // Returns the string representation of the callable.
    virtual ~BleachCallable() = default;
};
