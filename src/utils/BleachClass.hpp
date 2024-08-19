#pragma once

#include <any>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "./BleachCallable.hpp"


class Interpreter; // Forward declaration necessary to implement the BleachClass class.
class BleachFunction; // Forward declaration necessary to implement the BleachClass class.

/**
 * @class BleachClass
 * 
 * @brief This class is responsible for representing, at runtime, a user-defined class inside a Bleach program.
 *  
 * The BleachClass class is responsible for providing a runtime representation of every class that was defined 
 * by the user inside a Bleach program. This class has 3 attributes: The first one is "name". It is a string
 * that stores the name of the user-defined class whose runtime representation refers to. The second one is
 * "superclass". It is a pointer that refers to the instance of another BleachClass that will work as the
 * superclass of the current class. Pay attention to the fact that not every class has a superclass. If that's
 * the case, then the value of "superclass" will be nullptr. The third one is "methods". It is a map that stores
 * key-value pairs. In this map, a key is the name of a method (a string) and its associated value is a runtime 
 * representation of the method that was declared inside the class.
**/
class BleachClass : public BleachCallable, public std::enable_shared_from_this<BleachClass>{
  private:
    friend class BleachInstance; // Instances of the "BleachInstance" class can access the private attributes of this class.

    const std::string name;
    const std::shared_ptr<BleachClass> superclass;
    std::map<std::string, std::shared_ptr<BleachFunction>> methods;

  public:
    BleachClass(std::string name, std::shared_ptr<BleachClass> superclass, std::map<std::string, std::shared_ptr<BleachFunction>> methods);
    int arity() override;
    std::any call(Interpreter& interpreter, std::vector<std::any> arguments) override;
    std::any call(Interpreter& interpreter, Token paren, std::vector<std::any> arguments) override;
    std::shared_ptr<BleachFunction> findMethod(const std::string& name);
    std::string toString() override;
};
