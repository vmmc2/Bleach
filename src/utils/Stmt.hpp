#pragma once

#include <any>
#include <memory>
#include <utility>
#include <vector>

#include "./Expr.hpp"


// Necessary forward declarations of certain structs so they can be used inside the 'StmtVisitor' struct below.
struct Block;
struct Break;
struct Class; // Class declaration statement.
struct Continue;
struct DoWhile;
struct Expression; // Expression statement (Used in function and method calls).
struct For; // For loop statement.
struct Function; // Function statement (Used in function declarations).
struct If;
struct Print;
struct Return;
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
 * struct/class that derives from the StmtVisitor struct.
 */
struct StmtVisitor{
  virtual std::any visitBlockStmt(std::shared_ptr<Block> stmt) = 0;
  virtual std::any visitBreakStmt(std::shared_ptr<Break> stmt) = 0;
  virtual std::any visitClassStmt(std::shared_ptr<Class> stmt) = 0;
  virtual std::any visitContinueStmt(std::shared_ptr<Continue> stmt) = 0;
  virtual std::any visitDoWhileStmt(std::shared_ptr<DoWhile> stmt) = 0;
  virtual std::any visitExpressionStmt(std::shared_ptr<Expression> stmt) = 0;
  virtual std::any visitForStmt(std::shared_ptr<For> stmt) = 0;
  virtual std::any visitFunctionStmt(std::shared_ptr<Function> stmt) = 0;
  virtual std::any visitIfStmt(std::shared_ptr<If> stmt) = 0;
  virtual std::any visitPrintStmt(std::shared_ptr<Print> stmt) = 0;
  virtual std::any visitReturnStmt(std::shared_ptr<Return> stmt) = 0;
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
 * virtual method called "accept". This method will be overridden by the derived structs where each kind of
 * struct will have its own implementation for such method.
 */
struct Stmt{
  virtual std::any accept(StmtVisitor& visitor) = 0;
  virtual std::string toString() = 0;
};

/**
 * @struct Block
 * 
 * @brief Defines a struct to represent a block statement node from the AST of the Bleach language.
 *
 * The Block struct defines a struct to represent a block statement node from the AST (Abstract Syntax Tree) of
 * the Bleach language. A block statement is a statement that essentialy creates a new scope (the block scope)
 * and stores a (possibly empty) sequence of statements inside itself. 
 * Therefore, this struct has only one attribute, called "statements". This attribute is a list of statements
 * that represents the sequence of statements the block contains.
 */
struct Block : Stmt, public std::enable_shared_from_this<Block>{
  const std::vector<std::shared_ptr<Stmt>> statements;

  /**
   * @brief Constructs a Block node of the Bleach AST (Abstract Syntax Tree). 
   *
   * This constructor initializes a Block object with the list of statements that it will contain.
   *
   * @param statements: The list of statements that this block statements has. Such list is possibly empty.
  **/
  Block(std::vector<std::shared_ptr<Stmt>> statements)
    : statements{std::move(statements)}
  {}

  std::any accept(StmtVisitor& visitor) override{
    return visitor.visitBlockStmt(shared_from_this());
  }

  std::string toString() override{
    return "block statement";
  }
};

/**
 * @struct Break
 * 
 * @brief Defines a struct to represent a break statement node from the AST of the Bleach language.
 *
 * The Break struct defines a struct to represent a break statement node from the AST (Abstract Syntax Tree) of
 * the Bleach language. A break statement is a statement that is used to interrupt the execution of the nearest
 * enclosing loop (whether it is a "for", "do-while" of "while" loop).
 * This struct has just one attribute: "keyword". It is a token that represents the "break" keyword.
 */
struct Break : Stmt, public std::enable_shared_from_this<Break>{
  const Token keyword;

  /**
   * @brief Constructs a Break node of the Bleach AST (Abstract Syntax Tree). 
   *
   * This constructor initializes a Break object with the attribute that was mentioned above.
   *
   * @param keyword: The token that represents the "break" keyword.
  **/
  Break(Token keyword)
    : keyword{std::move(keyword)}
  {}

