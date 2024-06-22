#pragma once

#include <any>
#include <memory>
#include <utility>
#include <vector>

#include "./Token.hpp"


// Necessary forward declarations of certain structs so they can be used inside the 'ExprVisitor' struct below.
struct Assign;
struct Binary;
struct Grouping;
struct Literal;
struct Logical;
struct Ternary;
struct Unary;
struct Variable;

/**
 * @struct ExprVisitor
 * 
 * @brief Defines an interface for visiting different types of expression nodes from the Bleach AST (Abstract 
 * Syntax Tree).
 *
 * The ExprVisitor struct defines an abstract struct that is responsible for working as an interface whose
 * functions are responsible for visiting the different expression nodes of the Bleach language AST (Abstract
 * Syntax Tree). Such struct has a set of pure virtual functions that represent different operations on the 
 * mentioned expression nodes. This same struct also has a virtual destructor that ensures the destructor of
 * a derived class/struct is called correctly when an object is deleted through a pointer to the base 
 * class/struct. In practice, each of the expression nodes are going to be visited by an instance of a 
 * struct/class that derives from 'ExprVisitor'.
 */
struct ExprVisitor{
  virtual std::any visitAssignExpr(std::shared_ptr<Assign> expr) = 0;
  virtual std::any visitBinaryExpr(std::shared_ptr<Binary> expr) = 0;
  virtual std::any visitGroupingExpr(std::shared_ptr<Grouping> expr) = 0;
  virtual std::any visitLiteralExpr(std::shared_ptr<Literal> expr) = 0;
  virtual std::any visitLogicalExpr(std::shared_ptr<Logical> expr) = 0;
  virtual std::any visitTernaryExpr(std::shared_ptr<Ternary> expr) = 0;
  virtual std::any visitUnaryExpr(std::shared_ptr<Unary> expr) = 0;
  virtual std::any visitVariableExpr(std::shared_ptr<Variable> expr) = 0;
  virtual ~ExprVisitor() = default;
};

/**
 * @struct Expr
 * 
 * @brief Defines an abstract struct which will be used as a base struct to implement derived structs that 
 * represent different kinds of expression nodes in the AST (Abstract Syntax Tree) of the Bleach language.
 *
 * The Expr struct defines an abstract struct that is responsible for working as the base struct from which all
 * structs that represent different types of expression AST nodes will derive from. This struct has only a pure
 * virtual method called 'accept'. This method will be overridden by the derived structs where each kind of 
 * struct will have its own implementation for such method.
 */
struct Expr{
  virtual std::any accept(ExprVisitor& visitor) = 0;
};

/**
 * @struct Assign
 * 
 * @brief Defines a struct to represent an assignment expression node from the AST of the Bleach language.
 *
 * The Assign struct defines a struct that is responsible for representing an assignment expression node from 
 * the AST (Abstract Syntax Tree) of the Bleach language. Such struct has two attributes: a token that 
 * represents an identifier (a place, in-memory, which the assignment is going to be made to) and an expression
 * whose value is going to be assigned to such place in-memory.
 * When such expression is evaluated by the 'Interpreter' class, it "evaluates" the left-hand side operand to
 * find out to which variable declaration that token (identifier) is referring to. Then, it evaluates the 
 * right-hand side operand to produce a value. Finally, it assigns such produced value to the referred variable
 * and also returns the produced value, since an assignment in Bleach is an expression.
 */
struct Assign : Expr, public std::enable_shared_from_this<Assign>{
  const Token name;
  const std::shared_ptr<Expr> value;

  /**
   * @brief Constructs an Assign node of the Bleach AST (Abstract Syntax Tree). 
   *
   * This constructor initializes an Assign object with a token that corresponds to an identifier (the left-hand
   * side of the assignment expression) and an expression (the right-hand side of the assignment expression) 
   * that will later be evaluated to produce a value.
   * 
   * @param name: The left-hand side operand (represented by the Token type) of the assignment operation. Also
   * known as "l-value"
   * @param value: The right-hand side operand (represented by the std::shared_ptr<Expr> type) of the assignment
   * operation. Also known as "r-value".
  **/
  Assign(Token name, std::shared_ptr<Expr> value)
    : name{std::move(name)}, value{std::move(value)}
  {}

  std::any accept(ExprVisitor& visitor) override{
    return visitor.visitAssignExpr(shared_from_this());
  }
};

/**
 * @struct Binary
 * 
 * @brief Defines a struct to represent a binary expression node from the AST of the Bleach language.
 *
 * The Binary struct defines a struct that is responsible for representing a binary expression node from the
 * AST (Abstract Syntax Tree) of the Bleach language. Such struct has three attributes: two that represents the
 * operands of a binary expression ('left' and 'right') and another that represents the kind of operation that 
 * will be performed on the two operands ('op').
 */
