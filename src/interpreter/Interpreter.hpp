#pragma once

#include <any>
#include <cmath>
#include <functional>
#include <iomanip>
#include <memory>
#include <sstream>
#include <string>
#include <typeinfo>
#include <utility>
#include <vector>

#include "../utils/BleachBreak.hpp"
#include "../utils/BleachCallable.hpp"
#include "../utils/BleachClass.hpp"
#include "../utils/BleachContinue.hpp"
#include "../utils/BleachInstance.hpp"
#include "../utils/BleachLambdaFunction.hpp"
#include "../utils/BleachFunction.hpp"
#include "../utils/BleachReturn.hpp"
#include "../error/BleachRuntimeError.hpp"
#include "../error/Error.hpp"
#include "../utils/Environment.hpp"
#include "../utils/Expr.hpp"
#include "../utils/NativeFunctions.hpp"
#include "../utils/Stmt.hpp"


/**
 * @class Interpreter
 * 
 * @brief Performs the interpreting stage of the BLEACH Interpreter.
 * 
 * The Interpreter class is responsible for traversing the generated Bleach AST in a post-order manner and
 * evaluating and executing each node present in such AST. Such behavior is the program execution. In other
 * words, it's the final step of the interpreting pipeline.
 *
 * @note If the BLEACH Interpreter encounters an error during the traversal of the AST (program execution), then
 * it means that it has found a runtime error and, therefore, such traversal will be stopped and the error will
 * be reported to the user.
**/
class Interpreter : public ExprVisitor, public StmtVisitor{
  friend class BleachFunction;

  public:
    std::shared_ptr<Environment> globals{new Environment}; /**< Variable that always points to the outermost global environment (global scope). */
  private:
    std::shared_ptr<Environment> environment = globals; /**< Variable that tracks the current environment of the interpreter instance. Its value changes during execution as the interpreter enters and exits local scopes. */
    std::map<std::shared_ptr<Expr>, int> locals; /**< Variable that holds information about how many environments must be traveled between the current one and the enclosing one where the interpreter can find the variable's value. */

    /**
     * @brief Checks whether the provided operand of the unary operator ("-") is a value of type double. 
     *
     * This method is responsible for checking whether the operand of the unary operator ("-") is of type 
     * double.
     * 
     * @param op: The token that represents the unary operator ("-") (TokenType::MINUS).
     * @param operand: The value of the right operand of the unary operator ("-").
     * 
     * @return Nothing (void).
     * 
     * @note If the provided operand does not satisfy the conditions explained above, then an instance of a 
     * BleachRuntimeError is thrown by the interpreter.
     */
    void checkNumberOperand(const Token& op, const std::any& operand){
      if(operand.type() == typeid(double)){
        return;
      }

      throw BleachRuntimeError{op, "Operand must be a number."};
    }

    /**
     * @brief Checks whether the provided operands of the following binary operators ("-", "*", "/", ">",
     * ">=", "<", "<=") are values of type double.
     *
     * This method is responsible for checking whether the operands of the following binary operators ("-", 
     * "*", "/", ">", ">=", "<", "<=") are of type double.
     * 
     * @param left: The value of the left operand of the binary operator.
     * @param right: The value of the right operand of the binary operator.
     * 
     * @return A bool.
     */
    bool checkNumberOperands(const std::any& left, const std::any& right){
      if(left.type() == typeid(double) && right.type() == typeid(double)){
        return true;
      }else{
        return false;
      }
    }

    /**
     * @brief Checks whether the provided operands of the following binary operators (">", ">=", "<", "<=")
     * are values of type std::string.
     *
     * This method is responsible for checking whether the operands of the following binary operators (">",
     * ">=", "<", "<=") are of type std::string.
     * 
     * @param left: The value of the left operand of the binary operator.
     * @param right: The value of the right operand of the binary operator.
     * 
     * @return A bool.
     */
    bool checkStringOperands(const std::any& left, const std::any& right){
      if(left.type() == typeid(std::string) && right.type() == typeid(std::string)){
        return true;
      }else{
        return false;
      }
    }

    /**
     * @brief Checks whether the provided divisor of a division operation is zero or not. 
     *
     * This method works as a helper method for the "visitBinaryExpr" method specifically in the case where the
     * operator is a division operator ("/"). This method is responsible for checking whether or not the divisor
     * of the division operator is zero. If it is, then it throws a runtime error telling the user that a division
     * by zero is not allowed. 
     * 
     * @param divisor: The value that is the divisor operand of the division operator ("/").
     * @param op: The token whose lexeme represents the division operator ("/").
     * @param epsilon: A value that is used to check whether or not the divisor operand is zero.
     * 
     * @return A boolean that signals whether or not the provided divisor operand of a division operation is zero.
     */
    void checkZeroDivisor(const std::any& divisor, const Token& op, double epsilon = 1e-10){
      double div = std::any_cast<double>(divisor);

      if(std::fabs(div) < epsilon){
        throw BleachRuntimeError{op, "The divisor of a division cannot be 0."};
      }

      return;
    }

    /**
     * @brief Works as a helper method that simply sends back an Expr AST node back into the appropriate visit
     * method of the interpreter. 
     *
     * This method works as a helper method responsible for receiving a Expr node of the AST, calling its 
     * accept method and passing the current instance of the interpreter to it, so the node can call its 
     * appropriate visit method and evaluate its "inner" result.
     * 
     * @param expr: A node of the AST (Abstract Syntax Tree) that represents an Expr node from the Bleach
     * language.
     * 
     * @return The value obtained from the evaluation of the AST node that was passed to this method as its
     * argument.
     */
    std::any evaluate(std::shared_ptr<Expr> expr){
      return expr->accept(*this);
    }

    /**
     * @brief Works as a helper method that simply sends back the Stmt AST node back into the appropriate visit
     * method of the interpreter. 
     *
     * This method works as a helper method responsible for receiving a Stmt node of the AST, calling its 
     * accept method and passing the current instance of the interpreter to it, so the node can call its 
     * appropriate visit method and execute its "inner" functionality.
     * 
     * @param stmt: A node of an AST (Abstract Syntax Tree) that represents a Stmt node from the Bleach 
     * language.
     * 
     * @return Nothing (void).
     */
    void execute(std::shared_ptr<Stmt> stmt){
      stmt->accept(*this);

      return;
    }

    /**
     * @brief Checks whether the provided operands of the following operator ("==") are equal in value. 
     *
     * This method is responsible for checking whether the operands of the following binary operator ("==") 
     * are of the same type. If that's the case, then the method also checks whether or not such operands have
     * the same value.
     * 
     * @param left: The value of the left operand of the "==" operator.
     * @param right: The value of the right operand of the "==" operator.
     * 
     * @return A boolean that signal whether the values of the two provided operands are equal or not.
     */
    bool isEqual(const std::any& left, const std::any& right){
      if(left.type() == typeid(nullptr) && right.type() == typeid(nullptr)){
        return true;
      }
      if((left.type() != typeid(nullptr) && right.type() == typeid(nullptr)) || (left.type() == typeid(nullptr) && right.type() != typeid(nullptr))){
        return false;
      }
      if(left.type() == typeid(bool) && right.type() == typeid(bool)){
        return std::any_cast<bool>(left) == std::any_cast<bool>(right);
      }
      if(left.type() == typeid(double) && right.type() == typeid(double)){
        return std::any_cast<double>(left) == std::any_cast<double>(right);
      }
      if(left.type() == typeid(std::string) && right.type() == typeid(std::string)){
        return std::any_cast<std::string>(left) == std::any_cast<std::string>(right);
      }
      // #TODO: Extend this to deal with 'lists' and 'dicts'.

      return false;
    }

    /**
     * @brief Checks whether the value of the a Bleach object is considered "truthy" or not.
     *
     * This method is responsible for checking whether the value of the provided Bleach object is considered
     * "truthy" or not. 
     * For more context, Bleach follows Ruby convention: false and nil are considered to be "falsey" values. 
     * Every other value is considered to be "truthy".
     * 
     * @param object: The value of a Bleach object that will be checked to see if it's "truthy" or not.
     * 
     * @return A boolean that signal whether or not the provided value is considered "truthy" or not.
     */
    bool isTruthy(const std::any& object){
      if(object.type() == typeid(nullptr)){
        return false;
      }
      if(object.type() == typeid(bool)){
        return std::any_cast<bool>(object);
      }

      return true;
    }