  std::any accept(StmtVisitor& visitor) override{
    return visitor.visitBreakStmt(shared_from_this());
  }

  std::string toString() override{
    return "break statement";
  }
};

/**
 * @struct Class
 * 
 * @brief Defines a struct to represent a class declaration statement node from the AST of the Bleach language.
 *
 * The Class struct defines a struct to represent a class declaration statement node from the AST (Abstract 
 * Syntax Tree) of the Bleach language. A class declaration statement is a statement that is used to declare a
 * new user created class.
 * This struct has three attributes: The first one is "name". It's a token that represents the name of the 
 * declared class. The second one is "superclass". It's a variable expression that is responsible for storing 
 * the name of the superclass (if any) from which the class that has just been declared inherits from. The third
 * one is "methods". It's just a list of function declaration statements that represents the methods that were
 * declared inside this class.
 */
struct Class : Stmt, public std::enable_shared_from_this<Class>{
  const Token name;
  const std::shared_ptr<Variable> superclass;
  const std::vector<std::shared_ptr<Function>> methods;

  /**
   * @brief Constructs a Class node of the Bleach AST (Abstract Syntax Tree). 
   *
   * This constructor initializes a Class object with the attributes that was mentioned above.
   *
   * @param name: The token that represents the name of the declared class.
   * @param superclass: The expression that stores a token which represents the name of the superclass that the
   * declared class inherits from.
   * @param methods: The list of methods that the declared class has declared inside itself.
  **/
  Class(Token name, std::shared_ptr<Variable> superclass, std::vector<std::shared_ptr<Function>> methods)
    : name{std::move(name)}, superclass{std::move(superclass)}, methods{std::move(methods)}
  {}

  std::any accept(StmtVisitor& visitor) override{
    return visitor.visitClassStmt(shared_from_this());
  }

  std::string toString() override{
    return "class declaration statement";
  }
};

/**
 * @struct Continue
 * 
 * @brief Defines a struct to represent a continue statement node from the AST of the Bleach language.
 *
 * The Continue struct defines a struct to represent a continue statement node from the AST (Abstract Syntax 
 * Tree) of the Bleach language. A continue statement is a statement that is used to make the the nearest 
 * enclosing loop (whether it is a "for", "do-while" of "while" loop) immediately go to its next iteration.
 * This struct has just one attribute: "keyword". It is a token that represents the "continue" keyword.
 */
struct Continue : Stmt, public std::enable_shared_from_this<Continue>{
  const Token keyword;

  /**
   * @brief Constructs a Continue node of the Bleach AST (Abstract Syntax Tree). 
   *
   * This constructor initializes a Continue object with the attribute that was mentioned above.
   *
   * @param keyword: The token that represents the "continue" keyword.
  **/
  Continue(Token keyword)
    : keyword{std::move(keyword)}
  {}

  std::any accept(StmtVisitor& visitor) override{
    return visitor.visitContinueStmt(shared_from_this());
  }

  std::string toString() override{
    return "continue statement";
  }
};

/**
 * @struct DoWhile
 * 
 * @brief Defines a struct to represent a do-while statement node from the AST of the Bleach language.
 *
 * The DoWhile struct defines a struct to represent a do-while statement node from the AST (Abstract Syntax 
 * Tree) of the Bleach language. A do-while statement is a statement that during runtime works as a looping 
 * control flow mechanism. This struct has two attributes: The first one is called "condition". It is an 
 * expression whose evaluation will determine for how long the statements inside the do-while body will be 
 * executed. The second one is called "body". It is a list of statements that will be executed while the 
 * "condition" expression evaluates to true.
 * 
 * @note: Remember that the statements present inside a do-while statement are always executed in its very 
 * first iteration because the evaluation of the expression present inside "condition" is made at the end of 
 * each iteration.
 */
struct DoWhile : Stmt, public std::enable_shared_from_this<DoWhile>{
  const std::shared_ptr<Expr> condition;
  const std::vector<std::shared_ptr<Stmt>> body;

