#include <utility>

#include "./BleachInstance.hpp"
#include "../error/BleachRuntimeError.hpp"


/**
 * @brief Constructs a BleachInstance object. 
 *
 * This constructor initializes a BleachInstance object with the three attributes that were mentioned inside the
 * "BleachInstance.hpp" file.
 *
 * @param klass: The name of the user-define class that has generated an instance of this BleachInstance class. 
**/
BleachInstance::BleachInstance(std::shared_ptr<BleachClass> klass)
  : klass{std::move(klass)}
{}

/**
 * @brief Tries to retrieve the value associated to a property whose name was given as an argument.
 * 
 * This method is responsible for retrieving the value associated to an property. First of all, the interpreter
 * makes the assumption that the property is an attribute/field. This means that it will first search for its
 * name inside the "fields" map present inside the instance of the BleachInstance class. If it does not found
 * such name inside this map, then it tries to find it inside the "methods" map of the instance of the 
 * BleachClass class that has created this instance of the BleachInstance class. If it also does not find this
 * name inside such map, then a runtime error is thrown reporting that such property does not exist inside that
 * instance of the BleachInstance class and also not in the instance of the BleachClass class that created such
 * instance.
 * 
 * @param name: A token that represents the name of the property whose value is required.
 *
 * @return The value associated to the name of the property. If such name is not found, then a runtime error is
 * thrown.
 * 
 * @note: Pay attention to the fact that an interesting choice was made in the implementation of this method 
 * and such choice has semantical consequences. If a class has a method with the name "foo" and an instance of 
 * such class is created and an attribute/field with this same name "foo" is added to this instance, then every
 * time a search for the "foo" property is made, it will return the value associated to the attribute/field. It
 * will never return the method of the class associated with the name "foo". Basically, this means that 
 * attributes/fields shadow methods.
**/
std::any BleachInstance::get(const Token& name){
  // When some property of an instance is accessed, first we check if its a field/attribute.
  auto elem = fields.find(name.lexeme);
  if(elem != fields.end()){
    return elem->second;
  }

  // If that's not the case, then we check whether it's a method from the class of the instance.
  // In short, fields/attributes shadow methods.
  auto method = klass->findMethod(name.lexeme);
  if(method != nullptr){
    return method->bind(shared_from_this());
  }

  throw BleachRuntimeError{name, "Undefined property '" + name.lexeme + "'."};
}

/**
 * @brief Creates a binding between the name of an attribute/field and its respective value in a certain 
 * instance of the BleachInstance class.
 * 
 * This method is responsible for creating a binding between the an attribute/field and its respective value. 
 * Such method is only called when a "Set" expression is evaluated.
 * 
 * @param name: A token that represents the name of the attribute/field to which a value will be assigned to.
 * @param value: The value that will be assigned to the attribute/field of this instance of the BleachInstance
 * class.
 *
 * @return Nothing (void).
**/
void BleachInstance::set(const Token& name, std::any value){
  fields[name.lexeme] = std::move(value);

  return;
}

/**
 * @brief Returns the string representation of an instance of the BleachInstance class.
 * 
 * This method is responsible for returning the string representation of this instance of the BleachInstance
 * class.
 * 
 * @return A string that is the string representation of this instance of the BleachInstance class.
**/
std::string BleachInstance::toString(){
  return "<instance of the " + klass->toString() + " class>";
}
