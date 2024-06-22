#pragma once

#include <any>
#include <memory>
#include <utility>
#include <vector>

#include "./Expr.hpp"


// Necessary forward declarations of certain structs so they can be used inside the 'StmtVisitor' struct below.
struct Block;
struct DoWhile;
struct Expression; // Expression statement (Used in function and method calls).
struct If;
struct Print;
struct Var; // Variable declaration statement.
struct While;

/**
 * @struct StmtVisitor
 * 
 * @brief Defines an interface for visiting different types of statement nodes from the Bleach AST (Abstract 
 * Syntax Tree).
 *
 * The StmtVisitor struct defines an abstract struct that is responsible for working as an interface whose
 * functions are responsible for visiting the different statement nodes of the Bleach language AST (Abstract
 * Syntax Tree). Such struct has a set of pure virtual functions that represent different operations on the 
 * mentioned statement nodes. This same struct also has a virtual destructor that ensures the destructor of
 * a derived class/struct is called correctly when an object is deleted through a pointer to the base 
 * class/struct. In practice, each of the statement nodes are going to be visited by an instance of a 
 * struct/class that derives from 'StmtVisitor'.
 */
struct StmtVisitor{
  virtual std::any visitBlockStmt(std::shared_ptr<Block> stmt) = 0;
  virtual std::any visitDoWhileStmt(std::shared_ptr<DoWhile> stmt) = 0;
  virtual std::any visitExpressionStmt(std::shared_ptr<Expression> stmt) = 0;
  virtual std::any visitIfStmt(std::shared_ptr<If> stmt) = 0;
  virtual std::any visitPrintStmt(std::shared_ptr<Print> stmt) = 0;
  virtual std::any visitVarStmt(std::shared_ptr<Var> stmt) = 0;
  virtual std::any visitWhileStmt(std::shared_ptr<While> stmt) = 0;
  virtual ~StmtVisitor() = default;
};

/**
 * @struct Stmt
 * 
 * @brief Defines an abstract struct which will be used as a base struct to implement derived structs that 
 * represent different kinds of statement nodes in the AST (Abstract Syntax Tree) of the Bleach language.
 *
 * The Stmt struct defines an abstract struct that is responsible for working as the base struct from which all
 * structs that represent different types of statement AST nodes will derive from. This struct has only a pure
 * virtual method called 'accept'. This method will be overridden by the derived structs where each kind of
 * struct will have its own implementation for such method.
 */
struct Stmt{
  virtual std::any accept(StmtVisitor& visitor) = 0;
};

/**
 * @struct Block
 * 
 * @brief Defines a struct to represent a block statement node from the AST of the Bleach language.
 *
 * The Block struct defines a struct to represent a block statement node from the AST (Abstract Syntax Tree) of
 * the Bleach language. A block statement is a statement that essentialy creates a new scope (the block scope)
 * and stores a (possibly empty) sequence of statements inside itself. 
 * Therefore, this struct has only one attribute, called "statements", a std::vector<std::shared_ptr<Stmt>> that
 * represents the sequence of statements the block contains.
 */
struct Block : Stmt, public std::enable_shared_from_this<Block>{
  const std::vector<std::shared_ptr<Stmt>> statements;

  /**
   * @brief Constructs a Block node of the Bleach AST (Abstract Syntax Tree). 
   *
   * This constructor initializes a Block object with the list of statements that it will contain.
   *
   * @param statements: The list of statements that this block statements has. Such list is possibly empty 
   * (represented by the std::vector<std::shared_ptr<Stmt>> type).
  **/
  Block(std::vector<std::shared_ptr<Stmt>> statements)
    : statements{std::move(statements)}
  {}

  std::any accept(StmtVisitor& visitor) override{
    return visitor.visitBlockStmt(shared_from_this());
  }
};

struct DoWhile : Stmt, public std::enable_shared_from_this<DoWhile>{
  const std::shared_ptr<Expr> condition;
  const std::shared_ptr<Stmt> body;

  DoWhile(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> body)
    : condition{std::move(condition)}, body{std::move(body)}
  {}

  std::any accept(StmtVisitor& visitor) override{
    return visitor.visitDoWhileStmt(shared_from_this());
  }
};

