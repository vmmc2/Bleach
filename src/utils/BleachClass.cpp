#include <utility>

#include "./BleachClass.hpp"


BleachClass::BleachClass(std::string name, std::shared_ptr<BleachClass> superclass, std::map<std::string, std::shared_ptr<BleachFunction>> methods)
  : name{std::move(name)}, superclass{std::move(superclass)}, methods{std::move(methods)}
{}

int BleachClass::arity(){
  std::shared_ptr<BleachFunction> initializer = findMethod("init");
  
  if(initializer != nullptr){
    return initializer->arity();
  }

  return 0;
}

std::any BleachClass::call(Interpreter& interpreter, std::vector<std::any> arguments){ // className()
  auto instance = std::make_shared<BleachInstance>(shared_from_this()); // Creates an instance of the class.
  std::shared_ptr<BleachFunction> initializer = findMethod("init"); // Search for the "init" method, which is a constructor. A value of type std::shared_ptr<BleachFunction>.

  if(initializer != nullptr){ // If the initializer (std::shared_ptr<BleachFunction>) of the class has been found.
    initializer->bind(instance)->call(interpreter, std::move(arguments)); // Calling the constructor in the instance.
  }
  
  return instance; // Returning the instance after all this process is executed.
}

std::any BleachClass::call(Interpreter& interpreter, Token paren, std::vector<std::any> arguments){
  std::cout << "No implementation of this method available." << std::endl;
 
  return {};
}

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

std::string BleachClass::toString(){
  return name;
}
