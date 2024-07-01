#pragma once

#include <functional>
#include <map>
#include <memory>
#include <vector>

#include "../interpreter/Interpreter.hpp"

class Resolver : public ExprVisitor, public StmtVisitor{
  private:
    Interpreter& interpreter;
    std::vector<std::map<std::string, bool>> scopes;

    void declare(const Token& name){
      if(scopes.empty()){
        return;
      }

      std::map<std::string, bool>& scope = scopes.back();
      scope[name.lexeme] = false;

      return;
    }

    void define(const Token& name){
      if(scopes.empty()){
        return;
      }

      scopes.back()[name.lexeme] = true;

      return;
    }

    void beginScope(){
      scopes.push_back(std::map<std::string, bool>{});

      return;
    }

    void endScope(){
      scopes.pop_back();

      return;
    }

    void resolve(const std::shared_ptr<Expr>& expression){
      expression->accept(*this);

      return;
    }

    void resolve(const std::shared_ptr<Stmt>& statement){
      statement->accept(*this);

      return;
    }

  public:
    Resolver(Interpreter& interpreter)
      : interpreter{interpreter}
    {}

    void resolve(const std::vector<std::shared_ptr<Stmt>>& statements){
      for(const std::shared_ptr<Stmt>& statement : statements){
        resolve(statement);
      }

      return;
    }

    std::any visitAssignExpr(std::shared_ptr<Assign> stmt) override{

      return {};
    }

    std::any visitBinaryExpr(std::shared_ptr<Binary> stmt) override{

      return {};
    }

    std::any visitCallExpr(std::shared_ptr<Call> stmt) override{

      return {};
    }

    std::any visitGroupingExpr(std::shared_ptr<Grouping> stmt) override{

      return {};
    }

    std::any visitLambdaFunctionExpr(std::shared_ptr<LambdaFunction> stmt) override{

      return {};
    }

    std::any visitLiteralExpr(std::shared_ptr<Literal> stmt) override{

      return {};
    }

    std::any visitLogicalExpr(std::shared_ptr<Logical> stmt) override{

      return {};
    }

    std::any visitTernaryExpr(std::shared_ptr<Ternary> stmt) override{

      return {};
    }

    std::any visitUnaryExpr(std::shared_ptr<Unary> stmt) override{

      return {};
    }

    std::any visitVariableExpr(std::shared_ptr<Variable> stmt) override{

      return {};
    }

    std::any visitBlockStmt(std::shared_ptr<Block> stmt) override{
      beginScope();
      resolve(stmt->statements);
      endScope();

      return {};
    }

    std::any visitDoWhileStmt(std::shared_ptr<DoWhile> stmt) override{

      return {};
    }

    std::any visitExpressionStmt(std::shared_ptr<Expression> stmt) override{

      return {};
    }

    std::any visitFunctionStmt(std::shared_ptr<Function> stmt) override{

      return {};
    }

    std::any visitIfStmt(std::shared_ptr<If> stmt) override{

      return {};
    }

    std::any visitPrintStmt(std::shared_ptr<Print> stmt) override{

      return {};
    }

    std::any visitReturnStmt(std::shared_ptr<Return> stmt) override{

      return {};
    }

    std::any visitVarStmt(std::shared_ptr<Var> stmt) override{
      declare(stmt->name);
      if(stmt->initializer != nullptr){ // If a value is assigned to the variable in its declaration.
        resolve(stmt->initializer);
      }
      define(stmt->name);

      return {};
    }

    std::any visitWhileStmt(std::shared_ptr<While> stmt) override{

      return {};
    }
};