/**
 * @struct Expression
 * 
 * @brief Defines a struct to represent an expression statement node from the AST of the Bleach language.
 *
 * The Expression struct defines a struct to represent an expression statement node from the AST (Abstract 
 * Syntax Tree) of the Bleach language. An expression statement is a statement that has only one attribute.
 * Such attribute, called 'expression' represents the expression that will result in a side-effect to the state
 * of the interpreter.
 * Going more deep, an expression statement lets the user place an expression where a statement is expected.
 * They are very common in popular languages like C and Java. For example, any time you see a function of method
 * call followed by a ';', you are looking at an expression statement.
 */
struct Expression : Stmt, public std::enable_shared_from_this<Expression>{
  const std::shared_ptr<Expr> expression;

  /**
   * @brief Constructs a Expression node of the Bleach AST (Abstract Syntax Tree). 
   *
   * This constructor initializes an Expression object with the expression that is "wrapped" inside the
   * statement.
   *
   * @param expression: The expression that is wrapped inside the expression statement (represented by the 
   * std::shared_ptr<Expr> type).
  **/
  Expression(std::shared_ptr<Expr> expression)
    : expression{std::move(expression)}
  {}

  std::any accept(StmtVisitor& visitor) override{
    return visitor.visitExpressionStmt(shared_from_this());
  }
};

/**
 * @struct If
 * 
 * @brief Defines a struct to represent an if statement node from the AST of the Bleach language.
 *
 * The If struct defines a struct to represent an if statement node from the AST (Abstract Syntax Tree) of
 * the Bleach language. An if statement is a statement that must have an expression that works as its if condition.
 * This means that, if such expression is truthy, then the statement associated to such condition (here called
 * "ifBranch" will be executed). The if statement also has a (possibly empty) list of elif conditions, which
 * are going to be evaluated in the order they appear in the code (from top to bottom) if any of them is truthy,
 * then its associated statement will be executed. Finally, the if statement can also have an else condition.
 * If such condition is truthy, then its associated statement will be executed.
 * 
 * @note: It's important to remember that, the verification of whether the conditions of an if statement are
 * truthy or falsey is made from top to bottom. Which means that the condition associated to the "if" keyword
 * is the first one to be evaluated. Then, the conditions associated to each "elif" keyword are evaluated, one
 * by one, from top to bottom. Finally, the condition associated to the "else" keyword is evaluated. The first
 * condition that evaluates to true will have its associated statement executed. After one of the statements is
 * executed, the flow of the code "gets out" from the if statement.
 */
struct If : Stmt, public std::enable_shared_from_this<If>{
  const std::shared_ptr<Expr> ifCondition;
  const std::shared_ptr<Stmt> ifBranch;
  const std::vector<std::shared_ptr<Expr>> elifConditions;
  const std::vector<std::shared_ptr<Stmt>> elifBranches;
  const std::shared_ptr<Stmt> elseBranch;

  /**
   * @brief Constructs an If node of the Bleach AST (Abstract Syntax Tree). 
   *
   * This constructor initializes an If object with the associated attributes that we've mentioned above.
   *
   * @param ifCondition: The expression that is associated to the "if" keyword present in an if statement. 
   * (represented by the std::shared_ptr<Expr> type).
   * @param ifBranch: The statement that is associated to the "if" keyword from an if statement. This is the
   * statement that will be executed in case the expression associated to the "if" keyword is truthy. (represented
   * by the std::shared_ptr<Stmt> type).
   * @param elifConditions: The (possibly empty) list of expressions that are associated to each "elif" keyword 
   * that are present inside an if statement. These conditions are going to be stored inside the list in the
   * same order they appear in the code (represented by the std::vector<std::shared_ptr<Expr>> type).
   * @param elifBranches: The (possibly empty) list of statements that are associate to each "elif" keyword that
   * are present inside an if statement. These statements are going to be stored inside the list in the same 
   * order they appear in the code (represented by the std::vector<std::shared_ptr<Stmt>> type).
   * @param elseBranch: The (possibly non-existent) statement that is associated to the "else" keyword present
   * in an if statement (represented by the std::shared_ptr<Stmt> type).
  **/
  If(std::shared_ptr<Expr> ifCondition, std::shared_ptr<Stmt> ifBranch, std::vector<std::shared_ptr<Expr>> elifConditions, std::vector<std::shared_ptr<Stmt>> elifBranches, std::shared_ptr<Stmt> elseBranch)
    : ifCondition{std::move(ifCondition)}, ifBranch{std::move(ifBranch)}, elifConditions{std::move(elifConditions)}, elifBranches{std::move(elifBranches)}, elseBranch{std::move(elseBranch)}
  {}


