#include <utility>

#include "./BleachInstance.hpp"


BleachInstance::BleachInstance(std::shared_ptr<BleachClass> klass)
  : klass{std::move(klass)}
{}

std::string BleachInstance::toString(){
  return "class instance"; // Must change this later...
}