struct Binary : Expr, public std::enable_shared_from_this<Binary>{
  const std::shared_ptr<Expr> left;
  const Token op;
  const std::shared_ptr<Expr> right;

  /**
   * @brief Constructs a Binary node of the Bleach AST (Abstract Syntax Tree). 
   *
   * This constructor initializes a Binary object with the left operand, right operand and the operator that
   * represents the binary operation about to be performed on these operands.
   *
   * @param left: The left operand (represented by the std::shared_ptr<Expr> tyoe) of the binary 
   * operation/operator.
   * @param op: The operator that represents the binary operation about to be performed on the two operands 
   * (represented by the Token type).
   * @param right: The right operand (represented by the std::shared_ptr<Expr> type) of the binary 
   * operation/operator.
  **/
  Binary(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right)
    : left{std::move(left)}, op{std::move(op)}, right{std::move(right)} 
  {}

  std::any accept(ExprVisitor& visitor) override{
    return visitor.visitBinaryExpr(shared_from_this());
  }
};

/**
 * @struct Grouping
 * 
 * @brief Defines a struct to represent a grouping expression node from the AST of the Bleach language.
 *
 * The Grouping struct defines a struct to represent a grouping expression node from the AST (Abstract Syntax 
 * Tree) of the Bleach language. A grouping expression is just an expression that is enclosed by parentheses.
 * This struct has only one field called 'expression' that represents the expression inside the parentheses.
 */
struct Grouping : Expr, public std::enable_shared_from_this<Grouping>{
  const std::shared_ptr<Expr> expression;

  /**
   * @brief Constructs a Grouping node of the Bleach AST (Abstract Syntax Tree). 
   *
   * This constructor initializes a Grouping object with the a pointer to the expression that is present inside
   * the parentheses.
   *
   * @param expression: The expression that is presented inside the parentheses of a grouping node. This inside
   * expression is represented by the std::shared_ptr<Expr> type.
  **/
  Grouping(std::shared_ptr<Expr> expression)
    : expression{std::move(expression)}
  {}

  std::any accept(ExprVisitor& visitor) override{
    return visitor.visitGroupingExpr(shared_from_this());
  }
};

/**
 * @struct Literal
 * 
 * @brief Defines a struct to represent a literal expression node from the AST of the Bleach language.
 *
 * The Literal struct defines a struct to represent a literal expression node from the AST (Abstract Syntax
 * Tree) of the Bleach language. A literal expression is an expression that is just a value. This struct has
 * only one attribute called 'value' that represents the literal value present inside such struct.
 */
struct Literal : Expr, public std::enable_shared_from_this<Literal>{
  std::any value;

  /**
   * @brief Constructs a Literal node of the Bleach AST (Abstract Syntax Tree). 
   *
   * This constructor initializes a Literal object with the value that such literal represents.
   *
   * @param value: The value that was generated by the literal (represented by the std::any type).
  **/
  Literal(std::any value)
    : value{std::move(value)}
  {}

  std::any accept(ExprVisitor& visitor) override{
    return visitor.visitLiteralExpr(shared_from_this());
  }
};

/**
 * @struct Logical
 * 
 * @brief Defines a struct to represent a logical expression node from the AST of the Bleach language. Remember
 * that a logical expression node is just a variation of a binary expression node.
 *
 * The Logical struct defines a struct that is responsible for representing a logical binary expression node 
 * from the AST (Abstract Syntax Tree) of the Bleach language. Such struct has three attributes: two that 
 * represents the operands of a logical expression ('left' and 'right') and another that represents the kind 
 * of operation that will be performed on the two operands ('op').
 * 
 * @note: It's also important to make clear that there's an important difference between binary operators and 
 * logical operators: The logical ones can short-circuit. In other words, if, after evaluating the left operand 
 * of a logical operation, the result is already known, then the right operand is not even evaluated.
 * It's also important to mention that the "and" operator has a higher precedence compared to the "or" operator.
 */
struct Logical : Expr, public std::enable_shared_from_this<Logical>{
  const std::shared_ptr<Expr> left;
  const Token op;
  const std::shared_ptr<Expr> right;

  /**
   * @brief Constructs a Logical node of the Bleach AST (Abstract Syntax Tree). 
   *
   * This constructor initializes a Lofical object with the left operand, right operand and the operator that
   * represents the logical operation about to be performed on these operands.
   *
   * @param left: The left operand (represented by the std::shared_ptr<Expr> tyoe) of the logical 
   * operation/operator.
   * @param op: The operator that represents the logical operation about to be performed on the two operands 
   * (represented by the Token type).
   * @param right: The right operand (represented by the std::shared_ptr<Expr> type) of the logical 
   * operation/operator.
  **/
  Logical(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right)
    : left{std::move(left)}, op{std::move(op)}, right{std::move(right)} 
  {}