  /**
   * @brief Constructs a DoWhile node of the Bleach AST (Abstract Syntax Tree). 
   *
   * This constructor initializes a DoWhile object with the two attributes that were mentioned above.
   *
   * @param condition: The expression that will be evaluated at the end of each iteration of the do-while loop
   * (including the first one). The value produced by the evaluation of this expression is what determines 
   * whether or not the statements inside the do-while loop are going to be executed. If it's true, then yes. 
   * Otherwise, no.
   * @param body: The list of statements that will be executed or not depending on the value produced by the 
   * evaluation of the expression stored inside the "condition" attribute. Remember that the statements stored
   * inside "body" will always be executed in the first iteration of this type of loop because "condition" is 
   * always evaluated at the end of each iteration. 
  **/
  DoWhile(std::shared_ptr<Expr> condition, std::vector<std::shared_ptr<Stmt>> body)
    : condition{std::move(condition)}, body{std::move(body)}
  {}

  std::any accept(StmtVisitor& visitor) override{
    return visitor.visitDoWhileStmt(shared_from_this());
  }

  std::string toString() override{
    return "do-while statement";
  }
};

/**
 * @struct Expression
 * 
 * @brief Defines a struct to represent an expression statement node from the AST of the Bleach language.
 *
 * The Expression struct defines a struct to represent an expression statement node from the AST (Abstract 
 * Syntax Tree) of the Bleach language. This struct has only one attribute: such attribute, called "expression",
 * represents an expression that will result in a side-effect to the state of the interpreter.
 * Going more deep, an expression statement lets the user place an expression where a statement is expected.
 * These are very common in popular languages like C and Java. For example, any time you see a function or 
 * method call followed by a ";", you are looking at an expression statement.
 */
struct Expression : Stmt, public std::enable_shared_from_this<Expression>{
  const std::shared_ptr<Expr> expression;

  /**
   * @brief Constructs a Expression node of the Bleach AST (Abstract Syntax Tree). 
   *
   * This constructor initializes an Expression object with an expression that is "wrapped" inside the
   * statement.
   *
   * @param expression: The expression that is wrapped inside the expression statement.
  **/
  Expression(std::shared_ptr<Expr> expression)
    : expression{std::move(expression)}
  {}

  std::any accept(StmtVisitor& visitor) override{
    return visitor.visitExpressionStmt(shared_from_this());
  }

  std::string toString() override{
    return "expression statement";
  }
};

/**
 * @struct For
 * 
 * @brief Defines a struct to represent a for statement node from the AST of the Bleach language.
 *
 * The For struct defines a struct to represent a for statement node from the AST (Abstract Syntax 
 * Tree) of the Bleach language. A for statement is a statement that during runtime works as a looping 
 * control flow mechanism. This struct has four attributes: The first one is called "initializer". It's a 
 * statement executed only once before the runtime enters the for loop. Such statement is usually used to 
 * create a new variable or execute an expression statement. The second one is called "condition". It is an 
 * expression whose evaluated value determines for how long the statements inside the while body will be 
 * executed. Moreover, remember that, in the case of a for loop, such expression is also evaluated at the 
 * beginning of the for loop (before even executing the statements inside the for loop). The third one is called
 * "increment". It's an expression that will be evaluated at the end of each iteration of a for loop. The fourth
 * one is called "body". It's the list of statements that will be executed while the "condition" expression
 * evaluates to true.
 */
struct For : Stmt, public std::enable_shared_from_this<For>{
  const std::shared_ptr<Stmt> initializer;
  const std::shared_ptr<Expr> condition;
  const std::shared_ptr<Expr> increment;
  const std::vector<std::shared_ptr<Stmt>> body;

