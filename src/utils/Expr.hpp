#pragma once

#include <any>
#include <memory>
#include <utility>
#include <vector>

#include "./Token.hpp"


// Necessary forward declarations of certain structs so they can be used inside the 'ExprVisitor' struct below.
struct Assign;
struct Binary;
struct Call;
struct Get;
struct Grouping;
struct Index;
struct LambdaFunction;
struct Literal;
struct Logical;
struct Self;
struct Set;
struct Super;
struct Ternary;
struct Unary;
struct Variable;

struct Stmt; // Forward declaration needed to avoid circular dependencies.

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
  virtual std::any visitCallExpr(std::shared_ptr<Call> expr) = 0;
  virtual std::any visitGetExpr(std::shared_ptr<Get> expr) = 0;
  virtual std::any visitGroupingExpr(std::shared_ptr<Grouping> expr) = 0;
  virtual std::any visitIndexExpr(std::shared_ptr<Index> expr) = 0;
  virtual std::any visitLambdaFunctionExpr(std::shared_ptr<LambdaFunction> expr) = 0;
  virtual std::any visitLiteralExpr(std::shared_ptr<Literal> expr) = 0;
  virtual std::any visitLogicalExpr(std::shared_ptr<Logical> expr) = 0;
  virtual std::any visitSelfExpr(std::shared_ptr<Self> expr) = 0;
  virtual std::any visitSetExpr(std::shared_ptr<Set> expr) = 0;
  virtual std::any visitSuperExpr(std::shared_ptr<Super> expr) = 0;
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
 * the AST (Abstract Syntax Tree) of the Bleach language. Such struct has two attributes: The first one is called
 * "name". It is a token that represents an identifier (a place, in-memory, which the assignment is going to be 
 * made to). The second one is called "value". It is an expression whose value is going to be assigned to such 
 * place in-memory.
 * When such expression is evaluated by the Interpreter class, it "evaluates" the left-hand side operand to
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
   * that be evaluated later in order to produce a value.
   * 
   * @param name: The left-hand side operand of the assignment operation. Also known as "l-value"
   * @param value: The right-hand side operand of the assignment operation. Also known as "r-value".
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
 * operands of a binary expression ("left" and "right") and another that represents the kind of operation that 
 * will be performed on these two operands ("op").
 */
struct Binary : Expr, public std::enable_shared_from_this<Binary>{
  const std::shared_ptr<Expr> left;
  const Token op;
  const std::shared_ptr<Expr> right;

  /**
   * @brief Constructs a Binary node of the Bleach AST (Abstract Syntax Tree). 
   *
   * This constructor initializes a Binary object with the three attributes mentioned above: left operand,
   * right operand and the operator that represents the binary operation about to be performed on these operands.
   *
   * @param left: The left operand of the binary operator.
   * @param op: The operator that represents the binary operation about to be performed on the two operands. 
   * @param right: The right operand of the binary operator.
  **/
  Binary(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right)
    : left{std::move(left)}, op{std::move(op)}, right{std::move(right)} 
  {}

  std::any accept(ExprVisitor& visitor) override{
    return visitor.visitBinaryExpr(shared_from_this());
  }
};

/**
 * @struct Call
 * 
 * @brief Defines a struct to represent a call expression node from the AST of the Bleach language.
 *
 * The Call struct defines a struct that is responsible for representing a call expression node from the
 * AST (Abstract Syntax Tree) of the Bleach language. Such struct has three attributes: The first one is called
 * "callee". It is an expression that, at runtime, needs to be evaluated into what it's called a callable entity
 * (a class or a function or a method or a lambda/anonymous function). The second one is called "paren". It is
 * just the token that represents the right parentheses (")") of a call expression. Such token is commonly used
 * to handle error report. The third one is "arguments". It is a list of expressions where each expression will
 * be evaluated into an argument that will be passed to the respective parameter of a callable entity at 
 * runtime.
 */
