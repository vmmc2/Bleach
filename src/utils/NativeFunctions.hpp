#pragma once

#include <any>
#include <cmath>
#include <chrono>
#include <string>
#include <vector>

#include "./BleachCallable.hpp"
#include "./BleachRuntimeError.hpp"

class Interpreter;


// std::chrono::clock
class NativeClock : public BleachCallable{
  public:
    int arity() override{
      return 0;
    }
    
    std::any call(Interpreter& interpreter, std::vector<std::any> arguments) override{
      auto now = std::chrono::high_resolution_clock::now();
      auto duration = now.time_since_epoch();

      return std::chrono::duration<double>(duration).count();
    }

    std::string toString() override{
      return "<native function: std::io::clock>";
    }
};

// std::io::readLine
class NativeInput : public BleachCallable{

};

// std::io::print
class NativePrint : public BleachCallable{

};

// std::io::fileRead
class NativeFileRead : public BleachCallable{

};

// std::io::fileWrite
class NativeFileWrite : public BleachCallable{

};

// std::math::abs
class NativeAbsoluteValue : public BleachCallable{
  public:
    int arity() override{
      return 1;
    }

    std::any call(Interpreter& interpreter, std::vector<std::any> arguments) override{
      if(arguments.size() > 1){
        throw BleachRuntimeError{"Invalid number of arguments. Expected " + std::to_string(arity()) + " arguments but received " + std::to_string(arguments.size()) + " arguments."};
      }

      if(arguments[0].type() != typeid(double)){
        throw BleachRuntimeError{"Argument of the 'std::math::abs' function must be a number."};
      }

      double number = std::any_cast<double>(arguments[0]);

      return std::fabs(number);
    }

    std::string toString() override{
      return "<native function: std::io::abs>";
    }
};

// std::math::exp
class NativeExponentiation : public BleachCallable{

};

// std::math::log
class NativeLogarithm : public BleachCallable{

};

// std::math::random
class NativeRandom : public BleachCallable{

};

// std::math::sqrt
class NativeSquareRoot : public BleachCallable{
  public:
    int arity() override{
      return 1;
    }

    std::any call(Interpreter& interpreter, std::vector<std::any> arguments) override{
      if(arguments.size() > 1){
        throw BleachRuntimeError{"Invalid number of arguments. Expected " + std::to_string(arity()) + " arguments but received " + std::to_string(arguments.size()) + " arguments."};
      }

      if(arguments[0].type() != typeid(double)){
        throw BleachRuntimeError{"Argument of the 'std::math::sqrt' function must be a number."};
      }

      double radicand = std::any_cast<double>(arguments[0]);
      if(radicand < 0){
        throw BleachRuntimeError{"Argument of the 'std::math::sqrt' function cannot be a negative number."};
      }

      return std::sqrt(radicand);
    }

    std::string toString() override{
      return "<native function: std::io::sqrt>";
    }
};