  /**
   * @brief Constructs a For node of the Bleach AST (Abstract Syntax Tree). 
   *
   * This constructor initializes a For object with the four attributes that were mentioned above.
   *
   * @param initializer: The statement that will be executed only once and before the interpreter indeed starts 
   * the execution of the for loop.
   * @param condition: The expression that will be evaluated at the beginning of each iteration of the for loop.
   * The value produced by the evaluation of this expression is what determines whether or not the statements 
   * inside the for loop are going to be executed. If it's true, then yes. Otherwise, no.
   * @param increment: The expression that will be evaluated at the end of each iteration of the for loop.
   * @param body: The list of statements that will be executed or not depending on the value produced by the 
   * evaluation of the expression stored inside the "condition" attribute.
  **/
  For(std::shared_ptr<Stmt> initializer, std::shared_ptr<Expr> condition, std::shared_ptr<Expr> increment, std::vector<std::shared_ptr<Stmt>> body)
    : initializer{std::move(initializer)}, condition{std::move(condition)}, increment{std::move(increment)}, body{std::move(body)}
  {}

  std::any accept(StmtVisitor& visitor) override{
    return visitor.visitForStmt(shared_from_this());
  }

  std::string toString() override{
    return "for statement";
  }
};

/**
 * @struct Function
 * 
 * @brief Defines a struct to represent a function declaration statement node from the AST of the Bleach 
 * language.
 *
 * The Function struct defines a struct to represent a function declaration statement node from the AST 
 * (Abstract Syntax Tree) of the Bleach language. A function declaration statement is a statement that 
 * represents the declaration of a function (its name, its parameters' names and its body). This struct has
 * three attributes: The first one is called "name". It's a token that represents the name of the declared 
 * function. The second one is called "parameters". It is a list of tokens where each token represents the name
 * of a parameter from the declared function. The third one is called "body". It's a list of statements that 
 * will be executed when the declared function is called during runtime.
 */
struct Function : Stmt, public std::enable_shared_from_this<Function>{
  const Token name; // The name of the function. It's has a TokenType::IDENTIFIER as its type attribute.
  const std::vector<Token> parameters; // As above, the parameters are all tokens that have TokenType::IDENTIFIER as their type attribute.
  const std::vector<std::shared_ptr<Stmt>> body; // The list of statements that make the body of the function.

  /**
   * @brief Constructs a Function node of the Bleach AST (Abstract Syntax Tree). 
   *
   * This constructor initializes a Function object with the three attributes that were mentioned above.
   *
   * @param name: The token whose lexeme represents the name of the declared function.
   * @param parameters: The list of tokens whose lexemes represents the names of parameters from the declared
   * function.
   * @param body: The list of statements that will be executed once the function is called during runtime.
  **/
  Function(Token name, std::vector<Token> parameters, std::vector<std::shared_ptr<Stmt>> body)
    : name{std::move(name)}, parameters{std::move(parameters)}, body{std::move(body)}
  {}

  std::any accept(StmtVisitor& visitor) override{
    return visitor.visitFunctionStmt(shared_from_this());
  }

