#pragma once

#include <any>
#include <memory>
#include <utility>

#include "./BleachCallable.hpp"


class Environment; // Forward declaration necessary to implement the BleachLambdaFunction class.
class LambdaFunction; // Forward declaration necessary to implement the BleachLambdaFunction class.

/**
 * @class BleachLambdaFunction
 * 
 * @brief This class is responsible for representing a lambda function (anonymous function) value during 
 * runtime.
 * 
 * The BleachLambdaFunction class is responsible for providing a runtime representation of a lambda function,
 * also known as anonymous function, value. This struct has 2 attributes: The first one is called "closure". It
 * is the nearest enclosing environment with respect to the lambda (anonymous) function. The second one is called
 * "lambdaFunctionDeclaration". It is a specific type of expression that represents, at static time, a lambda
 * (anonymous) function.
**/
class BleachLambdaFunction : public BleachCallable{
  private:
    std::shared_ptr<Environment> closure;
    std::shared_ptr<LambdaFunction> lambdaFunctionDeclaration;
  public:
    BleachLambdaFunction(std::shared_ptr<LambdaFunction> lambdaFunctionDeclaration, std::shared_ptr<Environment> closure);
    int arity() override;
    std::any call(Interpreter& interpreter, std::vector<std::any> arguments) override;
    std::any call(Interpreter& interpreter, Token paren, std::vector<std::any> arguments) override;
    std::string toString() override;
};
