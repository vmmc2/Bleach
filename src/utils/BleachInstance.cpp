#include <utility>

#include "./BleachInstance.hpp"
#include "../error/BleachRuntimeError.hpp"


BleachInstance::BleachInstance(std::shared_ptr<BleachClass> klass)
  : klass{std::move(klass)}
{}

std::any BleachInstance::get(const Token& name){
  auto elem = fields.find(name.lexeme);
  if(elem != fields.end()){
    return elem->second;
  }

  throw BleachRuntimeError{name, "Undefined property '" + name.lexeme + "'."};
}

void BleachInstance::set(const Token& name, std::any value){
  fields[name.lexeme] = std::move(value);

  return;
}

std::string BleachInstance::toString(){
  return "class instance"; // Must change this later...
}