  std::string toString() override{
    return "function statement";
  }
};

/**
 * @struct If
 * 
 * @brief Defines a struct to represent an if statement node from the AST of the Bleach language.
 *
 * The If struct defines a struct to represent an if statement node from the AST (Abstract Syntax Tree) of
 * the Bleach language. An if statement is a statement that must have an expression that works as its if 
 * condition. This means that, if such expression is truthy, then the statements associated to such condition
 * (here called "ifBranch" will be executed). The if statement also has a (possibly empty) list of elif
 * conditions, which are going to be evaluated in the order they appear in the code (from top to bottom) if any
 * of them is truthy, then its associated statements will be executed. Finally, the if statement can also have
 * an else condition. If such condition is truthy, then its associated statements will be executed.
 * 
 * @note: It's important to remember that, the verification of whether the conditions of an if statement are
 * truthy or falsey is made from top to bottom. Which means that the condition associated to the "if" keyword
 * is the first one to be evaluated. Then, the conditions associated to each "elif" keyword are evaluated, one
 * by one, from top to bottom. Finally, the condition associated to the "else" keyword is evaluated. The first
 * condition that evaluates to true will have its associated statements executed. After one of such list of
 * statements is executed, the flow of the code "gets out" from the if statement.
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
   * @param ifBranch: The statement that is associated to the "if" keyword from an if statement. This is the
   * statement that will be executed in case the expression associated to the "if" keyword evaluates to a truthy
   * value.
   * @param elifConditions: The (possibly empty) list of expressions that are associated to each "elif" keyword 
   * that are present inside an if statement. These conditions are going to be stored inside the list in the
   * same order they appear in the code. Moreover, they will also be evaluated during runtime in this same
   * order.
   * @param elifBranches: The (possibly empty) list of statements that are associate to each "elif" keyword that
   * are present inside an if statement. These statements are going to be stored inside the list in the same 
   * order they appear in the code.
   * @param elseBranch: The (possibly non-existent) statements that are associated to the "else" keyword present
   * in an if statement.
  **/
  If(std::shared_ptr<Expr> ifCondition, std::shared_ptr<Stmt> ifBranch, std::vector<std::shared_ptr<Expr>> elifConditions, std::vector<std::shared_ptr<Stmt>> elifBranches, std::shared_ptr<Stmt> elseBranch)
    : ifCondition{std::move(ifCondition)}, ifBranch{std::move(ifBranch)}, elifConditions{std::move(elifConditions)}, elifBranches{std::move(elifBranches)}, elseBranch{std::move(elseBranch)}
  {}

  std::any accept(StmtVisitor& visitor) override{
    return visitor.visitIfStmt(shared_from_this());
  }

  std::string toString() override{
    return "if statement";
  }
};

/**
 * @struct Print
 * 
 * @brief Defines a struct to represent a print statement node from the AST of the Bleach language.
 *
 * The Print struct defines a struct to represent a print statement node from the AST (Abstract Syntax Tree)
 * of the Bleach language. A print statement is a statement that has only one attribute. Such attribute is called
 * "expression". It is, as its name suggests, an expression, whose value will be produced during runtime and 
 * then displayed to the user through the console/terminal.
 */
struct Print : Stmt, public std::enable_shared_from_this<Print>{
  const std::shared_ptr<Expr> expression;

  /**
   * @brief Constructs a Print node of the Bleach AST (Abstract Syntax Tree). 
   *
   * This constructor initializes a Print object with the expression that is "wrapped" inside the statement.
   *
   * @param expression: The expression that will be evaluated and displayed inside the console/terminal during
   * runtime.
  **/
  Print(std::shared_ptr<Expr> expression)
    : expression{std::move(expression)}
  {}

  std::any accept(StmtVisitor& visitor) override{
    return visitor.visitPrintStmt(shared_from_this());
  }

  std::string toString() override{
    return "print statement";
  }
};

/**
 * @struct Return
 * 
 * @brief Defines a struct to represent a return statement node from the AST of the Bleach language.
 *
 * The Return struct defines a struct to represent a return statement node from the AST (Abstract Syntax 
 * Tree) of the Bleach language. A return statement is a statement that is used inside functions, lambdas and
 * methods to signal the end of the body of a function, a lambda or a method. It can possibly, but not 
 * necessarily, be followed by an expression that indicates the value returned by the function, lambda or 
 * method.
 * This struct has two attributes: The first one is called "keyword". It is a token that represents the "return"
 * keyword. The second one is called "value". It is an expression, that can possibly not exist, whose purpose is
 * to be evaluated during runtime and return the produced value as its result.
 */
struct Return : Stmt, public std::enable_shared_from_this<Return>{
  const Token keyword;
  const std::shared_ptr<Expr> value;