    /**
     * @brief Formats a value of number type according to the presence, absence and amount of decimal places in
     * it.
     *
     * This method is responsible for formatting a value of number type according to certain criteria. Such
     * criteria is whether or not the number has decimal place and the amount of decimal places.
     * For example: If internally a value of number type is "4.0000", then, when printed, the user should see just
     * "4". The same case applies to the following value "3.1415900". The user should see the value "3.14159".
     * However, if a value of number type is "2.713575", then the user should indeed see the "2.713575" value.
     * 
     * @param value: A value whose type is number and will be formatted by this method.
     * 
     * @return A string whose value is a string representation of the formatted number value.
     */
    std::string formatDouble(double value){
        std::ostringstream out;

        // Check if the value has a fractional part
        if(value == static_cast<int>(value)){
          // If the value is an integer, don't show decimal places
          out << std::fixed << std::setprecision(0) << value;
        }else{
          // If the value has a fractional part, show it with the required precision
          out << std::fixed << std::setprecision(15) << value;

          // Remove trailing zeros
          std::string result = out.str();
          result.erase(result.find_last_not_of('0') + 1, std::string::npos);

          // If the last character is a '.', remove it as well
          if(result.back() == '.'){
            result.pop_back();
          }
          return result;
        }

        return out.str();
    }

    /**
     * @brief Retrieves the value bounded to a specific variable given the token whose lexeme is equal to such
     * variable's name.
     *
     * This method is responsible for receiving a token whose lexeme represents the name of a variable and also
     * an expression that represents the AST node of such variable during the parsing phase.
     * 
     * @param name: A token whose lexeme is the name of a variable whose value the interpreter is trying
     * retrieve.
     * @param expr: An expression that represents the AST node of such variable in the parsing stage.
     * 
     * @return The value that is bound to the variable that has been requested.
     */
    std::any lookUpVariable(const Token& name, std::shared_ptr<Expr> expr){
      auto elem = locals.find(expr);
      if(elem != locals.end()){ // If the Variable expression has been found here, then it means that it is a local variable.
        int distance = elem->second;
        return environment->getAt(name.lexeme, distance);
      }else{ // Otherwise, it is assumed that the variable was declared in the global scope.
        return globals->get(name); // Global variable are treated in a special way. If a global variable is not found, the a runtime error is thrown by the BLEACH Interpreter.
      }
    }

  public:
    /**
     * @brief Produces a string that works as a representation of the value present in the provided Bleach 
     * object. 
     *
     * This method generates a string representation of the value present inside the provided Bleach object.
     * 
     * @param object: A variable that stores the value of a Bleach object which the interpreter will try to 
     * convert to its string representation.
     * 
     * @return A string representation of the value present inside the provided Bleach object.
     * 
     * @note If the value of the Bleach object is not of any of the supported types, then this function will 
     * return a string containing an error message.
     */
    std::string stringify(const std::any& object, bool isInsideList = false){
      if(object.type() == typeid(nullptr)){
        return "nil";
      }
      if(object.type() == typeid(bool)){
        return std::any_cast<bool>(object) ? "true" : "false";
      }
      if(object.type() == typeid(std::string)){
        if(isInsideList){
          return "\"" + std::any_cast<std::string>(object) + "\"";
        }
        return std::any_cast<std::string>(object);
      }
      if(object.type() == typeid(double)){
        double value = std::any_cast<double>(object);
        return formatDouble(value);
      }
      if(object.type() == typeid(std::shared_ptr<BleachClass>)){
        return std::any_cast<std::shared_ptr<BleachClass>>(object)->toString();
      }
      if(object.type() == typeid(std::shared_ptr<BleachFunction>)){
        return std::any_cast<std::shared_ptr<BleachFunction>>(object)->toString();
      }
      if(object.type() == typeid(std::shared_ptr<BleachInstance>)){
        return std::any_cast<std::shared_ptr<BleachInstance>>(object)->toString(*this);
      }
      if(object.type() == typeid(std::shared_ptr<BleachLambdaFunction>)){
        return std::any_cast<std::shared_ptr<BleachLambdaFunction>>(object)->toString();
      }
      if(object.type() == typeid(std::shared_ptr<std::vector<std::any>>)){  
        std::shared_ptr<std::vector<std::any>> vecPtr = std::any_cast<std::shared_ptr<std::vector<std::any>>>(object);
        std::string listAsString = "[";

        for(int i = 0; i < vecPtr->size(); i++){
          if(i == vecPtr->size() - 1){
            listAsString += stringify(vecPtr->at(i), true);
          }else{
            listAsString += stringify(vecPtr->at(i), true);
            listAsString += ", ";
          }
        }

        listAsString += "]";

        return listAsString;
      }
      if(object.type() == typeid(std::shared_ptr<NativeClock>)){
        return std::any_cast<std::shared_ptr<NativeClock>>(object)->toString();
      }
      if(object.type() == typeid(std::shared_ptr<NativeReadLine>)){
        return std::any_cast<std::shared_ptr<NativeReadLine>>(object)->toString();
      }
      if(object.type() == typeid(std::shared_ptr<NativePrint>)){
        return std::any_cast<std::shared_ptr<NativePrint>>(object)->toString();
      }
      if(object.type() == typeid(std::shared_ptr<NativeFileRead>)){
        return std::any_cast<std::shared_ptr<NativeFileRead>>(object)->toString();
      }
      if(object.type() == typeid(std::shared_ptr<NativeFileWrite>)){
        return std::any_cast<std::shared_ptr<NativeFileWrite>>(object)->toString();
      }
      if(object.type() == typeid(std::shared_ptr<NativeAbsoluteValue>)){
        return std::any_cast<std::shared_ptr<NativeAbsoluteValue>>(object)->toString();
      }
      if(object.type() == typeid(std::shared_ptr<NativeLogarithm>)){
        return std::any_cast<std::shared_ptr<NativeLogarithm>>(object)->toString();
      }
      if(object.type() == typeid(std::shared_ptr<NativeExponentiation>)){
        return std::any_cast<std::shared_ptr<NativeExponentiation>>(object)->toString();
      }
      if(object.type() == typeid(std::shared_ptr<NativeSquareRoot>)){
        return std::any_cast<std::shared_ptr<NativeSquareRoot>>(object)->toString();
      }
      if(object.type() == typeid(std::shared_ptr<NativeRandom>)){
        return std::any_cast<std::shared_ptr<NativeRandom>>(object)->toString();
      }
      if(object.type() == typeid(std::shared_ptr<NativeOrd>)){
        return std::any_cast<std::shared_ptr<NativeOrd>>(object)->toString();
      }
      if(object.type() == typeid(std::shared_ptr<NativeStringToNumber>)){
        return std::any_cast<std::shared_ptr<NativeStringToNumber>>(object)->toString();
      }

      return "Error in stringify: object type not recognized.";
    }

    Interpreter(){
      globals->define("std::chrono::clock", std::make_shared<NativeClock>());
      globals->define("std::io::readLine", std::make_shared<NativeReadLine>());
      globals->define("std::io::print", std::make_shared<NativePrint>());
      globals->define("std::io::fileRead", std::make_shared<NativeFileRead>());
      globals->define("std::io::fileWrite", std::make_shared<NativeFileWrite>());
      globals->define("std::math::abs", std::make_shared<NativeAbsoluteValue>());
      globals->define("std::math::ceil", std::make_shared<NativeCeil>());
      globals->define("std::math::floor", std::make_shared<NativeFloor>());
      globals->define("std::math::log", std::make_shared<NativeLogarithm>());
      globals->define("std::math::pow", std::make_shared<NativeExponentiation>());
      globals->define("std::math::sqrt", std::make_shared<NativeSquareRoot>());
      globals->define("std::random::random", std::make_shared<NativeRandom>());
      globals->define("std::utils::ord", std::make_shared<NativeOrd>());
      globals->define("std::utils::strToNum", std::make_shared<NativeStringToNumber>());
      globals->define("std::utils::strToBool", std::make_shared<NativeStringToBool>());
      globals->define("std::utils::strToNil", std::make_shared<NativeStringToNil>());
    }

    /**
     * @brief Interprets/Executes the list of statements that represent a Bleach program (remember that each
     * statement represents an AST of the Bleach language), provided by the parser. 
     *
     * This method is responsible for interpreting/executing the list of statements generated by the parser 
     * (that represents a Bleach program), where each statement is an AST (Abstract Syntax Tree).
     * 
     * @param statements: The list of statements, where each statement is an AST (Abstract Syntax Tree), that 
     * represents a program written in the Bleach language.
     * 
     * @return Nothing (void).
     */
    void interpret(const std::vector<std::shared_ptr<Stmt>>& statements){
      try{
        for(const std::shared_ptr<Stmt>& statement : statements){
          execute(statement);
        }
      }catch(BleachRuntimeError error){
        runtimeError(error);
      }

      return;
    }

