#pragma once

#include <any>
#include <cmath>
#include <chrono>
#include <fstream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#include "./BleachCallable.hpp"
#include "../error/BleachRuntimeError.hpp"


class Interpreter;

// std::chrono::clock
class NativeClock : public BleachCallable{
  public:
    int arity() override{
      return 0;
    }

    std::any call(Interpreter& interpreter, std::vector<std::any> arguments) override{
      std::cout << "No implementation of this method available for the 'NativeClock' class." << std::endl;
      
      return {};
    } 
    
    std::any call(Interpreter& interpreter, Token paren, std::vector<std::any> arguments) override{
      if(arguments.size() != 0){
        const Token functionName{TokenType::IDENTIFIER, "std::chrono::clock", toString(), paren.line};
        throw BleachRuntimeError{functionName, "Invalid number of arguments. Expected " + std::to_string(arity()) + " arguments but received " + std::to_string(arguments.size()) + " arguments."};
      }

      auto now = std::chrono::high_resolution_clock::now();
      auto duration = now.time_since_epoch();

      return std::chrono::duration<double>(duration).count();
    }

    std::string toString() override{
      return "<native function: std::chrono::clock>";
    }
};

// std::io::readLine
class NativeReadLine : public BleachCallable{
  public:
    int arity() override{
      return 0;
    }

    std::any call(Interpreter& interpreter, std::vector<std::any> arguments) override{
      std::cout << "No implementation of this method available for the 'NativeReadLine' class." << std::endl;
      
      return {};
    }

    std::any call(Interpreter& interpreter, Token paren, std::vector<std::any> arguments) override{
      if(arguments.size() != 0){
        Token functionName{TokenType::IDENTIFIER, "std::io::ReadLine", toString(), paren.line};
        throw BleachRuntimeError{functionName, "Invalid number of arguments. Expected " + std::to_string(arity()) + " arguments but received " + std::to_string(arguments.size()) + " arguments."};
      }

      std::string lineContent;
      std::getline(std::cin, lineContent);

      return lineContent;
    }

    std::string toString() override{
      return "<native function: std::io::readLine>";
    } 
};

// std::io::fileRead
class NativeFileRead : public BleachCallable{
  public:
    int arity() override{
      return 1;
    }

    std::any call(Interpreter& interpreter, std::vector<std::any> arguments) override{
      std::cout << "No implementation of this method available for the 'NativeFileRead' class." << std::endl;
      
      return {};
    }

    bool hasTxtExtension(std::string filePath){
      const std::string extension = ".txt";

      if (filePath.length() >= extension.length()) {
          return (filePath.rfind(extension) == (filePath.length() - extension.length()));
      }

      return false;
    }

    std::any call(Interpreter& interpreter, Token paren, std::vector<std::any> arguments) override{
      Token functionName{TokenType::IDENTIFIER, "std::io::fileRead", toString(), paren.line};
      if(arguments.size() != 1){
        throw BleachRuntimeError{functionName, "Invalid number of arguments. Expected " + std::to_string(arity()) + " arguments but received " + std::to_string(arguments.size()) + " arguments."};
      }

      if(arguments[0].type() != typeid(std::string)){
        throw BleachRuntimeError{functionName, "Argument of the 'std::io::fileRead' function must be a string."};
      }

      std::string filePath = std::any_cast<std::string>(arguments[0]); // Can either be complete/full path or relative path.

      if(!hasTxtExtension(filePath)){
        throw BleachRuntimeError{functionName, "The 'std::io::fileRead' native function can only read the contents of files with a '.txt' extension."};
      }

      std::ifstream file(filePath);
      if(!file){
        throw BleachRuntimeError{functionName, "Could not open the provided file: '" + filePath + "'."};
      }

      std::ostringstream fileContent;
      fileContent << file.rdbuf();

      return fileContent.str();
    }

    std::string toString() override{
      return "<native function: std::io::fileRead>";
    }
};

// std::io::fileWrite
class NativeFileWrite : public BleachCallable{
  public:
    int arity() override{
      return 4;
    }

    std::any call(Interpreter& interpreter, std::vector<std::any> arguments) override{
      std::cout << "No implementation of this method available for the 'NativeFileWrite' class." << std::endl;
      
      return {};
    }

    bool hasTxtExtension(std::string filePath){
      const std::string extension = ".txt";

      if (filePath.length() >= extension.length()) {
          return (filePath.rfind(extension) == (filePath.length() - extension.length()));
      }

      return false;
    }

