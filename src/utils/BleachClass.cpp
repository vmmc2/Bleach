#include <utility>

#include "./BleachClass.hpp"


/**
 * @brief Constructs a BleachClass object. 
 *
 * This constructor initializes a BleachClass object with the three attributes that were mentioned inside the
 * "BleachClass.hpp" file.
 *
 * @param name: The name of the user-defined class (represented by the std::string type).
 * @param superclass: The superclass from which the class inherits methods. Pay attention to the fact that not
 * every class has a superclass (represented by the std::shared_ptr<BleachClass> type).
 * @param methods: The map that stores key-value pairs where, in each pair, the key is the name of a method that
 * was declared inside the class declaration and the value is the BleachFunction that represents such method
 * during runtime (represented by the std::map<std::string, std::shared_ptr<BleachFunction>> type).
**/
BleachClass::BleachClass(std::string name, std::shared_ptr<BleachClass> superclass, std::map<std::string, std::shared_ptr<BleachFunction>> methods)
  : name{std::move(name)}, superclass{std::move(superclass)}, methods{std::move(methods)}
{}

/**
 * @brief Returns the arity of the instance from the BleachClass class. 
 *
 * This method is responsible for returning the arity of the instance from the BleachClass class. It's 
 * important to mention that, when dealing with classes, its arity represents the number of arguments
 * expected by its constructor.
 *
 * @return An integer that represents the arity (number of arguments expected by the constructor) for this 
 * class (represented by the int type).
 * 
 * @note: Pay attention to the fact that, if the user-defined class has no "init" method present inside its
 * declaration, then, by default, this method returns 0. Essentialy, this means that the default constructor of
 * every class doesn't expect any arguments.
**/
int BleachClass::arity(){
  std::shared_ptr<BleachFunction> initializer = findMethod("init");
  
  if(initializer != nullptr){
    return initializer->arity();
  }

  return 0;
}

/**
 * @brief Executes the instance of the BleachClass class and returns an instance of the BleachInstance class.
 * 
 * This method is responsible for executing the instance of the BleachClass during runtime. The execution of
 * a BleachClass object essentialy means that an instance of that BleachClass, an object of type BleachInstance,
 * will be created and returned.
 * 
 * @param interpreter: The reference to the instance of the Interpreter class that is running the Bleach file
 * (represented by the Interpreter& type).
 * @param arguments: The list of arguments that are needed to create an instance of the given BleachClass (an
 * instance of the BleachInstance class). Making an analogy, such list of arguments is the the list of arguments
 * provided to the constructor of a class (represented by the std::vector<std::any> type).
 
 * @return An instance of the BleachInstance class. Such instance represents the object that was created given
 * the list of arguments to its constructor (which is, behind the scenes, this method) (represented by the 
 * std::any type).
**/
std::any BleachClass::call(Interpreter& interpreter, std::vector<std::any> arguments){ // className()
  auto instance = std::make_shared<BleachInstance>(shared_from_this()); // Creates an instance of the class.
  std::shared_ptr<BleachFunction> initializer = findMethod("init"); // Search for the "init" method, which is a constructor. A value of type std::shared_ptr<BleachFunction>.

  if(initializer != nullptr){ // If the initializer (std::shared_ptr<BleachFunction>) of the class has been found.
    initializer->bind(instance)->call(interpreter, std::move(arguments)); // Calling the constructor in the instance.
  }
  
  return instance; // Returning the instance after all this process is executed.
}

/**
 * @note: This overloaded version of the 'call' method is restricted to be used when calling Bleach native 
 * functions. It won't be called by an instance of a BleachClass class.
**/
std::any BleachClass::call(Interpreter& interpreter, Token paren, std::vector<std::any> arguments){
  std::cout << "No implementation of this method available for the 'BleachClass' class." << std::endl;
 
  return {};
}

/**
 * @brief Searches and returns the runtime representation of a method, given its name, inside the class and, if
 * needed, inside the chain of superclasses of the class. If such runtime representation is not found, this
 * method returns a nullptr.
 * 
 * This method is responsible for receiving a string representing the name of a possible method of a class.
 * Then, this method searches for the required method in its "methods" map and returns the runtime 
 * representation of the method (a BleachFunction). If the method is not found in the "methods" map of the 
 * class, then the search continues in a recursive manner following the chain of superclasses, if any. If the 
 * method is not found in the class and not in the chain of superclasses, then it returns a nullptr.
 * 
 * @param name: The name of the required method (represented by the const std::string& type).
 * 
 * @return The runtime representation of the method that is associated to the string "name" inside the "methods"
 * map of the class or inside the "methods" map inside any of the classes along the chain of superclasses (if
 * any) (represented by the std::shared_ptr<BleachFunction> type).
 * 
 * @note: If the runtime representation of the method whose name was received as an argument is not found inside
 * the "methods" map of the class or inside the "methods" map inside any of the classes along the chain of
 * superclasses (if any), then this method returns a nullptr.
**/
std::shared_ptr<BleachFunction> BleachClass::findMethod(const std::string& name){
  // First, we search for the method inside the current class.
  auto elem = methods.find(name);
  if(elem != methods.end()){
    return elem->second;
  }

  // If the method is not found, then, maybe it's inside of some superclass of the current class.
  if(superclass != nullptr){
    return superclass->findMethod(name);
  }

  return nullptr;
}

/**
 * @brief Returns the string representation of an instance of the BleachClass class.
 * 
 * This method is responsible for returning the string representation of an instance of the BleachClass class.
 * 
 * @return A string that is the string representation of an instance of the BleachClass class (represented by
 * the std::string type).
**/
std::string BleachClass::toString(){
  return "<class " + name + ">";
}