  /**
     * @brief Performs a binding between an expression that represents a variable and its respective depth (the
     * position in the environment chain where the Environment instance that contains such variable is).
     * 
     * This method is responsible for receiving an expression that represents a variable in the AST of the
     * Bleach language after the parsing stage and also an integer that tells the interpreter how many hops 
     * must be followed from the current Environment instance that the interpreter is on until the Environment
     * instance where such variable is present is reached. With these two pieces of information, the interpreter
     * can figure out where any local variable is (with respect to Environment instances).
     * 
     * @param expr: An expression that represents the AST node of the variable.
     * @param depth: An integer that represents how many hops must be followed from the current Environment
     * instance that the interpreter is on to find the desired local variable.
     * 
     * @return Nothing (void).
     */
    void resolve(std::shared_ptr<Expr> expr, int depth){
      locals[expr] = depth;

      return;
    }

    /**
     * @brief Creates a new environment for a block statement that is about to be executed and executes each of
     * the statements present inside such block.
     *
     * This method acts as an auxiliary method to the "visitBlockStmt" method from this same class. This method 
     * is responsible for creating a brand new environment for the block statement that is about to be visited
     * (during this creation, it stores the enclosing environment of the block inside the newly created 
     * environment for the block) and also for executing each of the statements that are present inside this 
     * block.
     * 
     * @param statements: The list of statements that the block statement that is about executed contains.
     * @param enviroment: The just created environment that represents the lexical scope (static scope) of the
     * block statement that is about to be executed by the interpreter.
     * 
     * @return Nothing ({}).
     */
    void executeBlock(const std::vector<std::shared_ptr<Stmt>>& statements, std::shared_ptr<Environment> environment){
      std::shared_ptr<Environment> previous = this->environment; // Stores the current environment that the interpreter is looking at inside this "previous" variable.

      try{
        this->environment = environment; // Make the current environment that the interpreter is looking at be the environment of the block statement that is being visited.

        for(const std::shared_ptr<Stmt>& stmt : statements){
          execute(stmt);
        }
      }catch(...){
        this->environment = previous; // Restores the previous environment in case a runtime error is thrown while visiting the statements inside the block.
        throw;
      }

      this->environment = previous; // Restores the previous environment after visiting the statements inside the block.

      return;
    }

    /**
     * @brief Visits a Block Statement node of the Bleach AST and performs the associated actions. 
     *
     * This method is responsible for visiting a Block Statement node of the Bleach AST and performing the
     * associated actions with this type of AST node.
     * 
     * @param stmt: The node of the Bleach AST that is a Block Statement node.
     * 
     * @return Nothing ({}).
     * 
     * @note This method is an overridden version of the "visitBlockStmt" method from the "StmtVisitor" struct.
     */
    std::any visitBlockStmt(std::shared_ptr<Block> stmt) override{
      executeBlock(stmt->statements, std::make_shared<Environment>(environment)); // In order to execute a 'Block' statement, the interpreter needs to create an environment that represents the lexical/static scope of the block and it also needs to execute each statement inside the block.

      return {};
    }

    /**
     * @brief Visits a Break Statement node of the Bleach AST and performs the associated actions. 
     *
     * This method is responsible for visiting a Break Statement node of the Bleach AST and performing the
     * associated actions with this type of AST node.
     * 
     * @param stmt: The node of the Bleach AST that is a Break Statement node.
     * 
     * @return This method does not return anything. Instead it throws an instance of the BleachBreak struct
     * that will be properly caught and dealt with during the execution of a loop structure during runtime.
     * 
     * @note This method is an overridden version of the "visitBreakStmt" method from the "StmtVisitor" struct.
     */
    std::any visitBreakStmt(std::shared_ptr<Break> stmt) override{
      throw BleachBreak{};
    }

    /**
     * @brief Visits a Class Declaration Statement node of the Bleach AST and performs the associated actions. 
     *
     * This method is responsible for visiting a Class Declaration Statement node of the Bleach AST and
     * performing the associated actions with this type of AST node.
     * 
     * @param stmt: The node of the Bleach AST that is a Class Declaration Statement node.
     * 
     * @return Nothing ({}).
     * 
     * @note This method is an overridden version of the "visitClassStmt" method from the "StmtVisitor" struct.
     */
    std::any visitClassStmt(std::shared_ptr<Class> stmt) override{
      std::any superclass;
      if(stmt->superclass != nullptr){
        superclass = evaluate(stmt->superclass); // This line here is responsible for returning the runtime value associated with the name of the superclass.
        if(superclass.type() != typeid(std::shared_ptr<BleachClass>)){
          throw BleachRuntimeError{stmt->superclass->name, "A superclass must be a class"};
        }
      }

      environment->define(stmt->name.lexeme, nullptr); // A class declaration doesn't have a value by itself.

      if(stmt->superclass != nullptr){
        environment = std::make_shared<Environment>(environment);
        environment->define("super", superclass);
      }

      std::map<std::string, std::shared_ptr<BleachFunction>> methods;
      for(std::shared_ptr<Function> method : stmt->methods){
        auto function = std::make_shared<BleachFunction>(method, environment, method->name.lexeme == "init");
        methods[method->name.lexeme] = function;
      }

      std::shared_ptr<BleachClass> superklass = nullptr;
      if(superclass.type() == typeid(std::shared_ptr<BleachClass>)){
        superklass = std::any_cast<std::shared_ptr<BleachClass>>(superclass);
      }
      auto klass = std::make_shared<BleachClass>(stmt->name.lexeme, superklass, methods);

      if(superklass != nullptr){
        environment = environment->enclosing;
      }

      environment->assign(stmt->name, std::move(klass));

      return {};
    }

    /**
     * @brief Visits a Continue Statement node of the Bleach AST and performs the associated actions. 
     *
     * This method is responsible for visiting a Continue Statement node of the Bleach AST and performing the
     * associated actions with this type of AST node.
     * 
     * @param stmt: The node of the Bleach AST that is a Continue Statement node.
     * 
     * @return This method does not return anything. Instead it throws an instance of the BleachContinue struct
     * that will be properly caught and dealt with during the execution of a loop structure during runtime.
     * 
     * @note This method is an overridden version of the "visitContinueStmt" method from the "StmtVisitor"
     * struct.
     */
    std::any visitContinueStmt(std::shared_ptr<Continue> stmt) override{
      throw BleachContinue{};
    }

    /**
     * @brief Visits a DoWhile Statement node of the Bleach AST and performs the associated actions. 
     *
     * This method is responsible for visiting a DoWhile Statement node of the Bleach AST and performing the
     * associated actions with this type of AST node.
     * 
     * @param stmt: The node of the Bleach AST that is a DoWhile Statement node.
     * 
     * @return Nothing ({}).
     * 
     * @note This method is an overridden version of the "visitDoWhileStmt" method from the "StmtVisitor"
     * struct.
     */
    std::any visitDoWhileStmt(std::shared_ptr<DoWhile> stmt) override{
      std::shared_ptr<Environment> previous = this->environment; // Stores the current environment that the interpreter is looking at inside this "previous" variable.

      try{
        this->environment = std::make_shared<Environment>(previous); // Make the current environment that the interpreter is looking at be the environment of the block statement that is being visited.
        
        foundContinueStatement:
          do{
            for(const std::shared_ptr<Stmt>& statement : stmt->body){
              try{
                execute(statement);
              }catch(BleachContinue){
                if(isTruthy(evaluate(stmt->condition))){
                  goto foundContinueStatement;
                }
              }
            }
          }while(isTruthy(evaluate(stmt->condition)));
      }catch(BleachBreak){
        // Do nothing. Just break out of the loop.
      }catch(...){
        this->environment = previous; // Restores the previous environment in case a runtime error is thrown while visiting the statements inside the block.
        throw;
      }

      this->environment = previous; // Restores the previous environment after visiting the statements inside the block.

      return {};
    }

    /**
     * @brief Visits a Expression Statement node of the Bleach AST and performs the associated actions. 
     *
     * This method is responsible for visiting a Expression Statement node of the Bleach AST and performing the
     * associated actions with this type of AST node.
     * 
     * @param stmt: The node of the Bleach AST that is a Expression Statement node.
     * 
     * @return Nothing ({}).
     * 
     * @note This method is an overridden version of the "visitExpressionStmt" method from the "StmtVisitor"
     * struct.
     */
    std::any visitExpressionStmt(std::shared_ptr<Expression> stmt) override{
      evaluate(stmt->expression);

      return {};
    }