struct Call : Expr, public std::enable_shared_from_this<Call>{
  const std::shared_ptr<Expr> callee;
  const Token paren; // Token that represents the closing parentheses ')'. It is used to report a runtime error caused by a function call, if it happens.
  const std::vector<std::shared_ptr<Expr>> arguments;

  /**
   * @brief Constructs a Call node of the Bleach AST (Abstract Syntax Tree). 
   *
   * This constructor initializes a Call object with the three attributes that were mentioned above ("callee",
   * "paren" and "arguments").
   *
   * @param callee: The expression that must be evaluated into a callable entity during runtime (a class, a 
   * function, a method or a lambda/anonymous function).
   * @param paren: The token whose lexeme represents the closing (right) parentheses that appear in a call 
   * expression. 
   * @param arguments: The list of expression where each expression will be evaluated into a value during 
   * runtime so the call expression can also be properly evaluated during runtime.
  **/
  Call(std::shared_ptr<Expr> callee, Token paren, std::vector<std::shared_ptr<Expr>> arguments)
    : callee{std::move(callee)}, paren{std::move(paren)}, arguments{std::move(arguments)}
  {}

  std::any accept(ExprVisitor& visitor) override{
    return visitor.visitCallExpr(shared_from_this());
  }
};

/**
 * @struct Get
 * 
 * @brief Defines a struct to represent a get expression node from the AST of the Bleach language.
 *
 * The Get struct defines a struct to represent a get expression node from the AST (Abstract Syntax Tree) of the
 * Bleach language. A get expression is an expression that essentialy retrieves the value of a specific property
 * from an instance of a class. This struct has only two attributes: The first one is called "object". It is an
 * expression that, at runtime, must be evaluated into an instance of a user-defined class. The second one is
 * called "name". It is a token whose lexeme represents the name of the property that is attempting to be 
 * retrieved and returned. 
 */
struct Get : Expr, public std::enable_shared_from_this<Get>{
  // This struct here represents a "Get" expression: someObject.someProperty
  // object -> someObject
  // name -> someProperty
  // At runtime, it will use a token of type IDENTIFIER to read the property with that name from the object
  // that the expression evaluates to.
  const std::shared_ptr<Expr> object;
  const Token name;

  /**
   * @brief Constructs a Get node of the Bleach AST (Abstract Syntax Tree). 
   *
   * This constructor initializes a Get object with the two attributes that were mentioned above ("object" and
   * "name").
   *
   * @param object: The expression that must be evaluated into an object during runtime.
   * @param name: The token whose lexeme represents the name of the object's property that is attempting to be 
   * retrieved and returned.
  **/
  Get(std::shared_ptr<Expr> object, Token name)
    : object{std::move(object)}, name{std::move(name)}
  {}

  std::any accept(ExprVisitor& visitor) override{
    return visitor.visitGetExpr(shared_from_this());
  }
};

/**
 * @struct Grouping
 * 
 * @brief Defines a struct to represent a grouping expression node from the AST of the Bleach language.
 *
 * The Grouping struct defines a struct to represent a grouping expression node from the AST (Abstract Syntax 
 * Tree) of the Bleach language. A grouping expression is just an expression that is enclosed by parentheses.
 * This struct has only one attribute called "expression" that represents the expression inside the parentheses.
 */
struct Grouping : Expr, public std::enable_shared_from_this<Grouping>{
  const std::shared_ptr<Expr> expression;

  /**
   * @brief Constructs a Grouping node of the Bleach AST (Abstract Syntax Tree). 
   *
   * This constructor initializes a Grouping object with the a pointer to the expression that is present inside
   * the parentheses.
   *
   * @param expression: The expression that is presented inside the parentheses of a grouping node.
  **/
  Grouping(std::shared_ptr<Expr> expression)
    : expression{std::move(expression)}
  {}

  std::any accept(ExprVisitor& visitor) override{
    return visitor.visitGroupingExpr(shared_from_this());
  }
};