    std::any call(Interpreter& interpreter, Token paren, std::vector<std::any> arguments) override{
      Token functionName{TokenType::IDENTIFIER, "std::io::fileWrite", toString(), paren.line};
      if(arguments.size() != 4){
        throw BleachRuntimeError{functionName, "Invalid number of arguments. Expected " + std::to_string(arity()) + " arguments but received " + std::to_string(arguments.size()) + " arguments."};
      }

      if(arguments[0].type() != typeid(std::string) || arguments[1].type() != typeid(std::string) || arguments[2].type() != typeid(std::string) || arguments[3].type() != typeid(bool)){
        throw BleachRuntimeError{functionName, "The first 3 arguments of the 'std::io::fileWrite' function must be all strings. The fourth and last one must be a boolean."};
      }

      std::string filePath = std::any_cast<std::string>(arguments[0]); // Can either be complete/full path or relative path.
      std::string openMode = std::any_cast<std::string>(arguments[1]); // Can either be "w" (write) or "a" (append).
      std::string contentToWrite = std::any_cast<std::string>(arguments[2]); // The content that will be written to the destination file.
      bool insertNewLine = std::any_cast<bool>(arguments[3]); // Boolean that signals whether or not a newline must be added to the file after writing the provided content.

      if(!hasTxtExtension(filePath)){
        throw BleachRuntimeError{functionName, "The 'std::io::fileWrite' native function can only write content to files with a '.txt' extension."};
      }
      if(openMode != "w" && openMode != "a"){
        throw BleachRuntimeError{functionName, "The 'std::io::fileWrite' native function only has two modes of opening and writing to a file: 'a' (append) or 'w' (write)."};
      }

      std::ofstream file(filePath, openMode == "a" ? std::ios::app : std::ios::out);
      if(!file){
        throw BleachRuntimeError{functionName, "Could not open the provided file: '" + filePath + "' in the given mode."};
      }

      file << contentToWrite;
      if(!file){
        throw BleachRuntimeError{functionName, "Could not write the content to the provided file: '" + filePath + "' in the given mode."};
      }

      if(insertNewLine){
        file << "\n";
      }

      return nullptr;
    }

    std::string toString() override{
      return "<native function: std::io::fileWrite>";
    }
};

// std::math::abs
class NativeAbsoluteValue : public BleachCallable{
  public:
    int arity() override{
      return 1;
    }

    std::any call(Interpreter& interpreter, std::vector<std::any> arguments) override{
      std::cout << "No implementation of this method available for the 'NativeAbsoluteValue' class." << std::endl;
      
      return {};
    }

    std::any call(Interpreter& interpreter, Token paren, std::vector<std::any> arguments) override{
      Token functionName{TokenType::IDENTIFIER, "std::math::abs", toString(), paren.line};
      if(arguments.size() != 1){
        throw BleachRuntimeError{functionName, "Invalid number of arguments. Expected " + std::to_string(arity()) + " arguments but received " + std::to_string(arguments.size()) + " arguments."};
      }

      if(arguments[0].type() != typeid(double)){
        throw BleachRuntimeError{functionName, "Argument of the 'std::math::abs' function must be a number."};
      }

      double number = std::any_cast<double>(arguments[0]);

      return std::fabs(number);
    }

    std::string toString() override{
      return "<native function: std::math::abs>";
    }
};

// std::math::ceil
class NativeCeil : public BleachCallable{
  public:
    int arity() override{
      return 1;
    }

    std::any call(Interpreter& interpreter, std::vector<std::any> arguments) override{
      std::cout << "No implementation of this method available for the 'NativeCeil' class." << std::endl;
      
      return {};
    }

    std::any call(Interpreter& interpreter, Token paren, std::vector<std::any> arguments) override{
      Token functionName{TokenType::IDENTIFIER, "std::math::ceil", toString(), paren.line};

      if(arguments.size() != 1){
        throw BleachRuntimeError{functionName, "Invalid number of arguments. Expected " + std::to_string(arity()) + " arguments but received " + std::to_string(arguments.size()) + " arguments."};
      }

      if(arguments[0].type() != typeid(double)){
        throw BleachRuntimeError{functionName, "The argument of the 'std::math::ceil' function must be a number."};
      }

      double value = std::any_cast<double>(arguments[0]);

      return std::ceil(value) == -0 ? 0 : std::ceil(value);
    }