    /**
     * @brief Visits a For Statement node of the Bleach AST and performs the associated actions. 
     *
     * This method is responsible for visiting a For Statement node of the Bleach AST and performing the
     * associated actions with this type of AST node.
     * 
     * @param stmt: The node of the Bleach AST that is a For Statement node.
     * 
     * @return Nothing ({}).
     * 
     * @note This method is an overridden version of the "visitForStmt" method from the "StmtVisitor" struct.
     */
    std::any visitForStmt(std::shared_ptr<For> stmt) override{
      std::shared_ptr<Environment> previous = this->environment; // Stores the current environment that the interpreter is looking at inside this "previous" variable.

      try{
        this->environment = std::make_shared<Environment>(previous); // Make the current environment that the interpreter is looking at be the environment of the block statement that is being visited.
        bool hasFoundContinueStatement = false;
        
        execute(stmt->initializer);

        foundContinueStatement:

        if(hasFoundContinueStatement){
          evaluate(stmt->increment);
        }
        while(isTruthy(evaluate(stmt->condition))){
          for(const std::shared_ptr<Stmt>& statement : stmt->body){
            try{
              execute(statement);
            }catch(BleachContinue){
              hasFoundContinueStatement = true;
              goto foundContinueStatement;
            }
          }
          evaluate(stmt->increment);
        }
      }catch(BleachBreak){
        // Do nothing. Just break out of the loop.
      }catch(...){
        this->environment = previous; // Restores the previous environment in case a runtime error is thrown while visiting the statements inside the block.
        throw;
      }

      this->environment = previous; // Restores the previous environment after visiting the statements inside the block.

      return {};
    }

    /**
     * @brief Visits a Function Declaration Statement node of the Bleach AST and performs the associated
     * actions. 
     *
     * This method is responsible for visiting a Function Declaration Statement node of the Bleach AST and
     * performing the associated actions with this type of AST node.
     * 
     * @param stmt: The node of the Bleach AST that is a Function Declaration Statement node.
     * 
     * @return Nothing ({}).
     * 
     * @note This method is an overridden version of the 'visitFunctionStmt' method from the 'StmtVisitor' 
     * struct.
     * Moreover, pay attention to the following fact: When the interpreter visits a Function Statement node, it
     * creates an instance of a "BleachFunction" object and stores it in the current environment. Moreover, 
     * since functions in Bleach must hold on to their parent environment, during the instance creation, the 
     * interpreter assigns the current environment that it currently is at as parent environment of the 
     * "BleachFunction" instance.
     */
    std::any visitFunctionStmt(std::shared_ptr<Function> stmt) override{
      auto function = std::make_shared<BleachFunction>(stmt, environment, false);
      environment->define(stmt->name.lexeme, function);

      return {};
    }

    /**
     * @brief Visits an If Statement node of the Bleach AST and performs the associated actions. 
     *
     * This method is responsible for visiting an If Statement node of the Bleach AST and performing the
     * associated actions with this type of AST node.
     * 
     * @param stmt: The node of the Bleach AST that is an If Statement node.
     * 
     * @return Nothing ({}).
     * 
     * @note This method is an overridden version of the "visitIfStmt" method from the "StmtVisitor" struct.
     */
    std::any visitIfStmt(std::shared_ptr<If> stmt) override{
      if(isTruthy(evaluate(stmt->ifCondition))){
        execute(stmt->ifBranch);
        return {};
      }
      for(int i = 0; i < stmt->elifConditions.size(); i++){
        if(isTruthy(evaluate(stmt->elifConditions[i]))){
          execute(stmt->elifBranches[i]);
          return {};
        }
      }
      if(stmt->elseBranch != nullptr){
        execute(stmt->elseBranch);
      }

      return {};
    }

    /**
     * @brief Visits a Print Statement node of the Bleach AST and performs the associated actions. 
     *
     * This method is responsible for visiting a Print Statement node of the Bleach AST and performing the
     * associated actions with this type of AST node.
     * 
     * @param stmt: The node of the Bleach AST that is a Print Statement node.
     * 
     * @return Nothing ({}).
     * 
     * @note This method is an overridden version of the "visitPrintStmt" method from the "StmtVisitor" struct.
     */
    std::any visitPrintStmt(std::shared_ptr<Print> stmt) override{
      std::any value = evaluate(stmt->expression);

      std::cout << stringify(value) << std::endl;

      return {};
    }

    /**
     * @brief Visits a Return Statement node of the Bleach AST and performs the associated actions. 
     *
     * This method is responsible for visiting a Return Statement node of the Bleach AST and performing the
     * associated actions with this type of AST node.
     * 
     * @param stmt: The node of the Bleach AST that is a Return Statement node.
     * 
     * @return Nothing ({}).
     * 
     * @note This method is an overridden version of the "visitReturnStmt" method from the "StmtVisitor"
     * struct. Moreover, this method will always throw a value. The value is the one generated when evaluating
     * the expression that might be present in the "return" statement. If there is no expression, then this
     * means the produced value is nil (nullptr).
     */
    std::any visitReturnStmt(std::shared_ptr<Return> stmt) override{
      std::any value = nullptr;
      if(stmt->value != nullptr){
        value = evaluate(stmt->value);
      }

      throw BleachReturn{value};
    }

    /**
     * @brief Visits a Variable Declaration Statement node of the Bleach AST and performs the associated 
     * actions. 
     *
     * This method is responsible for visiting a Variable Declaration Statement node of the Bleach AST and 
     * performing the associated actions with this type of AST node.
     * 
     * @param stmt: The node of the Bleach AST that is a Variable Declaration Statement node.
     * 
     * @return Nothing ({}).
     * 
     * @note This method is an overridden version of the "visitVarStmt" method from the "StmtVisitor" struct.
     */
    std::any visitVarStmt(std::shared_ptr<Var> stmt) override{
      std::string variableName = stmt->name.lexeme;
      std::any initialValue = nullptr;

      if(stmt->initializer != nullptr){
        initialValue = evaluate(stmt->initializer);
      }

      environment->define(variableName, std::move(initialValue));

      return {};
    }

    /**
     * @brief Visits a While Statement node of the Bleach AST and performs the associated actions. 
     *
     * This method is responsible for visiting a While Statement node of the Bleach AST and performing the
     * associated actions with this type of AST node.
     * 
     * @param stmt: The node of the Bleach AST that is a While Statement node.
     * 
     * @return Nothing ({}).
     * 
     * @note This method is an overridden version of the "visitWhileStmt" method from the "StmtVisitor" struct.
     */
    std::any visitWhileStmt(std::shared_ptr<While> stmt) override{
      std::shared_ptr<Environment> previous = this->environment; // Stores the current environment that the interpreter is looking at inside this "previous" variable.

      try{
        this->environment = std::make_shared<Environment>(previous); // Make the current environment that the interpreter is looking at be the environment of the block statement that is being visited.

        foundContinueStatement:
          while(isTruthy(evaluate(stmt->condition))){
            for(const std::shared_ptr<Stmt>& statement : stmt->body){
              try{
                execute(statement);
              }catch(BleachContinue){
                goto foundContinueStatement;
              }
            }
          }
      }catch(BleachBreak){
        // Do nothing. Just break out of the loop.
      }catch(...){
        this->environment = previous; // Restores the previous environment in case a runtime error is thrown while visiting the statements inside the block.
        throw;
      }
      this->environment = previous; // Restores the previous environment after visiting the statements inside the block.

      return {};
    }

    /**
     * @brief Visits an Assign expression node of the Bleach AST and produces the corresponding value. 
     *
     * This method is responsible for visiting an Assign expression node of the Bleach AST, producing a value
     * that corresponds to the type of expression present inside such Assign expression node.
     * 
     * @param expr: The node of the Bleach AST that is a Assign expression node.
     * 
     * @return The value obtained from the visit (evaluation of) to a Assign expression node of the Bleach AST.
     * 
     * @note This method is an overridden version of the "visitAssignExpr" method from the "ExprVisitor"
     * struct.
     */
    std::any visitAssignExpr(std::shared_ptr<Assign> expr) override{
      std::any value = evaluate(expr->value);

      auto elem = locals.find(expr);
      if(elem != locals.end()){
        int distance = elem->second;
        environment->assignAt(expr->name, value, distance);
      }else{
        globals->assign(expr->name, value);
      }

      return value;
    }

