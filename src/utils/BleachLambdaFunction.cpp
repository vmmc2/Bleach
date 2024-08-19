#include <utility>

#include "./BleachLambdaFunction.hpp"
#include "./Environment.hpp"
#include "../interpreter/Interpreter.hpp"
#include "./Stmt.hpp"


/**
 * @brief Constructs a BleachLambdaFunction object. 
 *
 * This constructor initializes a BleachLambdaFunction object with the two attributes that were mentioned inside
 * the "BleachLambdaFunction.hpp" file.
 *
 * @param closure: The environment that is the nearest enclosing environment of the generated 
 * BleachLambdaFunction object (represented by the std::shared_ptr<Environment> type).
 * @param lambdaFunctionDeclaration: The expression that represented the generated BleachLambdaFunction object
 * during static time. Essentialy, it is the AST node that was produced when parsing the declaration of the 
 * lambda (anonymous) function.
**/
BleachLambdaFunction::BleachLambdaFunction(std::shared_ptr<LambdaFunction> lambdaFunctionDeclaration, std::shared_ptr<Environment> closure)
  : lambdaFunctionDeclaration{std::move(lambdaFunctionDeclaration)}, closure{std::move(closure)}
{}

/**
 * @brief Returns the arity (amount of the parameters expected) when calling the BleachLambdaFunction object
 * during runtime.
 * 
 * This method is responsible for returning the arity (amount of parameters expected) when calling an instance
 * of the BleachLambdaFunction class during runtime. To verify the arity of the anonymous/lambda function, it
 * checks the corresponding declaration of the BleachLambdaFunction object that was parsed and returns the 
 * amount of parameters present inside such declaration.
 * 
 * @return An integer that denotes the arity (the amount of parameters expected by the anonymous/lambda 
 * function).
**/
int BleachLambdaFunction::arity(){
  return lambdaFunctionDeclaration->parameters.size();
}

/**
 * @brief Executes the instance of the BleachLambdaFunction and returns the return value of this object, if any.
 * 
 * This method is responsible for executing the instance of the BleachLambdaFunction during runtime an returning
 * its return value, if any.
 * 
 * @return The value returned by the execution of the BleachLambdaFunction object during runtime. If the 
 * declaration of the LambdaFunction AST node has no return statement, then during runtime, the execution of
 * the BleachLambdaFunction object (triggered by calling this method) will return a nullptr (nil) value.
**/
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

/**
 * @note: This overloaded version of the 'call' method is restricted to be used when calling Bleach native 
 * functions. It won't be called by an instance of a BleachLambdaFunction.
**/
std::any BleachLambdaFunction::call(Interpreter& interpreter, Token paren, std::vector<std::any> arguments){
  std::cout << "No implementation of this method available for the 'BleachLambdaFunction' class." << std::endl;
 
  return {};
}

/**
 * @brief Returns the string representation of an instance of the BleachLambdaFunction class.
 * 
 * This method is responsible for returning the string representation of an instance of the BleachLambdaFunction
 * class.
 * 
 * @return A string (std::string) that is the string representation of an instance of the BleachLambdaFunction
 * class.
**/
std::string BleachLambdaFunction::toString(){
  return "<lambda function>";
}
