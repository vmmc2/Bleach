#pragma once

#include <any>
#include <map>
#include <memory>
#include <string>
#include <utility>

#include "../error/BleachRuntimeError.hpp"
#include "./Token.hpp"

/**
 * @class Environment
 * 
 * @brief Utility class that stores bindings between variables and their respective values.
 * 
 * The Environment class is responsible for storing (during the whole execution of a program) the bindings 
 * between variables' names, that were declared inside the program, and their respective values. This utility
 * class is very important because it allows the interpreter to "remember" the declared variables.
 * 
 * @note: Pay attention to the fact that the "values" attribute, the "std::map" that stores the bindings between
 * variables and values, uses the "std::string" type as the key type. This is a deliberate decision, because
 * , inside the same scope, the tokens that represent the same identifier must point to the same value. To do
 * that, we use the "lexeme" attribute of the "Token" class. By doing so, we make sure that different tokens
 * with the same lexeme point to the same value, if, and only if, such tokens are in the same scope.
**/
class Environment{
  private:
    std::map<std::string,std::any> values; /**< Variable that stores the bindings between variables' names and their associated values. */
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

    /**
     * @brief Defines a variable and associates a value to it inside the current environment. 
     *
     * This method works as a variable definition method. It essentialy binds the name of the variable to its
     * respective value inside the current environment.
     * 
     * @param name: The name of the variable being defined. Such variable is of type "std::string&".
     * @param value: The value associated with the name of the variable that is being defined. Such variable is
     * of type "std::any".
     * 
     * @return Nothing (void).
     * 
     * @note: Pay attention to the fact that Bleach has made an interesting semantic choice. We allow the user
     * to perform variable redefinition, but only inside the global scope.
     */
    void define(const std::string& name, std::any value){
      values[name] = std::move(value);

      return;
    }

    /**
     * @brief Finds the variable whose name matches with the lexeme of the token (identifier) that has been
     * passed and, then, returns the value that is bound to such variable. 
     *
     * This method works by receiving a token that is an identifier (a variable name) as its unique parameter,
     * and then it searches for the lexeme of such token inside the current environment and inside the enclosing
     * environments (if necessary). When such lexeme is found, it returns the value that is bound to such 
     * variable name.
     * 
     * @param name: The token representing the name of the variable (identifier) whose value we need to retrieve.
     * Such variable is of type "const Token&".
     * 
     * @return The value that is bound to the passed variable's name. The return value is of type "std::any". 
     * 
     * @note: Pay attention to the fact that Bleach has made an interesting semantic choice. If the name of the
     * variable is not found in the current environment and in its enclosing environments, then it means that
     * such variable was not declared. Therefore, a runtime error is thrown.
     */
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