    std::string toString() override{
      return "<native function: std::math::ceil>";
    }
};

// std::math::floor
class NativeFloor : public BleachCallable{
  public:
    int arity() override{
      return 1;
    }

    std::any call(Interpreter& interpreter, std::vector<std::any> arguments) override{
      std::cout << "No implementation of this method available for the 'NativeFloor' class." << std::endl;
      
      return {};
    }

    std::any call(Interpreter& interpreter, Token paren, std::vector<std::any> arguments) override{
      Token functionName{TokenType::IDENTIFIER, "std::math::floor", toString(), paren.line};

      if(arguments.size() != 1){
        throw BleachRuntimeError{functionName, "Invalid number of arguments. Expected " + std::to_string(arity()) + " arguments but received " + std::to_string(arguments.size()) + " arguments."};
      }

      if(arguments[0].type() != typeid(double)){
        throw BleachRuntimeError{functionName, "The argument of the 'std::math::floor' function must be a number."};
      }

      double value = std::any_cast<double>(arguments[0]);

      return std::floor(value);
    }

    std::string toString() override{
      return "<native function: std::math::floor>";
    }
};

// std::math::log
class NativeLogarithm : public BleachCallable{
  public:
    int arity() override{
      return 2;
    }

    std::any call(Interpreter& interpreter, std::vector<std::any> arguments) override{
      std::cout << "No implementation of this method available for the 'NativeLogarithm' class." << std::endl;
      
      return {};
    }

    std::any call(Interpreter& interpreter, Token paren, std::vector<std::any> arguments) override{
      const double epsilon = 1e-9;
      Token functionName{TokenType::IDENTIFIER, "std::math::log", toString(), paren.line};

      if(arguments.size() != 2){
        throw BleachRuntimeError{functionName, "Invalid number of arguments. Expected " + std::to_string(arity()) + " arguments but received " + std::to_string(arguments.size()) + " arguments."};
      }

      if(arguments[0].type() != typeid(double) || arguments[1].type() != typeid(double)){
        throw BleachRuntimeError{functionName, "The two arguments of the 'std::math::log' function must be numbers."};
      }

      double base = std::any_cast<double>(arguments[0]);
      double argument = std::any_cast<double>(arguments[1]);

      if(std::fabs(base - 1) <= epsilon || std::signbit(base)){
        throw BleachRuntimeError{functionName, "The first argument (the base of the logarithm) of the 'std::math::log' must be a positive number and different from 1."};
      }
      if(std::signbit(argument)){
        throw BleachRuntimeError{functionName, "The second argument (the argument of the logarithm) of the 'std::math::log' must be a positive number."};
      }

      double num = std::log10(argument);
      double den = std::log10(base);

      if(std::fabs(den) <= epsilon){
        throw BleachRuntimeError{functionName, "Internal error while computing the logarithm of " + std::to_string(argument) + " in base " + std::to_string(base) + "."};
      }

      return (num/den);
    }

    std::string toString() override{
      return "<native function: std::math::log>";
    }
};

// std::math::pow
class NativeExponentiation : public BleachCallable{
  public:
    int arity() override{
      return 2;
    }

    std::any call(Interpreter& interpreter, std::vector<std::any> arguments) override{
      std::cout << "No implementation of this method available for the 'NativeExponentiation' class." << std::endl;
      
      return {};
    }

    std::any call(Interpreter& interpreter, Token paren, std::vector<std::any> arguments) override{
      Token functionName{TokenType::IDENTIFIER, "std::math::pow", toString(), paren.line};
      if(arguments.size() != 2){
        throw BleachRuntimeError{functionName, "Invalid number of arguments. Expected " + std::to_string(arity()) + " arguments but received " + std::to_string(arguments.size()) + " arguments."};
      }

      if(arguments[0].type() != typeid(double) || arguments[1].type() != typeid(double)){
        throw BleachRuntimeError{functionName, "The two arguments of the 'std::math::pow' function must be numbers."};
      }

      double base = std::any_cast<double>(arguments[0]);
      double exponent = std::any_cast<double>(arguments[1]);

      return std::pow(base, exponent);
    }

    std::string toString() override{
      return "<native function: std::math::pow>";
    }
};

// std::math::sqrt
class NativeSquareRoot : public BleachCallable{
  public:
    int arity() override{
      return 1;
    }