  std::any accept(StmtVisitor& visitor) override{
    return visitor.visitIfStmt(shared_from_this());
  }
};

/**
 * @struct Print
 * 
 * @brief Defines a struct to represent a print statement node from the AST of the Bleach language.
 *
 * The Print struct defines a struct to represent a print statement node from the AST (Abstract Syntax Tree)
 * of the Bleach language. A print statement is a statement that has only one attribute an expression, whose
 * value will be produced an then displayed to the user through the console/terminal.
 */
struct Print : Stmt, public std::enable_shared_from_this<Print>{
  const std::shared_ptr<Expr> expression;

  /**
   * @brief Constructs a Print node of the Bleach AST (Abstract Syntax Tree). 
   *
   * This constructor initializes a Print object with the expression that is "wrapped" inside the statement.
   *
   * @param expression: The expression that will be evaluated and displayed inside the console/terminal 
   * (represented by the std::shared_ptr<Expr> type).
  **/
  Print(std::shared_ptr<Expr> expression)
    : expression{std::move(expression)}
  {}

  std::any accept(StmtVisitor& visitor) override{
    return visitor.visitPrintStmt(shared_from_this());
  }
};

/**
 * @struct Var
 * 
 * @brief Defines a struct to represent a variable declaration statement node from the AST of the Bleach 
 * language.
 *
 * The Var struct defines a struct to represent a variable declaration statement node from the AST (Abstract 
 * Syntax Tree) of the Bleach language. A variable declaration statement is a statement that has two attributes. 
 * The first attribute is "name", a Token that represents a variable name. The second attribute is "initializer",
 * a std::shared_ptr<Expr> that represents the expression whose value will be used as the initial value of the 
 * variable. If none is provided, then the default initial value of the variable will be nil.
 */
struct Var : Stmt, public std::enable_shared_from_this<Var>{
  const Token name;
  const std::shared_ptr<Expr> initializer;

  /**
   * @brief Constructs a Var node of the Bleach AST (Abstract Syntax Tree). 
   *
   * This constructor initializes a Var object with the two attributes that were mentioned above.
   *
   * @param name: The token that holds the name of the varible being declared (represented by the Token type).
   * @param initializer: The expression that will be evaluated and whose resulting value will be used as the
   * initial value of the variable being declared (represented by the std::shared_ptr<Expr> type). If nullptr is
   * provided as its value, then the variable will have nil as its initial value.
  **/
  Var(Token name, std::shared_ptr<Expr> initializer)
    : name{std::move(name)}, initializer{std::move(initializer)}
  {}

  std::any accept(StmtVisitor& visitor) override{
    return visitor.visitVarStmt(shared_from_this());
  }
};

/**
 * @struct While
 * 
 * @brief Defines a struct to represent a while statement node from the AST of the Bleach language.
 *
 * The While struct defines a struct to represent a while statement node from the AST (Abstract Syntax Tree) of
 * the Bleach language. A while statement is a statement that during runtime works as a looping control flow
 * mechanism. This struct has two attributes: The first one is called "condition". It is an expression that
 * determines for how long the statement inside the while body will be executed. They are going to be executed
 * as long as the "condition" expression evaluates to true. The second one is called "body". Is is a statement
 * that will be executed while the "condition" expression evaluates to true.
 */
struct While : Stmt, public std::enable_shared_from_this<While>{
  const std::shared_ptr<Expr> condition;
  const std::shared_ptr<Stmt> body;

  /**
   * @brief Constructs a While node of the Bleach AST (Abstract Syntax Tree). 
   *
   * This constructor initializes a While object with the two attributes that were mentioned above.
   *
   * @param condition: The expression that will be evaluated at each iteration of the while loop (including the
   * first one). The value produced by the evaluation of this expression is what determines whether or not the
   * statement inside the while loop is going to be executed. If it's true, then yes. Otherwise, no (represented
   * by the std::shared_ptr<Expr> type).
   * @param body: The statement that will be executed or not depending on the value produced by the evaluation
   * of the expression stored inside the "condition" attribute (represented by the std::shared_ptr<Stmt> type). 
  **/
  While(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> body)
    : condition{std::move(condition)}, body{std::move(body)}
  {}

  std::any accept(StmtVisitor& visitor) override{
    return visitor.visitWhileStmt(shared_from_this());
  }
};