  /**
   * @brief Constructs a Return node of the Bleach AST (Abstract Syntax Tree). 
   *
   * This constructor initializes a Return object with the two attributes that were mentioned above.
   *
   * @param keyword: The token that represents the "return" keyword.
   * @param value: The expression that will be evaluated and whose resulting value will be used as the
   * return value of a function, a lambda or a method. 
   * If nullptr is provided as its value, then the return statement will return nil as its default value.
  **/
  Return(Token keyword, std::shared_ptr<Expr> value)
    : keyword{std::move(keyword)}, value{std::move(value)}
  {}

  std::any accept(StmtVisitor& visitor) override{
    return visitor.visitReturnStmt(shared_from_this());
  }

  std::string toString() override{
    return "return statement";
  }
};

/**
 * @struct Var
 * 
 * @brief Defines a struct to represent a variable declaration statement node from the AST of the Bleach 
 * language.
 *
 * The Var struct defines a struct to represent a variable declaration statement node from the AST (Abstract 
 * Syntax Tree) of the Bleach language. Such struct has two attributes. The first one is called "name". It is a 
 * token whose lexeme represents a variable name. The second one is "initializer". It is an expression whose 
 * value will be used as the initial value of the variable. If no expression is provided to the "initializer"
 * attribute (i.e., the attribute "initializer" has a nullptr as its value), then the default initial value of
 * the variable will be nil.
 */
struct Var : Stmt, public std::enable_shared_from_this<Var>{
  const Token name;
  const std::shared_ptr<Expr> initializer;

  /**
   * @brief Constructs a Var node of the Bleach AST (Abstract Syntax Tree). 
   *
   * This constructor initializes a Var object with the two attributes that were mentioned above.
   *
   * @param name: The token whose lexeme represents the name of the varible being declared.
   * @param initializer: The expression whose evaluated value will be used as the initial value of the variable
   * being declared. If a nullptr is provided as its value, then the variable will have nil as its initial 
   * value.
  **/
  Var(Token name, std::shared_ptr<Expr> initializer)
    : name{std::move(name)}, initializer{std::move(initializer)}
  {}

  std::any accept(StmtVisitor& visitor) override{
    return visitor.visitVarStmt(shared_from_this());
  }

  std::string toString() override{
    return "variable declaration statement";
  }
};

/**
 * @struct While
 * 
 * @brief Defines a struct to represent a while statement node from the AST of the Bleach language.
 *
 * The While struct defines a struct to represent a while statement node from the AST (Abstract Syntax Tree) of
 * the Bleach language. A while statement is a statement that during runtime works as a looping control flow
 * mechanism. This struct has two attributes: The first one is called "condition". It is an expression whose
 * value evaluated during runtime determines for how long the statements inside the while body will be executed.
 * The second one is called "body". It is a list of statements that will be executed while the "condition"
 * expression evaluates to true during runtime.
 */
struct While : Stmt, public std::enable_shared_from_this<While>{
  const std::shared_ptr<Expr> condition;
  const std::vector<std::shared_ptr<Stmt>> body;

  /**
   * @brief Constructs a While node of the Bleach AST (Abstract Syntax Tree). 
   *
   * This constructor initializes a While object with the two attributes that were mentioned above.
   *
   * @param condition: The expression that will be evaluated at the beginning of each iteration of the while 
   * loop (including the first one). The value produced by the evaluation of this expression is what determines
   * whether or not the statements inside the while loop are going to be executed. If it's true, then yes. 
   * Otherwise, no.
   * @param body: The list of statements that will be executed or not depending on the value produced by the 
   * evaluation of the expression stored inside the "condition" attribute.
  **/
  While(std::shared_ptr<Expr> condition, std::vector<std::shared_ptr<Stmt>> body)
    : condition{std::move(condition)}, body{std::move(body)}
  {}

  std::any accept(StmtVisitor& visitor) override{
    return visitor.visitWhileStmt(shared_from_this());
  }

  std::string toString() override{
    return "while statement";
  }
};
