#include <utility>

#include "./BleachFunction.hpp"
#include "./Environment.hpp"
#include "../interpreter/Interpreter.hpp"
#include "./Stmt.hpp"


/**
 * @brief Constructs a BleachFunction object. 
 *
 * This constructor initializes a BleachFunction object with the three attributes that were mentioned inside the
 * "BleachFunction.hpp" file.
 *
 * @param isInitializer: A boolean that signals whether or not the instance of the BleachFunction is a 
 * constructor of a class, also known as the "init" method.
 * @param closure: A pointer that points to an instance of the nearest enclosing instance of the Environment
 * class.
 * @param functionDeclaration: A pointer that points to an instance of the Function class, which is the 
 * corresponding static time representation of this instance of the BleachFunction class. In order words, the
 * "functionDeclaration" attribute is a pointer to the AST node that represents its corresponding function 
 * declaration statement node.
**/BleachFunction::BleachFunction(std::shared_ptr<Function> functionDeclaration, std::shared_ptr<Environment> closure, bool isInitializer)
  : functionDeclaration{std::move(functionDeclaration)}, closure{std::move(closure)}, isInitializer{isInitializer}
{}

/**
 * @brief Returns the arity (amount of the arguments expected) when calling the BleachFunction object during 
 * runtime.
 * 
 * This method is responsible for returning the arity (amount of arguments expected) when calling an instance
 * of the BleachFunction class during runtime. To verify the arity of the user-defined function, it checks the 
 * corresponding declaration of the BleachFunction object that was parsed and returns the amount of parameters 
 * present inside such declaration.
 * 
 * @return An integer that denotes the arity (the amount of arguments expected) for this function.
**/
int BleachFunction::arity(){
  return functionDeclaration->parameters.size();
}

/**
 * @brief Receives an instance of the BleachInstance class and returns a new instance of this same BleachFunction
 * class. The only difference, is that this new instance of the BleachFunction has an updated environment. This
 * environment contains a binding between the "self" name and the received instance of the BleachInstance class. 
 * 
 * First of all, it's important to mention that this method will only be used by instances of the BleachFunction
 * class that are representing methods. This method is responsible for receiving an instance of the 
 * BleachInstance class, creating a new Environment whose closure is the same of this instance of the 
 * BleachFunction class, then, in this new environment, it creates a binding between the name "self" and the
 * instance of the BleachInstance class that was received as an argument. Finally, it returns a new instance
 * of a BleachFunction class whose environment is this new one that was created.
 * 
 * @param instance: A pointer to an instance of the BleachInstance class.
 * 
 * @return A pointer to the newly created instance of the BleachFunction class.
**/
std::shared_ptr<BleachFunction> BleachFunction::bind(std::shared_ptr<BleachInstance> instance){
  auto environment = std::make_shared<Environment>(closure);
  environment->define("self", instance); // Declaring the variable "self" inside this new environment and defining its value to be equal to the instance that the method is being accessed from.
  
  return std::make_shared<BleachFunction>(functionDeclaration, environment, isInitializer); // Just pass on the original value of "isInitializer" to the newly created "BleachFunction" object.
}

/**
 * @brief Executes the instance of the BleachFunction class and returns whatever value the user-defined function
 * returns when executed.
 * 
 * This method is responsible for executing an instance of the BleachFunction class during runtime. The 
 * execution of a BleachFunction object essentialy means the execution of an user-defined function or method 
 * during runtime. This method will return whatever value the user-defined function or method returns. If 
 * there's no return statement inside the user-defined function or method declaration AST node stored in the 
 * instance of this class, then such function or method implicitly returns a nullptr (nil value in Bleach).
 * 
 * @param interpreter: The reference to the instance of the Interpreter class that is running the Bleach file.
 * @param arguments: The list of arguments that are expected to be received by the user-defined function or 
 * method during runtime.
 
 * @return The corresponding value that the user-defined function or method is supposed to return.
**/
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

/**
 * @note: This overloaded version of the 'call' method is restricted to be used when calling Bleach native 
 * functions. It won't be called by an instance of a BleachFunction class.
**/
std::any BleachFunction::call(Interpreter& interpreter, Token paren, std::vector<std::any> arguments){
 std::cout << "No implementation of this method available for the 'BleachFunction' class." << std::endl;
 
  return {};
}

/**
 * @brief Returns the string representation of an instance of the BleachFunction class.
 * 
 * This method is responsible for returning the string representation of an instance of the BleachFunction class.
 * 
 * @return A string that is the string representation of an instance of the BleachFunction class.
**/
std::string BleachFunction::toString(){
  return "<function " + functionDeclaration->name.lexeme + ">";
}
