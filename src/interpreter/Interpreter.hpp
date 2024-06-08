#pragma once

#include <any>
#include <memory>
#include <string>
#include <utility>

#include "../error/BleachRuntimeError.hpp"
#include "../error/Error.hpp"
#include "../utils/Expr.hpp"

/**
 * @class Interpreter
 * @brief Performs the
 *
 * The Interpter class is responsible for...
 *
 * @note The interpreter
**/
class Interpreter : public ExprVisitor{
  private:
    /**
     * @brief Defines 
     *
     * This method
     * 
     * @param expr:
     * 
     * @return
     */
    void checkNumberOperand(const Token& op, const std::any& operand){
      if(operand.type() == typeid(double)){
        return;
      }

      throw BleachRuntimeError{op, "Operand must be a number."};
    }

    /**
     * @brief Defines 
     *
     * This method
     * 
     * @param expr:
     * 
     * @return
     */
    void checkNumberOperands(const Token &op, const std::any& left, const std::any& right){
      if(left.type() == typeid(double) && right.type() == typeid(double)){
        return;
      }

      throw BleachRuntimeError{op, "Operands must be numbers."};
    }

    /**
     * @brief Defines 
     *
     * This method
     * 
     * @param expr:
     * 
     * @return
     */
    std::any evaluate(std::shared_ptr<Expr> expr){
      return expr->accept(*this);
    }

    /**
     * @brief Defines 
     *
     * This method
     * 
     * @param expr:
     * 
     * @return
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
     * @brief Defines 
     *
     * This method
     * 
     * @param expr:
     * 
     * @return
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
     * @brief Defines 
     *
     * This method
     * 
     * @param expr:
     * 
     * @return
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
        std::string numberAsText = std::to_string(std::any_cast<double>(object));
        std::string::size_type dotPosition = numberAsText.find(".");
        bool allZeroes = true;

        for(std::string::size_type startPoint = dotPosition + 1; startPoint < numberAsText.length(); startPoint++){
          if(numberAsText[startPoint] != '0'){
            allZeroes = false;
            break;
          }
        }

        if(allZeroes){
          numberAsText = numberAsText.substr(0, dotPosition);
        }
        
        return numberAsText;
      }

      return "Error in stringify: object type not recognized.";
    }

  public:
    /**
     * @brief Defines 
     *
     * This method
     * 
     * @param expr:
     * 
     * @return
     */
    void interpret(std::shared_ptr<Expr> expression){
      try{
        std::any value = evaluate(expression);
        std::cout << stringify(value) << std::endl;
      }catch(BleachRuntimeError error){
        runtimeError(error);
      }

      return;
    }

    /**
     * @brief Defines 
     *
     * This method
     * 
     * @param expr:
     * 
     * @return
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
          throw BleachRuntimeError{expr->op, "Operands must be two numbers or two strings."};
        case(TokenType::MINUS):
          checkNumberOperands(expr->op, left, right);
          return std::any_cast<double>(left) - std::any_cast<double>(right); // If the cast does not work, it will throw a bad_cast error.
        case(TokenType::STAR):
          checkNumberOperands(expr->op, left, right);
          return std::any_cast<double>(left) * std::any_cast<double>(right); // Evaluate the case of iteracting nums and strings in order to extend the language.
        case(TokenType::SLASH):
          checkNumberOperands(expr->op, left, right);
          return std::any_cast<double>(left) / std::any_cast<double>(right); // If the cast does not work, it will throw a bad_cast error.
      }

      // Unreachable
      return {};
    }

    /**
     * @brief Defines 
     *
     * This method
     * 
     * @param expr:
     * 
     * @return
     * 
     * @note This method is an overridden version of the 'visitGroupingExpr' method from the 'ExprVisitor' struct.
     */
    std::any visitGroupingExpr(std::shared_ptr<Grouping> expr) override{
      return evaluate(expr->expression);
    }

    /**
     * @brief Defines 
     *
     * This method
     * 
     * @param expr:
     * 
     * @return
     * 
     * @note This method is an overridden version of the 'visitLiteralExpr' method from the 'ExprVisitor' struct.
     */
    std::any visitLiteralExpr(std::shared_ptr<Literal> expr) override{
      return expr->value;
    }

    /**
     * @brief Defines 
     *
     * This method
     * 
     * @param expr:
     * 
     * @return
     * 
     * @note This method is an overridden version of the 'visitTernaryExpr' method from the 'ExprVisitor' struct.
     */
    std::any visitTernaryExpr(std::shared_ptr<Ternary> expr) override{
      return {};
    }

    /**
     * @brief Defines 
     *
     * This method
     * 
     * @param expr:
     * 
     * @return
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
};