/**
 * @struct Index
 * 
 * @brief Defines a struct to represent an indexing expression node from the AST of the Bleach language.
 *
 * The Index struct defines a struct to represent an indexing expression node from the AST (Abstract Syntax 
 * Tree) of the Bleach language. An indexing expression is just an expression that is enclosed by square brackets.
 * This struct has three attributes. The first one is called "object". This one represents the value that is
 * being indexed. The second one is called "index". This one represents the expression inside the square
 * brackets. The third (and last one) is called "rightBracket". This attribute is just stored in case an error
 * must be reported to the user.
 */
struct Index : Expr, public std::enable_shared_from_this<Index>{
  const std::shared_ptr<Expr> object; // The object being indexed, can be a variable (that holds a value of list type or str type) or a literal (list literal or string literal).
  const std::shared_ptr<Expr> index;
  const Token rightBracket;

  /**
   * @brief Constructs an Index node of the Bleach AST (Abstract Syntax Tree). 
   *
   * This constructor initializes an Index object with the three attributes that were mentioned above ("object",
   * "index" and "rightBracket").
   *
   * @param object: The expression that must be evaluated into a specific callable entity during runtime (an
   * instance of the "BleachListInstance" class or an instance of the "BleachStrInstance" class).
   * @param index: The expression whose value computed at runtime gives the index that will be accessed from the
   * value of "list" or "str" type. 
   * @param rightBracket: The token that represents a right bracket (']'). This token is used for error reporting
   * purposes.
  **/
  Index(std::shared_ptr<Expr> object, std::shared_ptr<Expr> index, Token rightBracket)
    : object{std::move(object)}, index{std::move(index)}, rightBracket{std::move(rightBracket)}
  {}

  std::any accept(ExprVisitor& visitor) override{
    return visitor.visitIndexExpr(shared_from_this());
  }
};

/**
 * @struct LambdaFunction
 * 
 * @brief Defines a struct to represent a lambda function (anonymous function) expression node from the AST of 
 * the Bleach language.
 *
 * The LambdaFunction struct defines a struct to represent a lambda function (anonymous function) expression 
 * node from the AST (Abstract Syntax Tree) of the Bleach language. A lambda function expression is just an 
 * expression that, during runtime, evaluates to an instance of the BleachLambdaFunction class that must be 
 * assigned to a variable.
 * This struct has only two attributes: The first one is called "parameters". It is a list of tokens that 
 * represents the list of parameters of the lambda (anonymous) function. The second one is called "body". It is
 * a list of statements. Such list contains every statement, in order, that will be executed by the lambda 
 * function during runtime.
 */
struct LambdaFunction : Expr, public std::enable_shared_from_this<LambdaFunction>{
  const std::vector<Token> parameters;
  const std::vector<std::shared_ptr<Stmt>> body;

  /**
   * @brief Constructs a LambdaFunction node of the Bleach AST (Abstract Syntax Tree). 
   *
   * This constructor initializes a LambdaFunction object with the two attributes that were mentioned above 
   * ("parameters" and "body").
   *
   * @param parameters: The list of tokens where the lexeme of each token represents the name of a parameter
   * expected by the lambda (anonymous) function.
   * @param body: The list of statements that are going to be executed during runtime when the runtime 
   * representation of this lambda (anonymous) function is called.
  **/
  LambdaFunction(std::vector<Token> parameters, std::vector<std::shared_ptr<Stmt>> body)
    : parameters{std::move(parameters)}, body{std::move(body)}
  {}

  std::any accept(ExprVisitor& visitor) override{
    return visitor.visitLambdaFunctionExpr(shared_from_this());
  }
};

/**
 * @struct Literal
 * 
 * @brief Defines a struct to represent a literal expression node from the AST of the Bleach language.
 *
 * The Literal struct defines a struct to represent a literal expression node from the AST (Abstract Syntax
 * Tree) of the Bleach language. A literal expression is an expression that is just a value. This struct has
 * only one attribute called "value". This one represents the literal value present inside such struct.
 */
struct Literal : Expr, public std::enable_shared_from_this<Literal>{
  std::any value;

