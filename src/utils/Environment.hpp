#pragma once

#include <any>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <utility>

#include "../error/Error.hpp"
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
 * variables and values, uses the "std::string" type as the key type. This is a deliberate decision, because,
 * inside the same scope, the tokens that represent the same identifier must point to the same value. To do
 * that, we use the "lexeme" attribute of the "Token" class. By doing so, we make sure that different tokens
 * with the same lexeme point to the same value, if, and only if, such tokens are in the same scope.
**/
class Environment : public std::enable_shared_from_this<Environment>{
  private:
    friend class Interpreter;

    std::map<std::string,std::any> values; /**< Variable that stores the bindings between variables' names and their associated values. */
    std::shared_ptr<Environment> enclosing; /**< Variable that points to its enclosing environment (the "parent" environment of this environment). */

  public:
    /**
     * @brief Constructs an Environment with no enclosing environment. 
     *
     * This constructor initializes an Environment object with no arguments (no enclosing environment). This
     * constructor is used to the creation of the global environment, since it does not have any enclosing or 
     * parent environment. Therefore, such constructor assigns nullptr to the "enclosing" attribute.
    **/
    Environment()
      : enclosing{nullptr}
    {}

    /**
     * @brief Constructs an Environment with an enclosing environment. 
     *
     * This constructor initializes an Environment object with only one argument (its enclosing environment, which
     * is another instance of an Environment class).
     *
     * @param enclosing: The pointer that points to the enclosing enviroment of the current Environment object
     * that is being created.
    **/
    Environment(std::shared_ptr<Environment> enclosing)
      : enclosing{std::move(enclosing)}
    {}

    /**
     * @brief Returns an ancestor (enclosing) environment given its distance with respect to the current 
     * environment. 
     *
     * This method is responsible for receiving an integer "distance" that represents the amount of hops that
     * must be followed from the current instance of the Environment class in its environment chain in order to 
     * retrieve a desired Environment instance. Such desired Environment instanec is what will be returned by 
     * this method.
     * 
     * @param distance: The distance (amount of hops) in the chain of environments between the current 
     * environment that the interpreter is on and the environment that needs to be retrieved.
     * 
     * @return A pointer that points to the enclosing enviroment that was required given the value to the 
     * "distance" parameter of this method.
    **/
    std::shared_ptr<Environment> ancestor(int distance){
      std::shared_ptr<Environment> environment = shared_from_this();
      for(int i = 0; i < distance; i++){
        environment = environment->enclosing;
      }

      return environment;
    }

    /**
     * @brief Assigns the received value to the variable which the lexeme of the received token refers to. 
     *
     * This method works as a variable assignment method. It essentialy assigns the new received value to the
     * variable which the lexeme of received token refers to. Pay attention to the fact that, to find out which
     * variable such lexeme is refering to, it starts searching for its lexeme in the current environment and
     * it goes from enclosing environment to enclosing environment, until it finds the variable in an 
     * environment or reaches the global scope.
     * 
     * @param name: The token whose lexeme represents the name of the variable which the passed value is going 
     * to be assigned to.
     * @param value: The value which will be assigned to the variable whose the parameter "name" is refering
     * to.
     * 
     * @return Nothing (void).
     * 
     * @note: Pay attention to the fact that this search starts on the current instance of the Environment class
     * that the interpreter is on but goes on following the chains of environments until it finds the lexeme of 
     * the variable we are trying to assign a value to. However, if this method traverses the whole chain of 
     * environments and doesn't find the lexeme of the variable to which the received token is refering to, 
     * then it means such variable was never declared by the user in the first place. Thus, an instance of the 
     * BleachRuntimeError class is thrown by the method.
     */
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
     * @brief Assigns, at a specific environment (determined by the received value of the "distance" parameter), 
     * the received value to the variable which the lexeme of the received token refers to. 
     *
     * This method works exactly as the "assign" method presented above. The only difference between the 
     * this method and the "assign" method is that the former performs an assignment operation inside a specific
     * Environment instance. Such instance is found given the value passed to the "distance" parameter of this 
     * method.
     * 
     * @param name: The token that contains the lexeme of the variable which the passed value is going to be 
     * assigned to.
     * @param value: The value which will be bound to the lexeme of the token that represents the variable 
     * whose the parameter "name" is refering to.
     * @param distance: The integer that represents the distance between the current Environment instance that 
     * the interpreter is on and the desired Environment instance in which the assignment operation must be 
     * performed.
     * 
     * @return Nothing (void).
     */
    void assignAt(const Token& name, std::any value, int distance){
      ancestor(distance)->values[name.lexeme] = std::move(value);

      return;
    }

    /**
     * @brief Defines a variable and associates a value to it inside the current environment. 
     *
     * This method works as a variable definition method. It essentialy binds the name of the variable to its
     * respective value inside the current environment.
     * 
     * @param name: The name of the variable being defined.
     * @param value: The value associated with the name of the variable that is being defined.
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
     * @brief Tries to find the variable whose name matches with the lexeme of the token (identifier) that has 
     * been passed and, then, returns the value that is bound to such variable. If such variable name is not
     * found, then this method throws an instance of BleachRuntimeError.
     *
     * This method works by receiving a token that is an identifier (a variable name) as its unique parameter,
     * and then it searches for the lexeme of such token inside the current environment and inside the enclosing
     * environments (if necessary). When such lexeme is found, it returns the value that is bound to such 
     * variable name.
     * 
     * @param name: The token representing the name of the variable (identifier) whose value needs to be 
     * retrieved.
     * 
     * @return The value that is bound to the lexeme of the token that represents the variable name. 
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

    /**
     * @brief Tries to find, at a specific environment (determined by the received value of the "distance" 
     * parameter), the variable whose name matches with the lexeme of the token (identifier) that has been
     * passed and, then, returns the value that is bound to such variable. 
     *
     * This method works exactly as the "get" method presented above. The only difference between this method 
     * and the "get" method is that the former works by receiving not only a token that is an identifier (a 
     * variable name) but also an integer that tells in how many hops in the environment chain will need to be 
     * made in order to get to the environment where the desired variable is. Then, at that environment, it 
     * searches for the lexeme of such token. When such lexeme is found, it returns the value that is bound to 
     * it.
     * 
     * @param name: The token representing the name of the variable (identifier) whose value we need to 
     * retrieve.
     * @param distance: The integer that tells how many hops will need to be made in order to get to the 
     * environment where the desired variable is.
     * 
     * @return The value that is bound to the passed variable's name. 
     */
    std::any getAt(const std::string&name, int distance){
      return ancestor(distance)->values[name];
    }
};
