#pragma once

#include <any>
#include <memory>
#include <utility>
#include <vector>

#include "./Expr.hpp"


// Necessary forward declarations of certain structs so they can be used inside the 'StmtVisitor' struct below.
struct Block;
struct Expression; // Expression statement (Used in function and method calls).
struct If;
struct Print;
struct Var; // Variable declaration statement.

struct StmtVisitor{
  virtual std::any visitBlockStmt(std::shared_ptr<Block> stmt) = 0;
  virtual std::any visitExpressionStmt(std::shared_ptr<Expression> stmt) = 0;
  virtual std::any visitIfStmt(std::shared_ptr<If> stmt) = 0;
  virtual std::any visitPrintStmt(std::shared_ptr<Print> stmt) = 0;
  virtual std::any visitVarStmt(std::shared_ptr<Var> stmt) = 0;
  virtual ~StmtVisitor() = default;
};

struct Stmt{
  virtual std::any accept(StmtVisitor& visitor) = 0;
};

struct Block : Stmt, public std::enable_shared_from_this<Block>{
  const std::vector<std::shared_ptr<Stmt>> statements;

  Block(std::vector<std::shared_ptr<Stmt>> statements)
    : statements{std::move(statements)}
  {}

  std::any accept(StmtVisitor& visitor) override{
    return visitor.visitBlockStmt(shared_from_this());
  }
};

struct Expression : Stmt, public std::enable_shared_from_this<Expression>{
  const std::shared_ptr<Expr> expression;

  Expression(std::shared_ptr<Expr> expression)
    : expression{std::move(expression)}
  {}

  std::any accept(StmtVisitor& visitor) override{
    return visitor.visitExpressionStmt(shared_from_this());
  }
};

struct If : Stmt, public std::enable_shared_from_this<If>{
  const std::shared_ptr<Expr> ifCondition;
  const std::shared_ptr<Stmt> ifBranch;
  const std::vector<std::shared_ptr<Expr>> elifConditions;
  const std::vector<std::shared_ptr<Stmt>> elifBranches;
  const std::shared_ptr<Stmt> elseBranch;

  If(std::shared_ptr<Expr> ifCondition, std::shared_ptr<Stmt> ifBranch, std::vector<std::shared_ptr<Expr>> elifConditions, std::vector<std::shared_ptr<Stmt>> elifBranches, std::shared_ptr<Stmt> elseBranch)
    : ifCondition{std::move(ifCondition)}, ifBranch{std::move(ifBranch)}, elifConditions{std::move(elifConditions)}, elifBranches{std::move(elifBranches)}, elseBranch{std::move(elseBranch)}
  {}


  std::any accept(StmtVisitor& visitor) override{
    return visitor.visitIfStmt(shared_from_this());
  }
};

struct Print : Stmt, public std::enable_shared_from_this<Print>{
  const std::shared_ptr<Expr> expression;

  Print(std::shared_ptr<Expr> expression)
    : expression{std::move(expression)}
  {}

  std::any accept(StmtVisitor& visitor) override{
    return visitor.visitPrintStmt(shared_from_this());
  }
};

// Variable declaration statement.
struct Var : Stmt, public std::enable_shared_from_this<Var>{
  const Token name;
  const std::shared_ptr<Expr> initializer;

  Var(Token name, std::shared_ptr<Expr> initializer)
    : name{std::move(name)}, initializer{std::move(initializer)}
  {}

  std::any accept(StmtVisitor& visitor) override{
    return visitor.visitVarStmt(shared_from_this());
  }
};