    std::any call(Interpreter& interpreter, std::vector<std::any> arguments) override{
      std::cout << "No implementation of this method available for the 'NativeSquareRoot' class." << std::endl;
      
      return {};
    }

   std::any call(Interpreter& interpreter, Token paren, std::vector<std::any> arguments) override{
      Token functionName{TokenType::IDENTIFIER, "std::math::sqrt", this->toString(), paren.line};
      if(arguments.size() != 1){
        throw BleachRuntimeError{functionName, "Invalid number of arguments. Expected " + std::to_string(arity()) + " arguments but received " + std::to_string(arguments.size()) + " arguments."};
      }

      if(arguments[0].type() != typeid(double)){
        throw BleachRuntimeError{functionName, "Argument of the 'std::math::sqrt' function must be a number."};
      }

      double radicand = std::any_cast<double>(arguments[0]);
      if(radicand < 0){
        throw BleachRuntimeError{functionName, "Argument of the 'std::math::sqrt' function cannot be a negative number."};
      }

      return std::sqrt(radicand);
    }

    std::string toString() override{
      return "<native function: std::math::sqrt>";
    }
};

// std::random::random
class NativeRandom : public BleachCallable{
  public:
    int arity() override{
      return 2;
    }

    std::any call(Interpreter& interpreter, std::vector<std::any> arguments) override{
      std::cout << "No implementation of this method available for the 'NativeRandom' class." << std::endl;
      
      return {};
    }

