#pragma once

#include <any>
#include <memory>
#include <utility>
#include <vector>

#include "./Token.hpp"

// Necessary forward declarations of certain structs so they can be used inside the 'ExprVisitor' struct below.
struct Binary;
struct Grouping;
struct Literal;
struct Ternary;
struct Unary;

/**
 * @struct ExprVisitor
 * 
 * @brief Defines an interface for visiting different types of nodes from the Bleach AST (Abstract Syntax Tree).
 *
 * The ExprVisitor struct defines an abstract class which is responsible for working as an interface whose
 * functions are responsible for visiting the different expression nodes of the Bleach language AST (Abstract
 * Syntax Tree). Such struct has a set of pure virtual functions that represent different operations on the 
 * mentioned expression nodes. This same struct also has a virtual destructor that ensures the destructor of
 * a derived class is called correctly when an object is deleted through a pointer to the base class.
 * In practice, each of the expression nodes are going to be visited by an instance of a class that derives
 * from 'ExprVisitor'.
 */
struct ExprVisitor{
  virtual std::any visitBinaryExpr(std::shared_ptr<Binary> expr) = 0;
  virtual std::any visitGroupingExpr(std::shared_ptr<Grouping> expr) = 0;
  virtual std::any visitLiteralExpr(std::shared_ptr<Literal> expr) = 0;
  virtual std::any visitTernaryExpr(std::shared_ptr<Ternary> expr) = 0;
  virtual std::any visitUnaryExpr(std::shared_ptr<Unary> expr) = 0;
  virtual ~ExprVisitor() = default;
};

/**
 * @struct Expr
 * @brief Represents 
 *
 * The Expr struct
 */
struct Expr{
  virtual std::any accept(ExprVisitor& visitor) = 0;
};

/**
 * @struct Binary
 * @brief Represents 
 *
 * The Binary struct
 */
struct Binary : Expr, public std::enable_shared_from_this<Binary>{
  const std::shared_ptr<Expr> left;
  const Token op;
  const std::shared_ptr<Expr> right;

  Binary(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right)
    : left{std::move(left)}, op{std::move(op)}, right{std::move(right)} 
  {}

  std::any accept(ExprVisitor& visitor) override{
    return visitor.visitBinaryExpr(shared_from_this());
  }
};

/**
 * @struct Grouping
 * @brief Represents 
 *
 * The Grouping struct
 */
struct Grouping : Expr, public std::enable_shared_from_this<Grouping>{
  const std::shared_ptr<Expr> expression;

  Grouping(std::shared_ptr<Expr> expression)
    : expression{std::move(expression)}
  {}

  std::any accept(ExprVisitor& visitor) override{
    return visitor.visitGroupingExpr(shared_from_this());
  }
};

/**
 * @struct Literal
 * @brief Represents 
 *
 * The Literal struct
 */
struct Literal : Expr, public std::enable_shared_from_this<Literal>{
  std::any value;

  Literal(std::any value)
    : value{std::move(value)}
  {}

  std::any accept(ExprVisitor& visitor) override{
    return visitor.visitLiteralExpr(shared_from_this());
  }
};

/**
 * @struct Ternary
 * @brief Represents 
 *
 * The Ternary struct
 */
struct Ternary : Expr, public std::enable_shared_from_this<Ternary>{
  const std::shared_ptr<Expr> condition;
  const std::shared_ptr<Expr> truthyResult;
  const std::shared_ptr<Expr> falsyResult;

  Ternary(std::shared_ptr<Expr> condition, std::shared_ptr<Expr> truthyResult, std::shared_ptr<Expr> falsyResult)
    : condition{std::move(condition)}, truthyResult{std::move(truthyResult)}, falsyResult{std::move(falsyResult)}
  {}

  std::any accept(ExprVisitor& visitor) override{
    return visitor.visitTernaryExpr(shared_from_this());
  }
};

/**
 * @struct Unary
 * @brief Represents 
 *
 * The Unary struct
 */
struct Unary : Expr, public std::enable_shared_from_this<Unary>{
  const Token op;
  const std::shared_ptr<Expr> right;

  Unary(Token op, std::shared_ptr<Expr> right)
    : op{std::move(op)}, right{std::move(right)}
  {}

  std::any accept(ExprVisitor& visitor) override{
    return visitor.visitUnaryExpr(shared_from_this());
  }
};
