#pragma once

#include <any>
#include <cmath>
#include <chrono>
#include <random>
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
      if(arguments.size() != 0){
        throw BleachRuntimeError{"Invalid number of arguments. Expected " + std::to_string(arity()) + " arguments but received " + std::to_string(arguments.size()) + " arguments."};
      }

      auto now = std::chrono::high_resolution_clock::now();
      auto duration = now.time_since_epoch();

      return std::chrono::duration<double>(duration).count();
    }

    std::string toString() override{
      return "<native function: std::io::clock>";
    }
};

// std::io::readLine
class NativeReadLine : public BleachCallable{
  public:
    int arity() override{
      return 1;
    }

    std::any call(Interpreter& interpreter, std::vector<std::any> arguments) override{
      if(arguments.size() != 0){
        throw BleachRuntimeError{"Invalid number of arguments. Expected " + std::to_string(arity()) + " arguments but received " + std::to_string(arguments.size()) + " arguments."};
      }

      std::string lineContent;
      std::getline(std::cin, lineContent);

      return lineContent;
    }

    std::string toString() override{
      return "<native function: std::io::readLine>";
    } 
};

// std::io::print
// I will need to implement a more robust version of this class. It must also be able to deal with the following
// types: BleachList, BleachDict, BleachInstance.
class NativePrint : public BleachCallable{
  public:
    int arity() override{
      return -1; // This means that the native function expects a variable number of arguments.
    }

    void printValue(std::any value){
      if(value.type() == typeid(double)){
        std::cout << std::any_cast<double>(value) << " ";
      }else if(value.type() == typeid(bool)){
        std::cout << std::any_cast<bool>(value) << " ";
      }else if(value.type() == typeid(nullptr)){
        std::cout << "nil" << " ";
      }else if(value.type() == typeid(std::string)){
        std::cout << std::any_cast<std::string>(value) << " ";
      }else{
        throw BleachRuntimeError{"The value cannot be printed."};
      }

      return;
    }

    std::any call(Interpreter& interpreter, std::vector<std::any> arguments) override{
      for(std::any argument : arguments){
        printValue(argument);
      }

      std::cout << std::endl;

      return {};
    }

    std::string toString() override{
      return "<native function: std::io::print>";
    } 
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
      if(arguments.size() != 1){
        throw BleachRuntimeError{"Invalid number of arguments. Expected " + std::to_string(arity()) + " arguments but received " + std::to_string(arguments.size()) + " arguments."};
      }

      if(arguments[0].type() != typeid(double)){
        throw BleachRuntimeError{"Argument of the 'std::math::abs' function must be a number."};
      }

      double number = std::any_cast<double>(arguments[0]);

      return std::fabs(number);
    }

    std::string toString() override{
      return "<native function: std::math::abs>";
    }
};

// std::math::pow
class NativeExponentiation : public BleachCallable{
  public:
    int arity() override{
      return 2;
    }

    std::any call(Interpreter& interpreter, std::vector<std::any> arguments) override{
      if(arguments.size() != 2){
        throw BleachRuntimeError{"Invalid number of arguments. Expected " + std::to_string(arity()) + " arguments but received " + std::to_string(arguments.size()) + " arguments."};
      }

      if(arguments[0].type() != typeid(double) || arguments[1].type() != typeid(double)){
        throw BleachRuntimeError{"The two arguments of the 'std::math::pow' function must be numbers."};
      }

      double base = std::any_cast<double>(arguments[0]);
      double exponent = std::any_cast<double>(arguments[1]);

      return std::pow(base, exponent);
    }

    std::string toString() override{
      return "<native function: std::math::pow>";
    }
};

// std::math::log
class NativeLogarithm : public BleachCallable{
  public:
    int arity() override{
      return 2;
    }

    std::any call(Interpreter& interpreter, std::vector<std::any> arguments) override{
      const double epsilon = 1e-9;

      if(arguments.size() != 2){
        throw BleachRuntimeError{"Invalid number of arguments. Expected " + std::to_string(arity()) + " arguments but received " + std::to_string(arguments.size()) + " arguments."};
      }

      if(arguments[0].type() != typeid(double) || arguments[1].type() != typeid(double)){
        throw BleachRuntimeError{"The two arguments of the 'std::math::log' function must be numbers."};
      }

      double base = std::any_cast<double>(arguments[0]);
      double argument = std::any_cast<double>(arguments[1]);

      if(std::fabs(base - 1) <= epsilon || std::signbit(base)){
        throw BleachRuntimeError{"The first argument (the base of the logarithm) of the 'std::math::log' must be a positive number and different from 1."};
      }
      if(std::signbit(argument)){
        throw BleachRuntimeError{"The second argument (the argument of the logarithm) of the 'std::math::log' must be a positive number."};
      }

      double num = std::log10(argument);
      double den = std::log10(base);

      if(std::fabs(den) <= epsilon){
        throw BleachRuntimeError{"Internal error while computing the logarithm of " + std::to_string(argument) + " in base " + std::to_string(base) + "."};
      }

      return (num/den);
    }

    std::string toString() override{
      return "<native function: std::math::log>";
    }
};

// std::random::random
class NativeRandom : public BleachCallable{
  public:
    int arity() override{
      return 2;
    }

    std::any call(Interpreter& interpreter, std::vector<std::any> arguments) override{
      if(arguments.size() != 2){
        throw BleachRuntimeError{"Invalid number of arguments. Expected " + std::to_string(arity()) + " arguments but received " + std::to_string(arguments.size()) + " arguments."};
      }

      if(arguments[0].type() != typeid(double) || arguments[1].type() != typeid(double)){
        throw BleachRuntimeError{"The two arguments of the 'std::random::random' function must be numbers."};
      }

      double left = std::any_cast<double>(arguments[0]);
      double right = std::any_cast<double>(arguments[1]);

      unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

      std::mt19937 gen(seed);
      std::uniform_real_distribution<> distribution(std::fmin(left, right), std::fmax(left, right));

      double randomNumber = distribution(gen);

      return randomNumber;
    }

    std::string toString() override{
      return "<native function: std::random::random>";
    }  
};

// std::math::sqrt
class NativeSquareRoot : public BleachCallable{
  public:
    int arity() override{
      return 1;
    }

    std::any call(Interpreter& interpreter, std::vector<std::any> arguments) override{
      if(arguments.size() != 1){
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
      return "<native function: std::math::sqrt>";
    }
};
