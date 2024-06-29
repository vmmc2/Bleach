#pragma once 

#include <any>
#include <memory>
#include <string>
#include <vector>

#include "./BleachCallable.hpp"


class Environment;
class Function;

class BleachFunction : public BleachCallable{
  private:
    std::shared_ptr<Environment> closure;
    std::shared_ptr<Function> functionDeclaration;
  public:
    BleachFunction(std::shared_ptr<Function> functionDeclaration, std::shared_ptr<Environment> closure);
    int arity() override;
    std::any call(Interpreter& interpreter, std::vector<std::any> arguments) override;
    std::string toString() override;
};
