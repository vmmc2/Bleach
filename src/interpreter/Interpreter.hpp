#pragma once

#include <any>
#include <memory>
#include <string>
#include <utility>

#include "../utils/Expr.hpp"


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
    bool isTruthy(const std::any& object){
      if(object.type() == typeid(nullptr)){
        return false;
      }
      if(object.type() == typeid(bool)){
        return std::any_cast<bool>(object);
      }

      return true;
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
     * 
     * @note This method is an overridden version of the 'visitBinaryExpr' method from the 'ExprVisitor' struct.
     */
    std::any visitBinaryExpr(std::shared_ptr<Binary> expr) override{
      std::any left = evaluate(expr->left);
      std::any right = evaluate(expr->right);

      switch(expr->op.type){
        case(TokenType::PLUS):
          if(left.type() == typeid(double) && right.type() == typeid(double)){
            return std::any_cast<double>(left) + std::any_cast<double>(right);
          }
          if(left.type() == typeid(std::string) && right.type() == typeid(std::string)){
            return std::any_cast<std::string>(left) + std::any_cast<std::string>(right);
          }
          break;
        case(TokenType::MINUS):
          return std::any_cast<double>(left) - std::any_cast<double>(right); // If the cast does not work, it will throw a bad_cast error.
        case(TokenType::STAR):
          return std::any_cast<double>(left) * std::any_cast<double>(right); // Evaluate the case of iteracting nums and strings in order to extend the language.
        case(TokenType::SLASH):
          return std::any_cast<double>(left) / std::any_cast<double>(right); // If the cast does not work, it will throw a bad_cast error.
      }
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
          return -std::any_cast<double>(right);
      }

      // Unreachable
      return {};
    }
};
