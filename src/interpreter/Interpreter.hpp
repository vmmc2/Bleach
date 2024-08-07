#pragma once

#include <any>
#include <cmath>
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
     * @param op: The token that represents the unary operator ("-") (TokenType::MINUS). This variable is of type
     * const Token&.
     * @param operand: The value of the right operand of the unary operator ("-"). This variable is of type
     * std::any&.
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
     * @param op: The token that represents any binary operator that is not "==" or "!=". This variable is of 
     * type const Token&.
     * @param left: The value of the left operand of the binary operator. This variable is of type std::any&.
     * @param right: The value of the right operand of the binary operator. This variable is of type std::any&.
     * 
     * @return Nothing (void).
     * 
     * @note If the provided operands do not satisfy the conditions explained above, then an instance of a 
     * BleachRuntimeError is thrown by the interpreter.
     */
    void checkNumberOperands(const Token &op, const std::any& left, const std::any& right){
      if(left.type() == typeid(double) && right.type() == typeid(double)){
        return;
      }

      throw BleachRuntimeError{op, "Operands must be numbers."};
    }

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
     * This method works as a helper method responsible for receiving a Expr node of the AST, calling its accept
     * method and passing the current instance of the interpreter to it, so the node can call its appropriate
     * visit method and evaluate its "inner" result.
     * 
     * @param expr: A node of the AST (Abstract Syntax Tree) that represents an Expr node from the Bleach
     * language. Such variable is of type std::shared_ptr<Expr>.
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
     * This method works as a helper method responsible for receiving a Stmt node of the AST, calling its accept
     * method and passing the current instance of the interpreter to it, so the node can call its appropriate
     * visit method and execute its "inner" functionality.
     * 
     * @param stmt: A node of an AST (Abstract Syntax Tree) that represents a Stmt node from the Bleach 
     * language. Such variable is of type std::shared_ptr<Stmt>.
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
     * This method is responsible for checking whether the operands of the following binary operator ("==") are
     * of type double.
     * 
     * @param left: The value of the left operand of the "==" operator. This variable is of type std::any&.
     * @param right: The value of the right operand of the "==" operator. This variable is of type std::any&.
     * 
     * @return A boolean that signal whether the two provided values are equal or not.
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
     * "truthy" or not. For more context, Bleach follows Ruby convention: false and nil are considered
     * to be "falsey" values. Every other value is considered to be "truthy".
     * 
     * @param object: The value of a Bleach object which will be checked to see if it's "truthy" or not. This
     * variable is of type std::any&.
     * 
     * @return A boolean that signal whether or not a value is considered to be "truthy" or not.
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

    std::any lookUpVariable(const Token& name, std::shared_ptr<Expr> expr){
      auto elem = locals.find(expr);
      if(elem != locals.end()){ // If the Variable expression has been found here, then it means that it is a local variable.
        int distance = elem->second;
        return environment->getAt(name.lexeme, distance);
      }else{ // Otherwise, it is assumed that the variable was declared in the global scope.
        return globals->get(name); // Global variable are treated in a special way. If a global variable is not found, the a runtime error is thrown by the BLEACH Interpreter.
      }
    }

    /**
     * @brief Produces a string that represents the value present in the provided Bleach object. 
     *
     * This method generates a string representation of the value present inside the provided Bleach object.
     * 
     * @param object: The value of a Bleach object which the interpreter will try to convert to a string. This
     * variable is of type std::any&.
     * 
     * @return A string (std::string) representation of the value present inside the provided Bleach object.
     * 
     * @note If the value of the Bleach object is not of any of the supported types, then this function will 
     * return a string containing an error message.
     */
    std::string stringify(const std::any& object){
      if(object.type() == typeid(nullptr)){
        return "nil";
      }
      if(object.type() == typeid(bool)){
        return std::any_cast<bool>(object) ? "true" : "false";
      }
      if(object.type() == typeid(std::string)){
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
        return std::any_cast<std::shared_ptr<BleachInstance>>(object)->toString();
      }
      if(object.type() == typeid(std::shared_ptr<BleachLambdaFunction>)){
        return std::any_cast<std::shared_ptr<BleachLambdaFunction>>(object)->toString();
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

      return "Error in stringify: object type not recognized.";
    }

  public:
    Interpreter(){
      globals->define("std::chrono::clock", std::make_shared<NativeClock>());
      globals->define("std::io::readLine", std::make_shared<NativeReadLine>());
      globals->define("std::io::print", std::make_shared<NativePrint>());
      // globals->define("std::io::fileRead", std::make_shared<NativeFileRead>());
      // globals->define("std::io::fileWrite", std::make_shared<NativeFileWrite>());
      globals->define("std::math::abs", std::make_shared<NativeAbsoluteValue>());
      globals->define("std::math::pow", std::make_shared<NativeExponentiation>());
      globals->define("std::math::log", std::make_shared<NativeLogarithm>());
      globals->define("std::math::sqrt", std::make_shared<NativeSquareRoot>());
      globals->define("std::random::random", std::make_shared<NativeRandom>());
    }

    /**
     * @brief Interprets/Executes the list of statements that represent a Bleach program (remember that each
     * statement represents an AST following of the Bleach language), provided by the parser. 
     *
     * This method is responsible for interpreting/executing the list of statements generated by the parser 
     * (that represents a Bleach program), where each statement is an AST (Abstract Syntax Tree).
     * 
     * @param statements: The list of statements, where each statement is an AST (Abstract Syntax Tree), that 
     * represents a program written in the Bleach lang. Such variable is of type std::vector<shared_ptr<Stmt>>&.
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

    void resolve(std::shared_ptr<Expr> expr, int depth){
      locals[expr] = depth;

      return;
    }

    /**
     * @brief Creates a new environment for a block statement that is about to be executed and executes each of
     * its statements.
     *
     * This method acts as an auxiliary method to the 'visitBlockStmt' method from this same class. This method 
     * is responsible for creating a brand new environment for the block statement that is about to be visited
     * (during this creation, it stores the enclosing environment of the block inside the newly created environment
     * for the block) and also for executing each of the statements that are present inside this block.
     * 
     * @param statements: The list of statements that the block statement that is about executed contains. This
     * variable is of type const std::vector<std::shared_ptr<Stmt>>&.
     * @param enviroment: The just created environment that represents the lexical scope (static scope) of the
     * block statement that is about to be executed by the interpreter. This variable is of type 
     * std::shared_ptr<Environment>.
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
     * @param stmt: The node of the Bleach AST that is a Block Statement node. This variable is of type 
     * std::shared_ptr<Block>.
     * 
     * @return Nothing ({}).
     * 
     * @note This method is an overridden version of the 'visitBlockStmt' method from the 'StmtVisitor' struct.
     */
    std::any visitBlockStmt(std::shared_ptr<Block> stmt) override{
      executeBlock(stmt->statements, std::make_shared<Environment>(environment)); // In order to execute a 'Block' statement, the interpreter needs to create an environment that represents the lexical/static scope of the block and it also needs to execute each statement inside the block.

      return {};
    }

    std::any visitBreakStmt(std::shared_ptr<Break> stmt) override{
      throw BleachBreak{};
    }

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

    std::any visitContinueStmt(std::shared_ptr<Continue> stmt) override{
      throw BleachContinue{};
    }

    /**
     * @brief Visits a DoWhile Statement node of the Bleach AST and performs the associated actions. 
     *
     * This method is responsible for visiting a DoWhile Statement node of the Bleach AST and performing the
     * associated actions with this type of AST node.
     * 
     * @param stmt: The node of the Bleach AST that is a DoWhile Statement node. This variable is of type 
     * std::shared_ptr<DoWhile>.
     * 
     * @return Nothing ({}).
     * 
     * @note This method is an overridden version of the 'visitDoWhileStmt' method from the 'StmtVisitor' 
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
     * @param stmt: The node of the Bleach AST that is a Expression Statement node. This variable is of type 
     * std::shared_ptr<Expression>.
     * 
     * @return Nothing ({}).
     * 
     * @note This method is an overridden version of the 'visitExpressionStmt' method from the 'StmtVisitor'
     * struct.
     */
    std::any visitExpressionStmt(std::shared_ptr<Expression> stmt) override{
      evaluate(stmt->expression);

      return {};
    }

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
     * @brief Visits a Function Statement node of the Bleach AST and performs the associated actions. 
     *
     * This method is responsible for visiting a Function Statement node of the Bleach AST and performing the
     * associated actions with this type of AST node.
     * 
     * @param stmt: The node of the Bleach AST that is a Function Statement node. This variable is of type 
     * std::shared_ptr<Function>.
     * 
     * @return Nothing ({}).
     * 
     * @note This method is an overridden version of the 'visitFunctionStmt' method from the 'StmtVisitor' 
     * struct.
     * Moreover, pay attention to the following fact: When the interpreter visits a Function Statement node, it
     * creates an instance of a "BleachFunction" object and stores it in the current environment. Moreover, since
     * functions in Bleach must hold on to their parent environment, during the instance creation, the interpreter
     * assigns the current environment that it currently is at as parent environment of the "BleachFunction" 
     * instance. However, I think that this is not 100% correct. Because the parent environment is being defined
     * at runtime, and as we know, the state of an environment can change during runtime. But, this will be
     * evaluated more carefully later.
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
     * @param stmt: The node of the Bleach AST that is an If Statement node. This variable is of type 
     * std::shared_ptr<Expression>.
     * 
     * @return Nothing ({}).
     * 
     * @note This method is an overridden version of the 'visitIfStmt' method from the 'StmtVisitor' struct.
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
     * @param stmt: The node of the Bleach AST that is a Print Statement node. This variable is of type 
     * std::shared_ptr<Print>.
     * 
     * @return Nothing ({}).
     * 
     * @note This method is an overridden version of the 'visitPrintStmt' method from the 'StmtVisitor' struct.
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
     * @param stmt: The node of the Bleach AST that is a Return Statement node. This variable is of type 
     * std::shared_ptr<Return>.
     * 
     * @return Nothing ({}).
     * 
     * @note This method is an overridden version of the 'visitReturnStmt' method from the 'StmtVisitor' struct.
     * Moreover, this method will always throw a value. The value is the one generated when evaluating the 
     * expression that might be present"return" statement. If there is no expression, then this means the
     * produced value is nil (C++ nullptr).
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
     * @param stmt: The node of the Bleach AST that is a Variable Declaration Statement node. This variable is 
     * of type std::shared_ptr<Var>.
     * 
     * @return Nothing ({}).
     * 
     * @note This method is an overridden version of the 'visitVarStmt' method from the 'StmtVisitor' struct.
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
     * @param stmt: The node of the Bleach AST that is a While Statement node. This variable is of type 
     * std::shared_ptr<While>.
     * 
     * @return Nothing ({}).
     * 
     * @note This method is an overridden version of the 'visitWhileStmt' method from the 'StmtVisitor' struct.
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
     * @param expr: The node of the Bleach AST that is a Assign expression node. This variable is of type 
     * std::shared_ptr<Assign>.
     * 
     * @return The value obtained from the visit (evaluation) to a Assign expression node of the Bleach AST.
     * 
     * @note This method is an overridden version of the 'visitAssignExpr' method from the 'ExprVisitor' struct.
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
     * @param expr: The node of the Bleach AST that is a Binary expression node. This variable is of type 
     * std::shared_ptr<Binary>.
     * 
     * @return The value obtained from the visit (evaluation) to a Binary expression node of the Bleach AST.
     * 
     * @note This method is an overridden version of the 'visitBinaryExpr' method from the 'ExprVisitor' struct.
     */
    std::any visitBinaryExpr(std::shared_ptr<Binary> expr) override{
      std::any left = evaluate(expr->left);
      std::any right = evaluate(expr->right);

      switch(expr->op.type){
        case(TokenType::GREATER):
          checkNumberOperands(expr->op, left, right);
          return std::any_cast<double>(left) > std::any_cast<double>(right);
        case(TokenType::GREATER_EQUAL):
          checkNumberOperands(expr->op, left, right);
          return std::any_cast<double>(left) >= std::any_cast<double>(right);
        case(TokenType::LESS):
          checkNumberOperands(expr->op, left, right);
          return std::any_cast<double>(left) < std::any_cast<double>(right);
        case(TokenType::LESS_EQUAL):
          checkNumberOperands(expr->op, left, right);
          return std::any_cast<double>(left) <= std::any_cast<double>(right);
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
          if((left.type() == typeid(std::string) || right.type() == typeid(double))){
            return std::any_cast<std::string>(left) + formatDouble(std::any_cast<double>(right));
          }
          throw BleachRuntimeError{expr->op, "Operands must be two numbers or two strings or one number and one string."};
        case(TokenType::MINUS):
          checkNumberOperands(expr->op, left, right);
          return std::any_cast<double>(left) - std::any_cast<double>(right); // If the cast does not work, it will throw a bad_cast error.
        case(TokenType::STAR):
          checkNumberOperands(expr->op, left, right);
          return std::any_cast<double>(left) * std::any_cast<double>(right); // Evaluate the case of iteracting nums and strings in order to extend the language.
        case(TokenType::SLASH):
          checkNumberOperands(expr->op, left, right);
          checkZeroDivisor(right, expr->op);
          return std::any_cast<double>(left) / std::any_cast<double>(right); // If the cast does not work, it will throw a bad_cast error.
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
     * @param expr: The node of the Bleach AST that is a Call expression node. This variable is of type 
     * std::shared_ptr<Call>.
     * 
     * @return The value obtained from the visit (evaluation) to a Call expression node of the Bleach AST.
     * 
     * @note This method is an overridden version of the 'visitCallExpr' method from the 'ExprVisitor' struct.
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

      throw BleachRuntimeError{expr->paren, "Can only call classes, functions, lambda functions, methods and native functions."};
    }

    std::any visitGetExpr(std::shared_ptr<Get> expr) override{
      std::any object = evaluate(expr->object);

      if(object.type() == typeid(std::shared_ptr<BleachInstance>)){
        return std::any_cast<std::shared_ptr<BleachInstance>>(object)->get(expr->name);
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
     * @param expr: The node of the Bleach AST that is a Grouping expression node. This variable is of type 
     * std::shared_ptr<Grouping>.
     * 
     * @return The value obtained from the visit (evaluation) to a Grouping expression node of the Bleach AST.
     * 
     * @note This method is an overridden version of the 'visitGroupingExpr' method from the 'ExprVisitor' 
     * struct.
     */
    std::any visitGroupingExpr(std::shared_ptr<Grouping> expr) override{
      return evaluate(expr->expression);
    }

    std::any visitLambdaFunctionExpr(std::shared_ptr<LambdaFunction> expr) override{
      return std::make_shared<BleachLambdaFunction>(expr, environment);
    }

    /**
     * @brief Visits a Literal expression node of the Bleach AST and produces the corresponding value. 
     *
     * This method is responsible for visiting a Literal expression node of the Bleach AST, producing a value
     * that corresponds to the type of expression present inside such Literal expression node.
     * 
     * @param expr: The node of the Bleach AST that is a Literal expression node. This variable is of type 
     * std::shared_ptr<Literal>.
     * 
     * @return The value obtained from the visit (evaluation) to a Literal expression node of the Bleach AST.
     * 
     * @note This method is an overridden version of the 'visitLiteralExpr' method from the 'ExprVisitor' 
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
     * @param expr: The node of the Bleach AST that is a Binary expression node. This variable is of type 
     * std::shared_ptr<Logical>.
     * 
     * @return The value obtained from the visit (evaluation) to a Logical expression node of the Bleach AST.
     * 
     * @note This method is an overridden version of the 'visitLogicalExpr' method from the 'ExprVisitor' 
     * struct. Pay attention to the fact that logical operators do not return "true" or "false". Instead, they
     * return the value itself. Moreover, note that this method applies the idea of short-circuit by first
     * evaluating the left operand of the logical expression. After that, it checks the operator type and if
     * its possible to short-circuit. If that's the case, it prematurely returns the value produced by the 
     * evaluation of the left expression/operand. Otherwise, it evaluates the right expression/operand and
     * returns its value.
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

    std::any visitSelfExpr(std::shared_ptr<Self> expr) override{
      return lookUpVariable(expr->keyword, expr);
    }

    std::any visitSetExpr(std::shared_ptr<Set> expr) override{
      std::any object = evaluate(expr->object);

      if(object.type() != typeid(std::shared_ptr<BleachInstance>)){
        throw BleachRuntimeError{expr->name, "Only instances of classes have fields."};
      }

      std::any value = evaluate(expr->value);
      std::any_cast<std::shared_ptr<BleachInstance>>(object)->set(expr->name, value);

      return value;
    }

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
     * @param expr: The node of the Bleach AST that is a Grouping expression node. This variable is of type 
     * std::shared_ptr<Ternary>.
     * 
     * @return The value obtained from the visit (evaluation) to a Ternary expression node of the Bleach AST.
     * 
     * @note This method is an overridden version of the 'visitTernaryExpr' method from the 'ExprVisitor' 
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
     * @param expr: The node of the Bleach AST that is a Unary expression node. This variable is of type 
     * std::shared_ptr<Unary>.
     * 
     * @return The value obtained from the visit (evaluation) to a Unary expression node of the Bleach AST.
     * 
     * @note This method is an overridden version of the 'visitUnaryExpr' method from the 'ExprVisitor' struct.
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
     * @param expr: The node of the Bleach AST that is a Variable expression node. This variable is of type 
     * std::shared_ptr<Variable>.
     * 
     * @return The value obtained from the visit (evaluation) to a Variable expression node of the Bleach AST.
     * 
     * @note This method is an overridden version of the 'visitVariableExpr' method from the 'ExprVisitor' 
     * struct.
     */
    std::any visitVariableExpr(std::shared_ptr<Variable> expr) override{
      return lookUpVariable(expr->name, expr);
    }
};