    /**
     * @brief Visits a Binary expression node of the Bleach AST and produces the corresponding value. 
     *
     * This method is responsible for visiting a Binary expression node of the Bleach AST, producing a value
     * that corresponds to the type of expression present inside such Binary expression node.
     * 
     * @param expr: The node of the Bleach AST that is a Binary expression node.
     * 
     * @return The value obtained from the visit (evaluation of) to a Binary expression node of the Bleach AST.
     * 
     * @note This method is an overridden version of the "visitBinaryExpr" method from the "ExprVisitor"
     * struct.
     */
    std::any visitBinaryExpr(std::shared_ptr<Binary> expr) override{
      std::any left = evaluate(expr->left);
      std::any right = evaluate(expr->right);

      switch(expr->op.type){
        case(TokenType::GREATER):
          if(checkNumberOperands(left, right)){
            return std::any_cast<double>(left) > std::any_cast<double>(right);
          }else if(checkStringOperands(left, right)){
            return std::any_cast<std::string>(left) > std::any_cast<std::string>(right);
          }

          throw BleachRuntimeError{expr->op, "Operands must be 2 numbers or 2 strings."};
        case(TokenType::GREATER_EQUAL):
          if(checkNumberOperands(left, right)){
            return std::any_cast<double>(left) >= std::any_cast<double>(right);
          }else if(checkStringOperands(left, right)){
            return std::any_cast<std::string>(left) >= std::any_cast<std::string>(right);
          }

          throw BleachRuntimeError{expr->op, "Operands must be 2 numbers or 2 strings."};
        case(TokenType::LESS):
          if(checkNumberOperands(left, right)){
            return std::any_cast<double>(left) < std::any_cast<double>(right);
          }else if(checkStringOperands(left, right)){
            return std::any_cast<std::string>(left) < std::any_cast<std::string>(right);
          }

          throw BleachRuntimeError{expr->op, "Operands must be 2 numbers or 2 strings."};
        case(TokenType::LESS_EQUAL):
          if(checkNumberOperands(left, right)){
            return std::any_cast<double>(left) <= std::any_cast<double>(right);
          }else if(checkStringOperands(left, right)){
            return std::any_cast<std::string>(left) <= std::any_cast<std::string>(right);
          }

          throw BleachRuntimeError{expr->op, "Operands must be 2 numbers or 2 strings."};
        case(TokenType::BANG_EQUAL):
          return !isEqual(left, right);
        case(TokenType::EQUAL_EQUAL):
          return isEqual(left, right);
        case(TokenType::PLUS):
          if(left.type() == typeid(double) && right.type() == typeid(double)){
            return std::any_cast<double>(left) + std::any_cast<double>(right);
          }
          if(left.type() == typeid(std::string) && right.type() == typeid(std::string)){
            return std::any_cast<std::string>(left) + std::any_cast<std::string>(right);
          }
          if(left.type() == typeid(double) && right.type() == typeid(std::string)){
            return formatDouble(std::any_cast<double>(left)) + std::any_cast<std::string>(right);
          }
          if((left.type() == typeid(std::string) && right.type() == typeid(double))){
            return std::any_cast<std::string>(left) + formatDouble(std::any_cast<double>(right));
          }
          if(left.type() == typeid(std::string) && right.type() == typeid(std::shared_ptr<BleachInstance>)){
            return std::any_cast<std::string>(left) + std::any_cast<std::shared_ptr<BleachInstance>>(right)->toString(*this);
          }
          if(left.type() == typeid(std::shared_ptr<BleachInstance>) && right.type() == typeid(std::string)){
            return std::any_cast<std::shared_ptr<BleachInstance>>(left)->toString(*this) + std::any_cast<std::string>(right);
          }
          if(left.type() == typeid(std::shared_ptr<std::vector<std::any>>) && right.type() == typeid(std::shared_ptr<std::vector<std::any>>)){
            auto result = std::make_shared<std::vector<std::any>>();
            auto vec1Ptr = std::any_cast<std::shared_ptr<std::vector<std::any>>>(left);
            auto vec2Ptr = std::any_cast<std::shared_ptr<std::vector<std::any>>>(right);
            result->insert(result->end(), vec1Ptr->begin(), vec1Ptr->end());
            result->insert(result->end(), vec2Ptr->begin(), vec2Ptr->end());
            return result;
          }

          throw BleachRuntimeError{expr->op, "Operands must be two numbers, or two strings, or two lists, or one number and one string."};
        case(TokenType::MINUS):
          if(checkNumberOperands(left, right)){
            return std::any_cast<double>(left) - std::any_cast<double>(right); // If the cast does not work, it will throw a bad_cast error.
          }

          throw BleachRuntimeError{expr->op, "Operands must be 2 numbers."};
        case(TokenType::STAR):
          if(checkNumberOperands(left, right)){
            return std::any_cast<double>(left) * std::any_cast<double>(right); // Evaluate the case of iteracting nums and strings in order to extend the language.
          }

          throw BleachRuntimeError{expr->op, "Operands must be 2 numbers."};
        case(TokenType::SLASH):
          if(checkNumberOperands(left, right)){
            checkZeroDivisor(right, expr->op);
            return std::any_cast<double>(left) / std::any_cast<double>(right); // If the cast does not work, it will throw a bad_cast error.
          }
      
          throw BleachRuntimeError{expr->op, "Operands must be 2 numbers."};
        case(TokenType::REMAINDER):
          if(checkNumberOperands(left, right)){
            checkZeroDivisor(right, expr->op);
            double dividend = std::any_cast<double>(left);
            double divisor = std::any_cast<double>(right);
            return std::fmod(dividend, divisor);
          }

          throw BleachRuntimeError{expr->op, "Operands must be 2 numbers."};
      }

      // Unreachable
      return {};
    }

