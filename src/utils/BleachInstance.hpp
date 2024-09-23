#pragma once

#include <any>
#include <map>
#include <memory>
#include <string>
#include <vector>


class BleachClass; // Forward declaration necessary to implement the BleachInstance class.
class BleachFunction; // Forward declaration necessary to implement the BleachInstance class.
class Interpreter; // Forward declaration necessary to implement the BleachInstance class.
class Token; // Forward declaration necessary to implement the BleachInstance class.

/**
 * @class BleachInstance
 * 
 * @brief This class is responsible for representing, at runtime, an instance of a user-defined class inside a 
 * Bleach program.
 *  
 * The BleachInstance class is responsible for providing a runtime representation of every instance from any
 * user-defined class that was defined by the user inside a Bleach program. This class has 2 attributes: The 
 * first one is "klass". It is a pointer to an instance of a BleachClass class. This is used to figure out to
 * what class an instance belongs to. The second one is "fields". It is a map that stores key-value pairs where
 * the key is a string representing the name of an attribute/field stored inside that instance of the 
 * BleachInstance class and its corresponding value is the value of the attribute/field. 
**/
class BleachInstance : public std::enable_shared_from_this<BleachInstance>{
  private:
    std::shared_ptr<BleachClass> klass;
    std::map<std::string, std::any> fields; // Do not forget that this is a runtime representation of an instance/object. That's why we use the "std::any" type here.
  public:
    BleachInstance(std::shared_ptr<BleachClass> klass);
    std::string formatDouble(double value);
    std::any get(const Token& name);
    void set(const Token& name, std::any value);
    std::string toString(Interpreter& interpreter);
};
