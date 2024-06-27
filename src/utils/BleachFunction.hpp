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
    std::shared_ptr<Function> functionDeclaration;
  public:
    BleachFunction(std::shared_ptr<Function> functionDeclaration);
    int arity() override;
    std::any call(Interpreter& interpreter, std::vector<std::any> arguments) override;
    std::string toString() override;
};
