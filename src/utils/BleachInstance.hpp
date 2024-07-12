#pragma once

#include <any>
#include <map>
#include <memory>
#include <string>


class BleachClass;
class Token;

class BleachInstance : public std::enable_shared_from_this<BleachInstance>{
  private:
    std::shared_ptr<BleachClass> klass;
  public:
    BleachInstance(std::shared_ptr<BleachClass> klass);
    std::string toString();
};