  /**
   * @brief Constructs a Literal node of the Bleach AST (Abstract Syntax Tree). 
   *
   * This constructor initializes a Literal object with the value that such literal represents.
   *
   * @param value: The value that was generated by the literal.
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
 * represents the operands of a logical expression ("left" and "right") and another that represents the kind 
 * of operation that will be performed on the two operands ("op").
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
   * This constructor initializes a Logical object with the left operand, right operand and the operator that
   * represents the logical operation about to be performed on these operands.
   *
   * @param left: The left operand of the logical operator.
   * @param op: The operator that represents the logical operation about to be performed on the two operands.
   * @param right: The right operand of the logical operator.
  **/
  Logical(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right)
    : left{std::move(left)}, op{std::move(op)}, right{std::move(right)} 
  {}

  std::any accept(ExprVisitor& visitor) override{
    return visitor.visitLogicalExpr(shared_from_this());
  }
};

/**
 * @struct Self
 * 
 * @brief Defines a struct to represent a self expression node from the AST of the Bleach language.
 *
 * The Self struct defines a struct to represent a self expression node from the AST (Abstract Syntax Tree) of 
 * the Bleach language. A self expression is an expression that just produces a value. However, such value is
 * very specific: the value is always the instance of a class. This expression is supposed to be used only in
 * the methods of a class declaration.
 * This struct has only one attribute called "keyword". This attribute is the token whose lexeme is the "self"
 * keyword. 
 */
struct Self : Expr, public std::enable_shared_from_this<Self>{
  const Token keyword;

  /**
   * @brief Constructs a Self node of the Bleach AST (Abstract Syntax Tree). 
   *
   * This constructor initializes a Self object with the only attribute that has been mentioned above.
   *
   * @param keyword: The token whose lexeme is the "self" keyword.
  **/
  Self(Token keyword)
    : keyword{std::move(keyword)}
  {}

  std::any accept(ExprVisitor& visitor) override{
    return visitor.visitSelfExpr(shared_from_this());
  }
};

/**
 * @struct Set
 * 
 * @brief Defines a struct to represent a set expression node from the AST of the Bleach language.
 *
 * The Set struct defines a struct to represent a set expression node from the AST (Abstract Syntax Tree) of the
 * Bleach language. A set expression is an expression that essentialy assigns a value to a specific 
 * field/attribute from an instance of a class. This struct has only three attributes: The first one is called
 * "object". It is an expression that, at runtime, must be evaluated into an instance of a user-defined class.
 * The second one is called "name". It is a token whose lexeme represents the name of the field/attribute that 
 * will receive a value during runtime. The third attribute is called "value". It is an expression that will be
 * evaluated to a value at runtime and such produced value will be assigned to the field/attribute of a specific
 * instance of an user-defined class. 
 */
struct Set : Expr, public std::enable_shared_from_this<Set>{
  // This struct here represents a "Set" expression: someObject.someProperty = someValue
  // object -> someObject
  // name -> someProperty
  // value -> someValue
  // At runtime, it will use a token of type IDENTIFIER to find out where the property with that name from the 
  // object that the expression evaluates to is stored, so it can assign the value to it.
  const std::shared_ptr<Expr> object;
  const Token name;
  const std::shared_ptr<Expr> value;

  /**
   * @brief Constructs a Set node of the Bleach AST (Abstract Syntax Tree). 
   *
   * This constructor initializes a Set object with the three attributes that were mentioned above ("object",
   * "name" and "value").
   *
   * @param object: The expression that must be evaluated into an object during runtime.
   * @param name: The token whose lexeme represents the name of the object's field/attribute that is attempting
   * to be retrieved and returned.
   * @param value: The expression whose value (produced at runtime) will be assigned to the specific 
   * field/attribute.
  **/
  Set(std::shared_ptr<Expr> object, Token name, std::shared_ptr<Expr> value)
    : object{std::move(object)}, name{std::move(name)}, value{std::move(value)}
  {}

  std::any accept(ExprVisitor& visitor) override{
    return visitor.visitSetExpr(shared_from_this());
  }
};