    /**
     * @brief Visits a Call expression node of the Bleach AST and produces the corresponding value. 
     *
     * This method is responsible for visiting a Call expression node of the Bleach AST, producing a value
     * that corresponds to the type of expression present inside such Call expression node.
     * 
     * @param expr: The node of the Bleach AST that is a Call expression node.
     * 
     * @return The value obtained from the visit (evaluation) to a Call expression node of the Bleach AST.
     * 
     * @note This method is an overridden version of the "visitCallExpr" method from the "ExprVisitor" struct.
     */
    std::any visitCallExpr(std::shared_ptr<Call> expr) override{
      std::any callee = evaluate(expr->callee); // First, the interpreter needs to evaluate the callee. Typically, this expression is just an identifier that looks up the function by its name, but it could be anything.

      std::vector<std::any> arguments;
      for(const std::shared_ptr<Expr>& argument : expr->arguments){ // Second, the interpreter evaluates, in order, each expression inside the arguments list to produce its respective value.
        arguments.push_back(evaluate(argument));
      }

      std::shared_ptr<BleachCallable> function;

      if(callee.type() == typeid(std::shared_ptr<BleachClass>)){
        function = std::any_cast<std::shared_ptr<BleachClass>>(callee);  // Pointers in a "std::any" wrapper must be unwrapped before they can be cast.
        if(arguments.size() != function->arity()){ // Checks whether the number of arguments passed in the class, function or method call is equal to its declared arity.
          throw BleachRuntimeError{expr->paren, "Expected " + std::to_string(function->arity()) + " arguments, but instead received " + std::to_string(arguments.size()) + "."};
        }
        return function->call(*this, std::move(arguments)); // Finally, the interpreter calls an instance of a Bleach class.
      }
      else if(callee.type() == typeid(std::shared_ptr<BleachFunction>)){ // Third, the interpreter checks whether the callee is of type "BleachFuntion" because, if that's not the case, then an user cannot call it.
        function = std::any_cast<std::shared_ptr<BleachFunction>>(callee);  // Pointers in a "std::any" wrapper must be unwrapped before they can be cast.
        if(arguments.size() != function->arity()){ // Checks whether the number of arguments passed in the class, function or method call is equal to its declared arity.
          throw BleachRuntimeError{expr->paren, "Expected " + std::to_string(function->arity()) + " arguments, but instead received " + std::to_string(arguments.size()) + "."};
        }
        return function->call(*this, std::move(arguments)); // Finally, the interpreter calls a Bleach function.
      }
      else if(callee.type() == typeid(std::shared_ptr<BleachLambdaFunction>)){
        function = std::any_cast<std::shared_ptr<BleachLambdaFunction>>(callee);  // Pointers in a "std::any" wrapper must be unwrapped before they can be cast.
        if(arguments.size() != function->arity()){ // Checks whether the number of arguments passed in the class, function or method call is equal to its declared arity.
          throw BleachRuntimeError{expr->paren, "Expected " + std::to_string(function->arity()) + " arguments, but instead received " + std::to_string(arguments.size()) + "."};
        }
        return function->call(*this, std::move(arguments)); // Finally, the interpreter calls a Bleach lambda function.
      }
      else if(callee.type() == typeid(std::shared_ptr<NativeClock>)){
        function = std::any_cast<std::shared_ptr<NativeClock>>(callee);

        return function->call(*this, std::move(expr->paren), std::move(arguments)); // Finally, the interpreter calls a Bleach native function.
      }
      else if(callee.type() == typeid(std::shared_ptr<NativeReadLine>)){
        function = std::any_cast<std::shared_ptr<NativeReadLine>>(callee);

        return function->call(*this, std::move(expr->paren), std::move(arguments)); // Finally, the interpreter calls a Bleach native function.
      }
      else if(callee.type() == typeid(std::shared_ptr<NativePrint>)){
        function = std::any_cast<std::shared_ptr<NativePrint>>(callee);

        return function->call(*this, std::move(expr->paren), std::move(arguments)); // Finally, the interpreter calls a Bleach native function.
      }
      else if(callee.type() == typeid(std::shared_ptr<NativeFileRead>)){
        function = std::any_cast<std::shared_ptr<NativeFileRead>>(callee);

        return function->call(*this, std::move(expr->paren), std::move(arguments)); // Finally, the interpreter calls a Bleach native function.
      }
      else if(callee.type() == typeid(std::shared_ptr<NativeFileWrite>)){
        function = std::any_cast<std::shared_ptr<NativeFileWrite>>(callee);

        return function->call(*this, std::move(expr->paren), std::move(arguments)); // Finally, the interpreter calls a Bleach native function.
      }
      else if(callee.type() == typeid(std::shared_ptr<NativeAbsoluteValue>)){
        function = std::any_cast<std::shared_ptr<NativeAbsoluteValue>>(callee);

        return function->call(*this, std::move(expr->paren), std::move(arguments)); // Finally, the interpreter calls a Bleach native function.
      }
      else if(callee.type() == typeid(std::shared_ptr<NativeCeil>)){
        function = std::any_cast<std::shared_ptr<NativeCeil>>(callee);

        return function->call(*this, std::move(expr->paren), std::move(arguments)); // Finally, the interpreter calls a Bleach native function.
      }
      else if(callee.type() == typeid(std::shared_ptr<NativeFloor>)){
        function = std::any_cast<std::shared_ptr<NativeFloor>>(callee);

        return function->call(*this, std::move(expr->paren), std::move(arguments)); // Finally, the interpreter calls a Bleach native function.
      }
      else if(callee.type() == typeid(std::shared_ptr<NativeExponentiation>)){
        function = std::any_cast<std::shared_ptr<NativeExponentiation>>(callee);

        return function->call(*this, std::move(expr->paren), std::move(arguments)); // Finally, the interpreter calls a Bleach native function.
      }
      else if(callee.type() == typeid(std::shared_ptr<NativeLogarithm>)){
        function = std::any_cast<std::shared_ptr<NativeLogarithm>>(callee);

        return function->call(*this, std::move(expr->paren), std::move(arguments)); // Finally, the interpreter calls a Bleach native function.
      }
      else if(callee.type() == typeid(std::shared_ptr<NativeSquareRoot>)){
        function = std::any_cast<std::shared_ptr<NativeSquareRoot>>(callee);

        return function->call(*this, std::move(expr->paren), std::move(arguments)); // Finally, the interpreter calls a Bleach native function.
      }
      else if(callee.type() == typeid(std::shared_ptr<NativeRandom>)){
        function = std::any_cast<std::shared_ptr<NativeRandom>>(callee);

        return function->call(*this, std::move(expr->paren), std::move(arguments)); // Finally, the interpreter calls a Bleach native function.
      }
      else if(callee.type() == typeid(std::shared_ptr<NativeOrd>)){
        function = std::any_cast<std::shared_ptr<NativeOrd>>(callee);

        return function->call(*this, std::move(expr->paren), std::move(arguments)); // Finally, the interpreter calls a Bleach native function.
      }
      else if(callee.type() == typeid(std::shared_ptr<NativeStringToNumber>)){
        function = std::any_cast<std::shared_ptr<NativeStringToNumber>>(callee);

        return function->call(*this, std::move(expr->paren), std::move(arguments)); // Finally, the interpreter calls a Bleach native function.
      }
      else if(callee.type() == typeid(std::shared_ptr<NativeStringToBool>)){
        function = std::any_cast<std::shared_ptr<NativeStringToBool>>(callee);

        return function->call(*this, std::move(expr->paren), std::move(arguments)); // Finally, the interpreter calls a Bleach native function.
      }
      else if(callee.type() == typeid(std::shared_ptr<NativeStringToNil>)){
        function = std::any_cast<std::shared_ptr<NativeStringToNil>>(callee);

        return function->call(*this, std::move(expr->paren), std::move(arguments)); // Finally, the interpreter calls a Bleach native function.
      }
      // Methods from 'list' and/or 'str' types:
      // "str" method: "find".
      else if(callee.type() == typeid(std::function<double(std::string)>)){
        auto stringMethod = std::any_cast<std::function<double(std::string)>>(callee);

        if(arguments.size() != 1){
          throw BleachRuntimeError{expr->paren, "Expected 1 argument for the 'find' method."};
        }
        if(arguments[0].type() != typeid(std::string)){
          throw BleachRuntimeError{expr->paren, "Expected 1 argument of type 'str' for the 'find' method."};
        }

        return stringMethod(std::any_cast<std::string>(arguments[0]));
      }
      // "list" and "str" method: "empty".
      else if(callee.type() == typeid(std::function<bool()>)){
        auto stringMethod = std::any_cast<std::function<bool()>>(callee);

        if(arguments.size() != 0){
          throw BleachRuntimeError{expr->paren, "Expected no arguments for the 'empty' method."};
        }

        return stringMethod();
      }
      // "list" method: "size" ---- "str" method: "length".
      else if(callee.type() == typeid(std::function<double()>)){
        auto stringMethod = std::any_cast<std::function<double()>>(callee);

        if(arguments.size() != 0){
          throw BleachRuntimeError{expr->paren, "Expected no arguments for the 'length' method."};
        }

        return std::floor(stringMethod());
      }
      // "str" method: "split".
      else if(callee.type() == typeid(std::function<std::shared_ptr<std::vector<std::any>>(std::string)>)){
        auto stringMethod = std::any_cast<std::function<std::shared_ptr<std::vector<std::any>>(std::string)>>(callee);

        if(arguments.size() != 1){
          throw BleachRuntimeError{expr->paren, "Expected 1 arguments for the 'split' method."};
        }
        if(arguments[0].type() != typeid(std::string)){
          throw BleachRuntimeError{expr->paren, "Expected 1 argument of type 'str' for the 'split' method."};
        }

        std::string separator = std::any_cast<std::string>(arguments[0]);

        return stringMethod(separator);
      }
      // "str" method: "substr".
      else if(callee.type() == typeid(std::function<std::string(double, double)>)){
        auto stringMethod = std::any_cast<std::function<std::string(double, double)>>(callee);

        if(arguments.size() != 2){
          throw BleachRuntimeError{expr->paren, "Expected 2 arguments for the 'substr' method."};
        }
        if(arguments[0].type() != typeid(double) || arguments[1].type() != typeid(double)){
          throw BleachRuntimeError{expr->paren, "Expected 2 arguments of type 'num' for the 'substr' method."};
        }

        double left = std::any_cast<double>(arguments[0]);
        double right = std::any_cast<double>(arguments[1]);

        if(left > right){
          throw BleachRuntimeError{expr->paren, "The value of the first argument cannot be larger than the second argument for the 'substr' method."};
        }
        if(left < 0 || right < 0){
          throw BleachRuntimeError{expr->paren, "The values of both arguments cannot be negative for the 'substr' method."};
        }
        if(std::floor(left) != left || std::floor(right) != right){
          throw BleachRuntimeError{expr->paren, "The value of both arguments must integers of type 'num'."};
        }

        return stringMethod(std::floor(left), std::floor(right));
      }
      // "list" method: "append".
      else if(callee.type() == typeid(std::function<void(std::any)>)){
        auto listMethod = std::any_cast<std::function<void(std::any)>>(callee);

        if(arguments.size() != 1){
          throw BleachRuntimeError{expr->paren, "Expected 1 arguments for the 'append' method."};
        }

        std::any value = arguments[0];
        listMethod(value);

        return nullptr;
      }
      // "list" method: "getAt".
      else if(callee.type() == typeid(std::function<std::any(int)>)){
        auto listMethod = std::any_cast<std::function<std::any(int)>>(callee);

        if(arguments.size() != 1){
          throw BleachRuntimeError{expr->paren, "Expected 1 arguments for the 'getAt' method."};
        }
        if(arguments[0].type() != typeid(double)){
          throw BleachRuntimeError{expr->paren, "Expected 1 argument of type 'num' for the 'getAt' method."};
        }

        double indexObject = std::any_cast<double>(arguments[0]);

        if(std::floor(indexObject) != indexObject){
          throw BleachRuntimeError{expr->paren, "The value of the first argument must be an integer of type 'num' for the 'getAt' method."};
        }

        int index = std::floor(indexObject);

        return listMethod(index);
      }
      // "list" method: "clear".
      else if(callee.type() == typeid(std::function<void()>)){
        auto listMethod = std::any_cast<std::function<void()>>(callee);

        if(arguments.size() != 0){
          throw BleachRuntimeError{expr->paren, "Expected 0 arguments for the 'clear' method."};
        }

        listMethod();

        return nullptr;
      }
      // "list" method: "pop".
      else if(callee.type() == typeid(std::function<std::any()>)){
        auto listMethod = std::any_cast<std::function<std::any()>>(callee);

        if(arguments.size() != 0){
          throw BleachRuntimeError{expr->paren, "Expected 0 arguments for the 'pop' method."};
        }

        return listMethod();
      }
      // "list" method: "setAt".
      else if(callee.type() == typeid(std::function<void(int, std::any)>)){
        auto listMethod = std::any_cast<std::function<void(int, std::any)>>(callee);

        if(arguments.size() != 2){
          throw BleachRuntimeError{expr->paren, "Expected 2 arguments for the 'setAt' method."};
        }
        if(arguments[0].type() != typeid(double)){
          throw BleachRuntimeError{expr->paren, "Expected the first argument to be of type 'num' for the 'setAt' method."};
        }

        double indexObject = std::any_cast<double>(arguments[0]);

        if(std::floor(indexObject) != indexObject){
          throw BleachRuntimeError{expr->paren, "The value of the first argument must be an integer of type 'num' for the 'setAt' method."};
        }

        int index = std::floor(indexObject);
        std::any value = arguments[1];

        listMethod(index, value);

        return nullptr;
      }
      // "list" method: "fill".
      else if(callee.type() == typeid(std::function<void(std::any, int)>)){
        auto listMethod = std::any_cast<std::function<void(std::any, int)>>(callee);

        if(arguments.size() != 2){
          throw BleachRuntimeError{expr->paren, "Expected 2 arguments for the 'fill' method."};
        }
        if(arguments[1].type() != typeid(double)){
          throw BleachRuntimeError{expr->paren, "Expected the second argument to be of type 'num' for the 'fill' method."};
        }

        double amountObject = std::any_cast<double>(arguments[1]);

        if(std::floor(amountObject) != amountObject){
          throw BleachRuntimeError{expr->paren, "The value of the second argument must be an integer of type 'num' for the 'fill' method."};
        }

        int amount = std::floor(amountObject);
        std::any value = arguments[0];

        listMethod(value, amount);

        return nullptr;
      }


      throw BleachRuntimeError{expr->paren, "Can only call classes, functions, lambda functions, methods and native functions."};
    }

