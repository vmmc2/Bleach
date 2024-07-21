#include <utility>

#include "./BleachFunction.hpp"
#include "./Environment.hpp"
#include "../interpreter/Interpreter.hpp"
#include "./Stmt.hpp"


BleachFunction::BleachFunction(std::shared_ptr<Function> functionDeclaration, std::shared_ptr<Environment> closure, bool isInitializer)
  : functionDeclaration{std::move(functionDeclaration)}, closure{std::move(closure)}, isInitializer{isInitializer}
{}

int BleachFunction::arity(){
  return functionDeclaration->parameters.size();
}

std::shared_ptr<BleachFunction> BleachFunction::bind(std::shared_ptr<BleachInstance> instance){
  auto environment = std::make_shared<Environment>(closure);
  environment->define("self", instance); // Declaring the variable "self" inside this new environment and defining its value to be equal to the instance that the method is being accessed from.
  
  return std::make_shared<BleachFunction>(functionDeclaration, environment, isInitializer); // Just pass on the original value of "isInitializer" to the newly created "BleachFunction" object.
}

std::any BleachFunction::call(Interpreter& interpreter, std::vector<std::any> arguments){
  auto environment = std::make_shared<Environment>(closure); // Create an environment (scope) for the function that is about to be executed. The function environment has as its parent environment the closure that involves it.

  for(int i = 0; i < functionDeclaration->parameters.size(); i++){ // Create the bindings between the parameters of the function and its corresponding arguments, that were passed during the function.
    environment->define(functionDeclaration->parameters[i].lexeme, arguments[i]);
  }

  try{
    interpreter.executeBlock(functionDeclaration->body, environment); // Execute the statements that are present inside the function. Pay attention to the fact that the current environment of the newly created function is passed as an argument to this method.
  }catch(BleachReturn returnValue){ // Caught a return value during the execution of the function. Then, it needs to return such value.
    if(isInitializer){
      return closure->getAt("self", 0); // Earlier empty return ("return;") from a constructor of a class.
    }
    return returnValue.value;
  }

  if(isInitializer){ // If the function is a constructor ("init" method), then it will always (implicitly) return "self".
    return closure->getAt("self", 0); // Remember that the binding between "self" and its corresponding instance is stored in the "closure" environment.
  }

  return nullptr; // This here is necessary for the case when a function does not have a "return" statement. By default, all user defined functions in Bleach return nil (C++ nullptr).
}

std::string BleachFunction::toString(){
  return "<function " + functionDeclaration->name.lexeme + ">";
}
