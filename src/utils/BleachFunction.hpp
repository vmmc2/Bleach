#pragma once 

#include <any>
#include <memory>
#include <string>
#include <vector>

#include "./BleachCallable.hpp"


class BleachInstance; // Forward declaration necessary to implement the BleachFunction class.
class Environment; // Forward declaration necessary to implement the BleachFunction class.
class Function; // Forward declaration necessary to implement the BleachFunction class.

/**
 * @class BleachFunction
 * 
 * @brief This class is responsible for representing, at runtime, a user-defined function inside a Bleach 
 * program.
 *  
 * The BleachFunction class is responsible for providing a runtime representation of every user-defined function 
 * in a Bleach program. This class has 3 attributes: The first one is "isInitializer". It is a boolean that 
 * signals whether or not the instance of this class is a constructor method (remember that, in this interpreter
 * implementation, there is no distinction between functions and methods during runtime). The second one is
 * "closure". It is a pointer that refers to the instance of the Environment class that is nearest one that 
 * encloses the declaration of this function/method. The third one is "functionDeclaration". It is a pointer that
 * refers to an instance of the Function class. This instance of the Function class is the one that represents
 * this instance of the BleachFunction during static time.
**/
class BleachFunction : public BleachCallable{
  private:
    bool isInitializer;
    std::shared_ptr<Environment> closure;
    std::shared_ptr<Function> functionDeclaration;
    
  public:
    BleachFunction(std::shared_ptr<Function> functionDeclaration, std::shared_ptr<Environment> closure, bool isInitializer);
    int arity() override;
    std::shared_ptr<BleachFunction> bind(std::shared_ptr<BleachInstance> instance);
    std::any call(Interpreter& interpreter, std::vector<std::any> arguments) override;
    std::any call(Interpreter& interpreter, Token paren, std::vector<std::any> arguments) override;
    std::string toString() override;
};
