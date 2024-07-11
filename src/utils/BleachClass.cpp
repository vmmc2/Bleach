#include <utility>

#include "./BleachClass.hpp"


BleachClass::BleachClass(std::string name, std::map<std::string, std::shared_ptr<BleachFunction>> methods)
  : name{std::move(name)}, methods{std::move(methods)}
{}

int BleachClass::arity(){
  return 0;
}

std::any BleachClass::call(Interpreter& interpreter, std::vector<std::any> arguments){
  return {};
}

std::shared_ptr<BleachFunction> BleachClass::findMethod(const std::string& name){
  return nullptr;
}

std::string BleachClass::toString(){
  return name;
}
