#include <utility>

#include "./BleachClass.hpp"


BleachClass::BleachClass(std::string name)
  : name{std::move(name)}
{}

int BleachClass::arity(){
  return 0;
}

std::any BleachClass::call(Interpreter& interpreter, std::vector<std::any> arguments){
  auto instance = std::make_shared<BleachInstance>(shared_from_this());
  
  return instance;
}

std::shared_ptr<BleachFunction> BleachClass::findMethod(const std::string& name){
  return nullptr;
}

std::string BleachClass::toString(){
  return name;
}
