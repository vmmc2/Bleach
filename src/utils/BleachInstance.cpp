#include <utility>

#include "./BleachInstance.hpp"
#include "../error/BleachRuntimeError.hpp"


BleachInstance::BleachInstance(std::shared_ptr<BleachClass> klass)
  : klass{std::move(klass)}
{}

std::any BleachInstance::get(const Token& name){
  // When some property of an instance is accessed, first we check if its a field/attribute.
  auto elem = fields.find(name.lexeme);
  if(elem != fields.end()){
    return elem->second;
  }

  // If that's not the case, then we check whether it's a method from the class of the instance.
  // In short, fields/attributes shadow methods.
  auto method = klass->findMethod(name.lexeme);
  if(method != nullptr){
    return method->bind(shared_from_this());
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