    std::any call(Interpreter& interpreter, Token paren, std::vector<std::any> arguments) override{
      Token functionName{TokenType::IDENTIFIER, "std::random::random", this->toString(), paren.line};
      if(arguments.size() != 2){
        throw BleachRuntimeError{functionName, "Invalid number of arguments. Expected " + std::to_string(arity()) + " arguments but received " + std::to_string(arguments.size()) + " arguments."};
      }

      if(arguments[0].type() != typeid(double) || arguments[1].type() != typeid(double)){
        throw BleachRuntimeError{functionName, "The two arguments of the 'std::random::random' function must be numbers."};
      }

      double left = std::any_cast<double>(arguments[0]);
      double right = std::any_cast<double>(arguments[1]);

      if(left > right){
        throw BleachRuntimeError{functionName, "The first argument cannot be larger than the second argument."};
      }

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

// std::utils::ord
class NativeOrd : public BleachCallable{
  public:
    int arity() override{
      return 1;
    }

    std::any call(Interpreter& interpreter, std::vector<std::any> arguments) override{
      std::cout << "No implementation of this method available for the 'NativeOrd' class." << std::endl;
      
      return {};
    }

   std::any call(Interpreter& interpreter, Token paren, std::vector<std::any> arguments) override{
      Token functionName{TokenType::IDENTIFIER, "std::utils::ord", this->toString(), paren.line};
      if(arguments.size() != 1){
        throw BleachRuntimeError{functionName, "Invalid number of arguments. Expected " + std::to_string(arity()) + " arguments but received " + std::to_string(arguments.size()) + " arguments."};
      }

      if(arguments[0].type() != typeid(std::string)){
        throw BleachRuntimeError{functionName, "Argument of the 'std::utils::ord' function must be a string."};
      }

      std::string str = std::any_cast<std::string>(arguments[0]);
      if(str.length() != 1){
        throw BleachRuntimeError{functionName, "Argument of the 'std::utils::ord' function cannot be a string of length different than 1."};
      }

      return static_cast<double>(static_cast<int>(str[0]));
    }

    std::string toString() override{
      return "<native function: std::utils::ord>";
    }
};

// std::utils::strToNum
class NativeStringToNumber : public BleachCallable{
  public:
    int arity() override{
      return 1;
    }

    std::any call(Interpreter& interpreter, std::vector<std::any> arguments) override{
      std::cout << "No implementation of this method available for the 'NativeStringToNumber' class." << std::endl;
      
      return {};
    }

   std::any call(Interpreter& interpreter, Token paren, std::vector<std::any> arguments) override{
      Token functionName{TokenType::IDENTIFIER, "std::utils::strToNum", this->toString(), paren.line};
      if(arguments.size() != 1){
        throw BleachRuntimeError{functionName, "Invalid number of arguments. Expected " + std::to_string(arity()) + " arguments but received " + std::to_string(arguments.size()) + " arguments."};
      }

      if(arguments[0].type() != typeid(std::string)){
        throw BleachRuntimeError{functionName, "Argument of the 'std::utils::strToNum' function must be a string."};
      }

      std::string str = std::any_cast<std::string>(arguments[0]);
      double answer;

      try{
        answer = std::stod(str);
      }catch(const std::invalid_argument& e){
        throw BleachRuntimeError{functionName, "Argument of the 'std::utils::strToNum' function could not be converted to a number."};
      }catch(const std::out_of_range& e){
        throw BleachRuntimeError{functionName, "Argument of the 'std::utils::strToNum' function overflowed the range of the 'num' type."};
      }

      return answer;
    }

    std::string toString() override{
      return "<native function: std::utils::strToNum>";
    }
};

// std::utils::strToBool
class NativeStringToBool : public BleachCallable{
  public:
    int arity() override{
      return 1;
    }

    std::any call(Interpreter& interpreter, std::vector<std::any> arguments) override{
      std::cout << "No implementation of this method available for the 'NativeStringToBool' class." << std::endl;
      
      return {};
    }

   std::any call(Interpreter& interpreter, Token paren, std::vector<std::any> arguments) override{
      Token functionName{TokenType::IDENTIFIER, "std::utils::strToBool", this->toString(), paren.line};
      if(arguments.size() != 1){
        throw BleachRuntimeError{functionName, "Invalid number of arguments. Expected " + std::to_string(arity()) + " arguments but received " + std::to_string(arguments.size()) + " arguments."};
      }

      if(arguments[0].type() != typeid(std::string)){
        throw BleachRuntimeError{functionName, "Argument of the 'std::utils::strToBool' function must be a number."};
      }

      std::string str = std::any_cast<std::string>(arguments[0]);
      if(str == "true"){
        return true;
      }else if(str == "false"){
        return false;
      }

      throw BleachRuntimeError{functionName, "Could not convert the string value: " + str + " to a bool value."};
    }

    std::string toString() override{
      return "<native function: std::utils::strToBool>";
    }
};

// std::utils::strToNil
class NativeStringToNil : public BleachCallable{
  public:
    int arity() override{
      return 1;
    }

    std::any call(Interpreter& interpreter, std::vector<std::any> arguments) override{
      std::cout << "No implementation of this method available for the 'NativeStringToNil' class." << std::endl;
      
      return {};
    }

   std::any call(Interpreter& interpreter, Token paren, std::vector<std::any> arguments) override{
      Token functionName{TokenType::IDENTIFIER, "std::utils::strToNil", this->toString(), paren.line};
      if(arguments.size() != 1){
        throw BleachRuntimeError{functionName, "Invalid number of arguments. Expected " + std::to_string(arity()) + " arguments but received " + std::to_string(arguments.size()) + " arguments."};
      }

      if(arguments[0].type() != typeid(std::string)){
        throw BleachRuntimeError{functionName, "Argument of the 'std::utils::strToNil' function must be a number."};
      }

      std::string str = std::any_cast<std::string>(arguments[0]);
      if(str == "nil"){
        return nullptr;
      }

      throw BleachRuntimeError{functionName, "Could not convert the string value: " + str + " to the nil value."};
    }

    std::string toString() override{
      return "<native function: std::utils::strToNil>";
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

    std::string formatDouble(double value){
        std::ostringstream out;

        // Check if the value has a fractional part
        if(value == static_cast<int>(value)){
          // If the value is an integer, don't show decimal places
          out << std::fixed << std::setprecision(0) << value;
        }else{
          // If the value has a fractional part, show it with the required precision
          out << std::fixed << std::setprecision(15) << value;

          // Remove trailing zeros
          std::string result = out.str();
          result.erase(result.find_last_not_of('0') + 1, std::string::npos);

          // If the last character is a '.', remove it as well
          if(result.back() == '.'){
            result.pop_back();
          }
          return result;
        }

        return out.str();
    }

    std::string printValue(Interpreter& interpreter, Token functionName, std::any object, bool isInsideList=false){
      if(object.type() == typeid(nullptr)){
        return "nil";
      }
      if(object.type() == typeid(bool)){
        return std::any_cast<bool>(object) ? "true" : "false";
      }
      if(object.type() == typeid(std::string)){
        if(isInsideList){
          return "\"" + std::any_cast<std::string>(object) + "\"";
        }
        return std::any_cast<std::string>(object);
      }
      if(object.type() == typeid(double)){
        double value = std::any_cast<double>(object);
        return formatDouble(value);
      }
      if(object.type() == typeid(std::shared_ptr<BleachClass>)){
        return std::any_cast<std::shared_ptr<BleachClass>>(object)->toString();
      }
      if(object.type() == typeid(std::shared_ptr<BleachFunction>)){
        return std::any_cast<std::shared_ptr<BleachFunction>>(object)->toString();
      }
      if(object.type() == typeid(std::shared_ptr<BleachInstance>)){
        return std::any_cast<std::shared_ptr<BleachInstance>>(object)->toString(interpreter);
      }
      if(object.type() == typeid(std::shared_ptr<BleachLambdaFunction>)){
        return std::any_cast<std::shared_ptr<BleachLambdaFunction>>(object)->toString();
      }
      if(object.type() == typeid(std::shared_ptr<std::vector<std::any>>)){  
        std::shared_ptr<std::vector<std::any>> vecPtr = std::any_cast<std::shared_ptr<std::vector<std::any>>>(object);
        std::string listAsString = "[";

        for(int i = 0; i < vecPtr->size(); i++){
          if(i == vecPtr->size() - 1){
            listAsString += printValue(interpreter, functionName, vecPtr->at(i), true);
          }else{
            listAsString += printValue(interpreter, functionName, vecPtr->at(i), true);
            listAsString += ", ";
          }
        }

        listAsString += "]";

        return listAsString;
      }
      if(object.type() == typeid(std::shared_ptr<NativeClock>)){
        return std::any_cast<std::shared_ptr<NativeClock>>(object)->toString();
      }
      if(object.type() == typeid(std::shared_ptr<NativeReadLine>)){
        return std::any_cast<std::shared_ptr<NativeReadLine>>(object)->toString();
      }
      if(object.type() == typeid(std::shared_ptr<NativePrint>)){
        return std::any_cast<std::shared_ptr<NativePrint>>(object)->toString();
      }
      if(object.type() == typeid(std::shared_ptr<NativeFileRead>)){
        return std::any_cast<std::shared_ptr<NativeFileRead>>(object)->toString();
      }
      if(object.type() == typeid(std::shared_ptr<NativeFileWrite>)){
        return std::any_cast<std::shared_ptr<NativeFileWrite>>(object)->toString();
      }
      if(object.type() == typeid(std::shared_ptr<NativeAbsoluteValue>)){
        return std::any_cast<std::shared_ptr<NativeAbsoluteValue>>(object)->toString();
      }
      if(object.type() == typeid(std::shared_ptr<NativeLogarithm>)){
        return std::any_cast<std::shared_ptr<NativeLogarithm>>(object)->toString();
      }
      if(object.type() == typeid(std::shared_ptr<NativeExponentiation>)){
        return std::any_cast<std::shared_ptr<NativeExponentiation>>(object)->toString();
      }
      if(object.type() == typeid(std::shared_ptr<NativeSquareRoot>)){
        return std::any_cast<std::shared_ptr<NativeSquareRoot>>(object)->toString();
      }
      if(object.type() == typeid(std::shared_ptr<NativeRandom>)){
        return std::any_cast<std::shared_ptr<NativeRandom>>(object)->toString();
      }
      if(object.type() == typeid(std::shared_ptr<NativeOrd>)){
        return std::any_cast<std::shared_ptr<NativeOrd>>(object)->toString();
      }
      if(object.type() == typeid(std::shared_ptr<NativeStringToNumber>)){
        return std::any_cast<std::shared_ptr<NativeStringToNumber>>(object)->toString();
      }

      return "Error in stringify: object type not recognized.";
    }

    std::any call(Interpreter& interpreter, std::vector<std::any> arguments) override{
      std::cout << "No implementation of this method available for the 'NativePrint' class." << std::endl;
      
      return {};
    }

    std::any call(Interpreter& interpreter, Token paren, std::vector<std::any> arguments) override{
      Token functionName{TokenType::IDENTIFIER, "std::io::print", toString(), paren.line};
      for(std::any argument : arguments){
        std::cout << printValue(interpreter, functionName, argument) << " ";
      }

      std::cout << std::endl;

      return nullptr;
    }

    std::string toString() override{
      return "<native function: std::io::print>";
    } 
};