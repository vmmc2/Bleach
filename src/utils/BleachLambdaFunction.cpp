#include <utility>

#include "./BleachLambdaFunction.hpp"
#include "./Environment.hpp"
#include "../interpreter/Interpreter.hpp"
#include "./Stmt.hpp"

BleachLambdaFunction::BleachLambdaFunction(std::shared_ptr<LambdaFunction> lambdaFunctionDeclaration, std::shared_ptr<Environment> closure)
  : lambdaFunctionDeclaration{std::move(lambdaFunctionDeclaration)}, closure{std::move(closure)}
{}

int BleachLambdaFunction::arity(){
  return lambdaFunctionDeclaration->parameters.size();
}

std::any BleachLambdaFunction::call(Interpreter& interpreter, std::vector<std::any> arguments){
  auto environment = std::make_shared<Environment>(closure); // Create an environment (scope) for the function that is about to be executed. The function environment has as its parent environment the closure that involves it.

  for(int i = 0; i < lambdaFunctionDeclaration->parameters.size(); i++){ // Create the bindings between the parameters of the function and its corresponding arguments, that were passed during the function.
    environment->define(lambdaFunctionDeclaration->parameters[i].lexeme, arguments[i]);
  }

  try{
    interpreter.executeBlock(lambdaFunctionDeclaration->body, environment); // Execute the statements that are present inside the function. Pay attention to the fact that the current environment of the newly created function is passed as an argument to this method.
  }catch(BleachReturn returnValue){ // Caught a return value during the execution of the function. Then, it needs to return such value.
    return returnValue.value;
  }

  return nullptr; // This here is necessary for the case when a function does not have a "return" statement. By default, all user defined functions in Bleach return nil (C++ nullptr).
}

std::any BleachLambdaFunction::call(Interpreter& interpreter, Token paren, std::vector<std::any> arguments){
  std::cout << "No implementation of this method available for the 'BleachLambdaClass'." << std::endl;
 
  return {};
}

std::string BleachLambdaFunction::toString(){
  return "<lambda function>";
}
