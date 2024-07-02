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

    void resolve(const std::vector<std::shared_ptr<Stmt>>& statements){
      for(const std::shared_ptr<Stmt>& statement : statements){
        resolve(statement);
      }

      return;
    }

    void resolveFunction(std::shared_ptr<Function> function){
      beginScope();
      
      for(const Token& parameter : function->parameters){
        declare(parameter);
        define(parameter);
      }
      resolve(function->body);

      endScope();

      return;
    }

    void resolveLocal(std::shared_ptr<Expr> expr, const Token& name){
      for(int i = scopes.size() - 1; i >= 0; i--){
        if(scopes[i].find(name.lexeme) != scopes[i].end()){
          interpreter.resolve(expr, scopes.size() - 1 - i); // This tells the interpreter how many hops it will need to do in order to find the variable declaration to which this Variable expression is referring to. 
          // We pass the Variable expression pointer because if the token was passed, it would not work properly. A pointer is unique. A Token is not.
          return;
        }
      }

      return;
    }

  public:
    Resolver(Interpreter& interpreter)
      : interpreter{interpreter}
    {}

    std::any visitAssignExpr(std::shared_ptr<Assign> expr) override{
      resolve(expr->value); // First, the resolver needs to resolve the r-value of the assignment expression.
      resolveLocal(expr, expr->name); // Then, the resolver resolves the l-value of the assignment expression. This is used to figure out to which variable the l-value is referring to.

      return {};
    }

    std::any visitBinaryExpr(std::shared_ptr<Binary> expr) override{

      return {};
    }

    std::any visitCallExpr(std::shared_ptr<Call> expr) override{

      return {};
    }

    std::any visitGroupingExpr(std::shared_ptr<Grouping> expr) override{

      return {};
    }

    std::any visitLambdaFunctionExpr(std::shared_ptr<LambdaFunction> expr) override{

      return {};
    }

    std::any visitLiteralExpr(std::shared_ptr<Literal> expr) override{

      return {};
    }

    std::any visitLogicalExpr(std::shared_ptr<Logical> expr) override{

      return {};
    }

    std::any visitTernaryExpr(std::shared_ptr<Ternary> expr) override{

      return {};
    }

    std::any visitUnaryExpr(std::shared_ptr<Unary> expr) override{

      return {};
    }

    std::any visitVariableExpr(std::shared_ptr<Variable> expr) override{
      if(!scopes.empty()){
        auto& scope = scopes.back();
        auto elem = scope.find(expr->name.lexeme);
        if(elem != scope.end() && elem->second == false){ // Remember: If the interpreter is visiting this node, then its visiting a name that references a variable inside an expression.
          error(expr->name, "Cannot read local variable in its own initializer."); // If the variable that it refers to has a false value associated to it in the scope, then it means we are inside an initializer using a variable that is refering to the variable that is being declared. Not allowed.
        }
      }

      resolveLocal(expr, expr->name);

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
      declare(stmt->name);
      define(stmt->name);

      resolveFunction(stmt);

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
      declare(stmt->name); // First, a variable is declared. (Its associated value in the scope is false).
      if(stmt->initializer != nullptr){ // If an expression is assigned to the variable in its declaration, then it needs to be resolved.
        resolve(stmt->initializer); // We then need to resolve the initializer expression. However, it might be possible that the initializer refers to a variable that has the same name as the variable being declared. If that's the case, an error is reported since this is not allowed.
      }
      define(stmt->name); // After declaring the variable, resolving its possible initializer, we can define it (Its associated value in the scope is now true).

      return {};
    }

    std::any visitWhileStmt(std::shared_ptr<While> stmt) override{

      return {};
    }
};
