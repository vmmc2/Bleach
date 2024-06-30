#pragma once

#include <any>
#include <memory>
#include <utility>

#include "./BleachCallable.hpp"

class Environment;
class LambdaFunction;

class BleachLambdaFunction : public BleachCallable{
  private:
    std::shared_ptr<Environment> closure;
    std::shared_ptr<LambdaFunction> lambdaFunctionDeclaration;
  public:
    BleachLambdaFunction(std::shared_ptr<LambdaFunction> lambdaFunctionDeclaration, std::shared_ptr<Environment> closure);
    int arity() override;
    std::any call(Interpreter& interpreter, std::vector<std::any> arguments) override;
    std::string toString() override;
};