    /**
     * @brief Visits a Get Expression node of the Bleach AST and performs the associated actions. 
     *
     * This method is responsible for visiting a Get Expression node of the Bleach AST and performing the
     * associated actions with this type of AST node.
     * 
     * @param expr: The node of the Bleach AST that is a Get Expression node.
     * 
     * @return The value obtained from the visit (evaluation of) to a Get expression node of the Bleach AST.
     * 
     * @note This method is an overridden version of the "visitGetExpr" method from the "ExprVisitor" struct.
     */
    std::any visitGetExpr(std::shared_ptr<Get> expr) override{
      std::any object = evaluate(expr->object);

      if(object.type() == typeid(std::shared_ptr<BleachInstance>)){
        return std::any_cast<std::shared_ptr<BleachInstance>>(object)->get(expr->name);
      }else if(object.type() == typeid(std::string)){
        std::string str = std::any_cast<std::string>(object);
        Token methodToken = expr->name;
        std::string methodName = expr->name.lexeme;

        if(methodName == "find"){
          return std::function<double(std::string)>([str](std::string substr){
            if(str.find(substr) == std::string::npos){
              return static_cast<double>(-1);
            }
            return static_cast<double>(str.find(substr));
          });
        }else if(methodName == "length"){
          return std::function<double()>([str](){
            return static_cast<double>(str.size());
          });
        }else if(methodName == "empty"){
          return std::function<bool()>([str](){
            return str.empty();
          });
        }else if(methodName == "split"){
          return std::function<std::shared_ptr<std::vector<std::any>>(std::string)>([str, methodToken](std::string separator){
            auto result = std::make_shared<std::vector<std::any>>();
            size_t start = 0;
            size_t end = 0;

            while ((end = str.find(separator, start)) != std::string::npos) {
                result->push_back(str.substr(start, end - start));
                start = end + separator.length();
            }
            result->push_back(str.substr(start));

            return result;
          });
        }else if(methodName == "substr"){
          return std::function<std::string(double, double)>([str, methodToken](double left, double right){
            int start = static_cast<int>(std::floor(left));
            int end = static_cast<int>(std::floor(right));

            if(start >= str.length()){
              throw BleachRuntimeError{methodToken, "The value of the first argument cannot be equal to or larger than the size of the value of 'str' type."};
            }

            return str.substr(start, end - start + 1);
          });
        }

        throw BleachRuntimeError{expr->name, "Undefined method of the 'str' type."};
      }else if(object.type() == typeid(std::shared_ptr<std::vector<std::any>>)){
        auto vecPtr = std::any_cast<std::shared_ptr<std::vector<std::any>>>(object);
        Token methodToken = expr->name;
        std::string methodName = expr->name.lexeme;

        if(methodName == "getAt"){ // DONE
          return std::function<std::any(int)>([vecPtr, methodToken](int index){
            if(index < 0 || index >= vecPtr->size()){
              throw BleachRuntimeError{methodToken, "Index out of bounds. The value of 'list' type has size equal " + std::to_string(vecPtr->size()) + ", but the index provided was equal to: " + std::to_string(static_cast<int>(std::floor(index))) + "."};
            }
            return vecPtr->at(index);
          });
        }else if(methodName == "clear"){ // DONE
          return std::function<void()>([vecPtr](){
            vecPtr->clear();
            return;
          });
        }else if(methodName == "empty"){ // DONE
          return std::function<bool()>([vecPtr](){
            return vecPtr->empty();
          });
        }else if(methodName == "fill"){ // DONE
          return std::function<void(std::any, int)>([vecPtr, methodToken](std::any value, int size){
            if(size < 0){
              throw BleachRuntimeError{methodToken, "Index out of bounds. The size of 'list' type cannot be negative. The value provided as the second argument was: " + std::to_string(size) + "."};
            }
            vecPtr->resize(size);
            for(int i = 0; i < vecPtr->size(); i++){
              (*vecPtr)[i] = value;
            }
            return;
          });
        }
        else if(methodName == "pop"){ // DONE
          return std::function<std::any()>([vecPtr, methodToken](){
            if(vecPtr->empty()){
              throw BleachRuntimeError{methodToken, "The value of 'list' type is already empty."};
            }
            std::any lastValue = vecPtr->at(vecPtr->size() - 1);
            vecPtr->pop_back();
            return lastValue;
          });
        }else if(methodName == "append"){ // DONE
          return std::function<void(std::any)>([vecPtr](std::any value){
            vecPtr->push_back(value);
            return;
          });
        }else if(methodName == "setAt"){ // DONE
          return std::function<void(int, std::any)>([vecPtr, methodToken](int index, std::any value){
            if(index < 0 || index >= vecPtr->size()){
              throw BleachRuntimeError{methodToken, "Index out of bounds. The value of 'list' type has size equal " + std::to_string(vecPtr->size()) + ", but the index provided was equal to: " + std::to_string(static_cast<int>(std::floor(index))) + "."};
            }
            (*vecPtr)[index] = value;
            return;
          });
        }else if(methodName == "size"){ // DONE
          return std::function<double()>([vecPtr](){
            return static_cast<double>(vecPtr->size());
          });
        }

        throw BleachRuntimeError{expr->name, "Undefined method of the 'list' type."};
      }

      throw BleachRuntimeError{expr->name, "Only instances, lists or strings have properties."};
    }

    /**
     * @brief Visits a Grouping (parenthesized) expression node of the Bleach AST and produces the 
     * corresponding value. 
     *
     * This method is responsible for visiting a Grouping expression node of the Bleach AST, producing a value
     * that corresponds to the type of expression present inside such Grouping expression node.
     * 
     * @param expr: The node of the Bleach AST that is a Grouping expression node.
     * 
     * @return The value obtained from the visit (evaluation) to a Grouping expression node of the Bleach AST.
     * 
     * @note This method is an overridden version of the 'visitGroupingExpr' method from the 'ExprVisitor' 
     * struct.
     */
    std::any visitGroupingExpr(std::shared_ptr<Grouping> expr) override{
      return evaluate(expr->expression);
    }

