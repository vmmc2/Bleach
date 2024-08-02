#pragma once 

#include <any>
#include <memory>
#include <string>
#include <vector>

#include "./BleachCallable.hpp"

class BleachInstance;
class Environment;
class Function;

class BleachFunction : public BleachCallable{
  private:
    bool isInitializer;
    std::shared_ptr<Environment> closure;
    std::shared_ptr<Function> functionDeclaration;
  public:
    BleachFunction(std::shared_ptr<Function> functionDeclaration, std::shared_ptr<Environment> closure, bool isInitializer);
    int arity() override;
    std::shared_ptr<BleachFunction> bind(std::shared_ptr<BleachInstance> instance);
    std::any call(Interpreter& interpreter, std::vector<std::any> arguments) override;
    std::any call(Interpreter& interpreter, Token paren, std::vector<std::any> arguments) override;
    std::string toString() override;
};