  std::any accept(ExprVisitor& visitor) override{
    return visitor.visitLogicalExpr(shared_from_this());
  }
};

/**
 * @struct Ternary
 * 
 * @brief Defines a struct to represent a ternary expression node from the AST of the Bleach language.
 *
 * The Ternary struct defines a struct to represent a ternary expression node from the AST (Abstract Syntax
 * Tree) of the Bleach language. A ternary expression is an expression that has three operands, as its name
 * suggests. Such struct has three attributes: one called 'condition' which will be evaluated to true or false,
 * and other two attributes called 'truthyResult' and 'falsyResult'. If the 'condition' attribute is evaluated
 * to true, then the attribute 'truthyResult' will be evaluated. Otherwise, the 'falsyResult' is the one that
 * will be evaluated.
 */
struct Ternary : Expr, public std::enable_shared_from_this<Ternary>{
  const std::shared_ptr<Expr> condition;
  const std::shared_ptr<Expr> ifBranch;
  const std::shared_ptr<Expr> elseBranch;

  /**
   * @brief Constructs a Ternary node of the Bleach AST (Abstract Syntax Tree). 
   *
   * This constructor initializes a Ternary object with the condition (that must be evaluated) of the ternary
   * and the pointers to the two possible results (expressions) of evaluating such condition (truthyResult and
   * falsyResult).
   *
   * @param condition: The condition that must be evaluated as true of false (represented by the std::shared_ptr<Expr>
   * type).
   * @param ifBranch: The resulting value that will be produced in case the condition is evaluated to true
   * (represented by the std::shared_ptr<Expr> type).
   * @param elseBranch: The resulting value that will be produced in case the condition is evaluated to false
   * (represented by the std::shared_ptr<Expr> type).
  **/
  Ternary(std::shared_ptr<Expr> condition, std::shared_ptr<Expr> ifBranch, std::shared_ptr<Expr> elseBranch)
    : condition{std::move(condition)}, ifBranch{std::move(ifBranch)}, elseBranch{std::move(elseBranch)}
  {}

  std::any accept(ExprVisitor& visitor) override{
    return visitor.visitTernaryExpr(shared_from_this());
  }
};

/**
 * @struct Unary
 * 
 * @brief Defines a struct to represent an unary expression node from the AST of the Bleach language.
 *
 * The Unary struct defines a struct to represent an unary expression node from the AST (Abstract Syntax Tree)
 * of the Bleach language. An unary expression is an expression that has only one operand, as its name suggests.
 * Such struct has only two attributes: one called 'op' that represents the operator of the unary expression and
 * another called 'right' that represents the operand on which the operator will be applied on.
 */
struct Unary : Expr, public std::enable_shared_from_this<Unary>{
  const Token op;
  const std::shared_ptr<Expr> right;

  /**
   * @brief Constructs an Unary node of the Bleach AST (Abstract Syntax Tree). 
   *
   * This constructor initializes an Unary object with the right operand and the operator that represents the 
   * operation about to be performed on this operand.
   *
   * @param op: The operator that represents the operation about to be performed on the right operands
   * (represented by the Token type).
   * @param right: The right operand (represented by the std::shared_ptr<Expr> type) of the unary 
   * operation/operator.
  **/
  Unary(Token op, std::shared_ptr<Expr> right)
    : op{std::move(op)}, right{std::move(right)}
  {}

  std::any accept(ExprVisitor& visitor) override{
    return visitor.visitUnaryExpr(shared_from_this());
  }
};

/**
 * @struct Variable
 * 
 * @brief Defines a struct to represent a variable access expression node from the AST of the Bleach language.
 *
 * The Variable struct defines a struct to represent a variable access expression node from the AST (Abstract 
 * Syntax Tree) of the Bleach language. A variable access expression is an expression that has only one 
 * attribute: A token that refers to the name of a variable.
 * 
 * @note The act of accessing a variable is considered an expression because it produces a value: the value that
 * is bound to the variable whose token "name" is referencing.
 */
struct Variable : Expr, public std::enable_shared_from_this<Variable>{
  const Token name;

  /**
   * @brief Constructs a Variable node of the Bleach AST (Abstract Syntax Tree). 
   *
   * This constructor initializes a Variable object with the attribute that was mentioned above.
   *
   * @param name: The token whose lexeme stores the name of a variable (represented by the Token type).
  **/
  Variable(Token name)
    : name{std::move(name)}
  {}

  std::any accept(ExprVisitor& visitor) override{
    return visitor.visitVariableExpr(shared_from_this());
  }
};