    /**
     * @brief Visits a LambdaFunction Expression node of the Bleach AST and performs the associated actions. 
     *
     * This method is responsible for visiting a LambdaFunction Expression node of the Bleach AST and
     * performing the associated actions with this type of AST node.
     * 
     * @param expr: The node of the Bleach AST that is a LambdaFunction Expression node.
     * 
     * @return The value obtained from the visit (evaluation of) to a LambdaFunction expression node of the
     * Bleach AST.
     * 
     * @note This method is an overridden version of the "visitLambdaFunctionExpr" method from the
     * "ExprVisitor" struct.
     */
    std::any visitLambdaFunctionExpr(std::shared_ptr<LambdaFunction> expr) override{
      return std::make_shared<BleachLambdaFunction>(expr, environment);
    }

    std::any visitListLiteralExpr(std::shared_ptr<ListLiteral> expr) override{
      std::vector<std::any> vec;

      for(int i = 0; i < expr->elements.size(); i++){
        vec.push_back(evaluate(expr->elements[i]));
      }

      return std::make_shared<std::vector<std::any>>(vec);
    }

    /**
     * @brief Visits a Literal expression node of the Bleach AST and produces the corresponding value. 
     *
     * This method is responsible for visiting a Literal expression node of the Bleach AST, producing a value
     * that corresponds to the type of expression present inside such Literal expression node.
     * 
     * @param expr: The node of the Bleach AST that is a Literal expression node.
     * 
     * @return The value obtained from the visit (evaluation) to a Literal expression node of the Bleach AST.
     * 
     * @note This method is an overridden version of the "visitLiteralExpr" method from the "ExprVisitor" 
     * struct.
     */
    std::any visitLiteralExpr(std::shared_ptr<Literal> expr) override{
      return expr->value;
    }

    /**
     * @brief Visits a Logical expression node of the Bleach AST and produces the corresponding value. 
     *
     * This method is responsible for visiting a Logical expression node of the Bleach AST, producing a value
     * that corresponds to the type of expression present inside such Binary expression node.
     * 
     * @param expr: The node of the Bleach AST that is a Binary expression node.
     * 
     * @return The value obtained from the visit (evaluation) to a Logical expression node of the Bleach AST.
     * 
     * @note This method is an overridden version of the "visitLogicalExpr" method from the "ExprVisitor" 
     * struct.
     * Moreover, pay attention to the fact that logical operators do not return "true" or "false". Instead,
     * they return the value itself. 
     * Furthermore, note that this method applies the idea of short-circuit by first evaluating the left
     * operand of the logical expression. After that, it checks the operator type and if it's possible to
     * short-circuit. If that's the case, it prematurely returns the value produced by the evaluation of the
     * left expression. Otherwise, it evaluates the right expression and returns its value.
     */
    std::any visitLogicalExpr(std::shared_ptr<Logical> expr) override{
      std::any left = evaluate(expr->left);

      if(expr->op.type == TokenType::AND){
        if(!isTruthy(left)){
          return left;
        }
      }else if(expr->op.type == TokenType::OR){
        if(isTruthy(left)){
          return left;
        }
      }

      return evaluate(expr->right);
    }

    /**
     * @brief Visits a Self Expression node of the Bleach AST and performs the associated actions. 
     *
     * This method is responsible for visiting a Self Expression node of the Bleach AST and performing the
     * associated actions with this type of AST node.
     * 
     * @param expr: The node of the Bleach AST that is a Self Expression node.
     * 
     * @return The value obtained from the visit (evaluation of) to a Self expression node of the Bleach AST.
     * 
     * @note This method is an overridden version of the "visitSelfExpr" method from the "ExprVisitor" struct.
     */
    std::any visitSelfExpr(std::shared_ptr<Self> expr) override{
      return lookUpVariable(expr->keyword, expr);
    }

    /**
     * @brief Visits a Set Expression node of the Bleach AST and performs the associated actions. 
     *
     * This method is responsible for visiting a Set Expression node of the Bleach AST and performing the
     * associated actions with this type of AST node.
     * 
     * @param expr: The node of the Bleach AST that is a Set Expression node.
     * 
     * @return The value obtained from the visit (evaluation of) to a Set expression node of the Bleach AST.
     * 
     * @note This method is an overridden version of the "visitSetExpr" method from the "ExprVisitor" struct.
     */
    std::any visitSetExpr(std::shared_ptr<Set> expr) override{
      std::any object = evaluate(expr->object);

      if(object.type() != typeid(std::shared_ptr<BleachInstance>)){
        throw BleachRuntimeError{expr->name, "Only instances of classes have fields."};
      }

      std::any value = evaluate(expr->value);
      std::any_cast<std::shared_ptr<BleachInstance>>(object)->set(expr->name, value);

      return value;
    }

    /**
     * @brief Visits a Super Expression node of the Bleach AST and performs the associated actions. 
     *
     * This method is responsible for visiting a Super Expression node of the Bleach AST and performing the
     * associated actions with this type of AST node.
     * 
     * @param expr: The node of the Bleach AST that is a Super Expression node.
     * 
     * @return The value obtained from the visit (evaluation of) to a Super expression node of the Bleach AST.
     * 
     * @note This method is an overridden version of the "visitSuperExpr" method from the "ExprVisitor" struct.
     */
    std::any visitSuperExpr(std::shared_ptr<Super> expr) override{
      int distance = locals[expr];

      auto superclass = std::any_cast<std::shared_ptr<BleachClass>>(environment->getAt("super", distance));
      auto object = std::any_cast<std::shared_ptr<BleachInstance>>(environment->getAt("self", distance - 1));

      std::shared_ptr<BleachFunction> method = superclass->findMethod(expr->method.lexeme);

      if(method == nullptr){
        throw BleachRuntimeError{expr->method, "Undefined property (field or method):" + expr->method.lexeme + "."};
      }

      return method->bind(object);
    }

    /**
     * @brief Visits a Ternary expression node of the Bleach AST and produces the corresponding value. 
     *
     * This method is responsible for visiting a Ternary expression node of the Bleach AST, producing a value
     * that corresponds to the type of expression present inside such Ternary expression node.
     * 
     * @param expr: The node of the Bleach AST that is a Grouping expression node.
     * 
     * @return The value obtained from the visit (evaluation) to a Ternary expression node of the Bleach AST.
     * 
     * @note This method is an overridden version of the "visitTernaryExpr" method from the "ExprVisitor"
     * struct.
     */
    std::any visitTernaryExpr(std::shared_ptr<Ternary> expr) override{
      if(isTruthy(evaluate(expr->condition))){
        return evaluate(expr->ifBranch);
      }else{
        return evaluate(expr->elseBranch);
      }

      // Unreachable
      return {};
    }

    /**
     * @brief Visits a Unary expression node of the Bleach AST and produces the corresponding value. 
     *
     * This method is responsible for visiting a Unary expression node of the Bleach AST, producing a value
     * that corresponds to the type of expression present inside such Unary expression node.
     * 
     * @param expr: The node of the Bleach AST that is a Unary expression node.
     * 
     * @return The value obtained from the visit (evaluation) to a Unary expression node of the Bleach AST.
     * 
     * @note This method is an overridden version of the "visitUnaryExpr" method from the "ExprVisitor" struct.
     */
    std::any visitUnaryExpr(std::shared_ptr<Unary> expr) override{
      std::any right = evaluate(expr->right);

      switch(expr->op.type){
        case(TokenType::BANG):
          return !isTruthy(right);
        case(TokenType::MINUS):
          checkNumberOperand(expr->op, right);
          return -std::any_cast<double>(right);
      }

      // Unreachable
      return {};
    }

    /**
     * @brief Visits a Variable expression node of the Bleach AST and produces the corresponding value. 
     *
     * This method is responsible for visiting a Variable expression node of the Bleach AST, producing a value
     * that corresponds to the type of expression present inside such Variable expression node and returning it.
     * 
     * @param expr: The node of the Bleach AST that is a Variable expression node.
     * 
     * @return The value obtained from the visit (evaluation) to a Variable expression node of the Bleach AST.
     * 
     * @note This method is an overridden version of the "visitVariableExpr" method from the "ExprVisitor" 
     * struct.
     */
    std::any visitVariableExpr(std::shared_ptr<Variable> expr) override{
      return lookUpVariable(expr->name, expr);
    }
};