/**
 * @struct Super
 * 
 * @brief Defines a struct to represent a super expression node from the AST of the Bleach language.
 *
 * The Super struct defines a struct to represent a super expression node from the AST (Abstract Syntax Tree) of 
 * the Bleach language. A super expression is an expression that can be used just in very specific cases. Being
 * more explicitly about it, such type of expression can only be used inside a class declaration that has a 
 * superclass. Moreover, after the "super" expression, it's expected to have the following tokens: "." and 
 * "identifier". In short, such expression is used to call a method from the superclass of a class.
 * This struct has only two attributes. The first one is called "keyword". This attribute is the token whose 
 * lexeme is the "super" keyword. The second one is called "method". This attribute is also a token, but this is 
 * a token whose lexeme is the name of the method that is being called on the superclass of the class this 
 * expression has appeared. 
 */
struct Super : Expr, public std::enable_shared_from_this<Super>{
  const Token keyword;
  const Token method;

  /**
   * @brief Constructs a Super node of the Bleach AST (Abstract Syntax Tree). 
   *
   * This constructor initializes a Super object with the only attribute that has been mentioned above.
   *
   * @param keyword: The token whose lexeme is the "super" keyword.
   * @param method: The token whose lexeme is the name of the method from the superclass that a method from a 
   * instance of a class is calling.
  **/
  Super(Token keyword, Token method)
    : keyword{std::move(keyword)}, method{std::move(method)}
  {}

  std::any accept(ExprVisitor& visitor) override{
    return visitor.visitSuperExpr(shared_from_this());
  }
};

/**
 * @struct Ternary
 * 
 * @brief Defines a struct to represent a ternary expression node from the AST of the Bleach language.
 *
 * The Ternary struct defines a struct to represent a ternary expression node from the AST (Abstract Syntax
 * Tree) of the Bleach language. A ternary expression is an expression that has three operands, as its name
 * suggests. Such struct has three attributes: The first one is called "condition" which will be evaluated to 
 * true or false. The other two attributes are called "ifBranch" and "elseBranch". If the "condition" attribute
 * is evaluated to true, then the attribute "ifBranch", which is an expression, will be evaluated. Otherwise, 
 * the attribute "elseBranch", which is also an expression, is the one that will be evaluated.
 */
struct Ternary : Expr, public std::enable_shared_from_this<Ternary>{
  const std::shared_ptr<Expr> condition;
  const std::shared_ptr<Expr> ifBranch;
  const std::shared_ptr<Expr> elseBranch;

  /**
   * @brief Constructs a Ternary node of the Bleach AST (Abstract Syntax Tree). 
   *
   * This constructor initializes a Ternary object with the three attributes that were mentioned above 
   * ("condition", "ifBranch" and "elseBranch").
   *
   * @param condition: The condition that must be evaluated as true of false.
   * @param ifBranch: The expression whose value will be produced in case the attribute "condition" is evaluated 
   * to true during runtime.
   * @param elseBranch: The expression whose value will be produced in case the attribute "condition" is 
   * evaluated to false during runtime.
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
 * Such struct has only two attributes: The first one is called "op". It represents the operator of the unary 
 * expression. The second one is called "right". It represents the operand on which the operator will be applied
 * on.
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
   * @param op: The operator that represents the operation about to be performed on the right operand.
   * @param right: The right operand of the unary operator.
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
 * attribute: A token whose lexeme refers to the name of a variable.
 * 
 * @note The act of accessing a variable is considered an expression because it produces a value: the value that
 * is bound to the variable whose lexeme of the token "name" is referencing.
 */
struct Variable : Expr, public std::enable_shared_from_this<Variable>{
  const Token name;

  /**
   * @brief Constructs a Variable node of the Bleach AST (Abstract Syntax Tree). 
   *
   * This constructor initializes a Variable object with the attribute that was mentioned above.
   *
   * @param name: The token whose lexeme stores the name of a variable.
  **/
  Variable(Token name)
    : name{std::move(name)}
  {}

  std::any accept(ExprVisitor& visitor) override{
    return visitor.visitVariableExpr(shared_from_this());
  }
};
