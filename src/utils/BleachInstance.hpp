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
    std::map<std::string, std::any> fields; // Do not forget that this is a runtime representation of an instance/object. That's why we use the "std::any" type here.
  public:
    BleachInstance(std::shared_ptr<BleachClass> klass);
    std::any get(const Token& name);
    void set(const Token& name, std::any value);
    std::string toString();
};
