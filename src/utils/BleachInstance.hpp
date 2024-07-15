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
    std::map<std::string, std::any> fields;
  public:
    BleachInstance(std::shared_ptr<BleachClass> klass);
    std::any get(const Token& name);
    std::string toString();
};