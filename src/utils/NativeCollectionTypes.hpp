#pragma once

#include <any>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "./BleachCallable.hpp"
#include "./BleachInstance.hpp"
#include "../error/BleachRuntimeError.hpp"


class Interpreter;

// std::collections::array
class BleachArrayConstructor : public BleachCallable{
  public:
    int arity() override{
      return 0;
    }

    std::any call(Interpreter& interpreter, std::vector<std::any> arguments) override{
      std::cout << "No implementation of this method available for the 'BleachArrayConstructor' class." << std::endl;
      
      return {};
    }
    
    std::any call(Interpreter& interpreter, Token paren, std::vector<std::any> arguments) override{
      return std::make_shared<BleachArray>();
    }

    std::string toString() override{
      return "<native function: std::collections::array>";
    }
};

class BleachArray : public BleachInstance{
  private:
    std::vector<std::any> array;

  public:
    std::any get(const Token& name){
      if (name.lexeme == "get") {
        return std::make_shared<BleachCallable>([this](Interpreter& interpreter, const std::vector<std::any>& arguments) -> std::any {
            int index = std::any_cast<int>(arguments[0]);
            return array[index];
        });
      }else if(name.lexeme == "set"){

      }else if(name.lexeme == "append"){

      }else if(name.lexeme == "back"){

      }else if(name.lexeme == "clear"){

      }else if(name.lexeme == "empty"){

      }else if(name.lexeme == "eraseAt"){

      }else if(name.lexeme == "front"){

      }else if(name.lexeme == "insertAt"){

      }else if(name.lexeme == "pop"){

      }else if(name.lexeme == "size"){

      }
    }

    void set(const Token& name, std::any value){

    }

    std::string toString(Interpreter& interpreter){

    }
};

// std::collections::string
class BleachStringConstructor : public BleachCallable{
  public:
    int arity() override{
      return 1;
    }

    std::any call(Interpreter& interpreter, std::vector<std::any> arguments) override{
      std::cout << "No implementation of this method available for the 'BleachStringConstructor' class." << std::endl;
      
      return {};
    }
    
    std::any call(Interpreter& interpreter, Token paren, std::vector<std::any> arguments) override{

    }

    std::string toString() override{
      return "<native function: std::collections::string>";
    }
};

class BleachString{

};