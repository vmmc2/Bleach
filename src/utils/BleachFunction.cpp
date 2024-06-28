#include <utility>

#include "./BleachFunction.hpp"
#include "./Environment.hpp"
#include "../interpreter/Interpreter.hpp"
#include "./Stmt.hpp"


BleachFunction::BleachFunction(std::shared_ptr<Function> functionDeclaration)
  : functionDeclaration{std::move(functionDeclaration)}
{}

int BleachFunction::arity(){
  return functionDeclaration->parameters.size();
}

std::any BleachFunction::call(Interpreter& interpreter, std::vector<std::any> arguments){
  auto environment = std::make_shared<Environment>(interpreter.globals); // Create an environment (scope) for the function that is about to be executed.

  for(int i = 0; i < functionDeclaration->parameters.size(); i++){ // Create the bindings between the parameters of the function and its corresponding arguments, that were passed during the function.
    environment->define(functionDeclaration->parameters[i].lexeme, arguments[i]);
  }

  interpreter.executeBlock(functionDeclaration->body, environment); // Execute the statements that are present inside the function. Pay attention to the fact that the current environment of the newly created function is passed as an argument to this method.

  return nullptr;
}

std::string BleachFunction::toString(){
  return "<function " + functionDeclaration->name.lexeme + ">";
}