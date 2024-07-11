#pragma once

#include <any>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "./BleachCallable.hpp"


class Interpreter;
class BleachFunction;

class BleachClass : public BleachCallable, public std::enable_shared_from_this<BleachClass>{
  private:
    friend class BleachInstance;
    const std::string name;
    const std::map<std::string, std::shared_ptr<BleachFunction>> methods;

  public:
    BleachClass(std::string name, std::map<std::string, std::shared_ptr<BleachFunction>> methods);
    int arity() override;
    std::any call(Interpreter& interpreter, std::vector<std::any> arguments) override;
    std::shared_ptr<BleachFunction> findMethod(const std::string& name);
    std::string toString() override;
};
