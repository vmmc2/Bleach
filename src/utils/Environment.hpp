#pragma once

#include <any>
#include <map>
#include <memory>
#include <string>
#include <utility>

#include "../error/BleachRuntimeError.hpp"
#include "./Token.hpp"


class Environment{
  private:
    std::map<std::string,std::any> values;
    std::shared_ptr<Environment> enclosing;

  public:
    Environment()
      : enclosing{nullptr}
    {}

    Environment(std::shared_ptr<Environment> enclosing)
      : enclosing{std::move(enclosing)}
    {}

    void assign(const Token& name, std::any value){
      auto elem = values.find(name.lexeme);

      if(elem != values.end()){
        values[name.lexeme] = std::move(value);
        return;
      }

      if(enclosing != nullptr){
        enclosing->assign(name, std::move(value));
        return;
      }

      throw BleachRuntimeError{name, "Undefined variable '" + name.lexeme + "'."};
    }


    void define(const std::string& name, std::any value){
      values[name] = std::move(value);

      return;
    }

    std::any get(const Token& name){
      auto elem = values.find(name.lexeme);

      if(elem != values.end()){
        return values[name.lexeme];
      }
      
      if(enclosing != nullptr){
        return enclosing->get(name);
      }

      throw BleachRuntimeError{name, "Undefined variable '" + name.lexeme + "'."};
    }
};