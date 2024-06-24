#pragma once

#include <cassert>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "../error/Error.hpp"
#include "../utils/Expr.hpp"
#include "../utils/Stmt.hpp"
#include "../utils/Token.hpp"


/**
 * @class Parser
 * 
 * @brief Performs the syntax analysis stage of the BLEACH Interpreter.
 *
 * The Parser class is responsible for iterating through the list of tokens generated by the lexer in the
 * previous stage of the interpreting pipeline and generating (if possible) an AST (Abstract Syntax Tree)
 * that reflects the structure of the program written in the Bleach language. If the generated sequence of
 * tokens is not a valid Bleach program, then the AST will not be generated and a null pointer will be produced
 * by the parser instead of the syntax tree.
 * 
 * @note As mentioned above, the parser expects the received sequence of tokens to be valid. If that's not the
 * case, then a null pointer will be returned as the result of the parsing operation.
**/
class Parser{
  private:
    struct ParseError : public std::runtime_error{ /**< Struct that represents a syntax error (parsing error). It basically works as a sentinel struct that is used to unwind the parser when a syntax error is found, so it can execute its synchronization process. */
      using std::runtime_error::runtime_error;
    };
    int current = 0; /**< Variable that points to the next token that has not been consumed yet by the parser. */
    const std::vector<Token>& tokens; /**< Variable that represents the sequence of tokens received by the parser from the lexer. Such sequence will be parsed into an AST. */

    /**
     * @brief Returns the token that has just been consumed by the parser.
     *
     * This method is responsible for returning the most recently consumed token by the parser.
     * 
     * @return A token: the token that has just been consumed by the parser.
    **/
    Token previous(){
      return tokens[current - 1];
    }

    /**
     * @brief Returns the token that is just about to be consumed by the parser.
     *
     * This method is responsible for returning the token that the parser has yet to consume. In other words, 
     * the token that is being currently pointed by the 'current' attribute of the 'Parser' class.
     * 
     * @return A Token: the next token that has not been consumed yet by the parser. In other words, the token 
     * that is currently being pointed by the 'current' attribute of the 'Parser' class.
    **/
    Token peek(){
      return tokens[current];
    }

    /**
     * @brief Checks whether or not the parser has reached the end of its token sequence.
     *
     * This method is responsible for checking if the parser has reached the end of its tokens sequence.
     * 
     * @return A boolean that signals whether or not the parser has reached the end of the tokens sequence.
    **/
    bool isAtEnd(){
      return peek().type == TokenType::FILE_END;
    }

    /**
     * @brief Checks whether or not the next token that has not been consumed yet has a token type equal to the
     * provided token type.
     *
     * This method is responsible for receiving a TokenType through the 'type' parameter and checking whether
     * this type is equal to the type of the next token that has not been consumed yet by the parser. Pay 
     * attention to the fact that this method never consumes a token. It only looks at it.
     * 
     * @param type: A TokenType value which we will use to check whether or not the next token has such value
     * as its TokenType.
     * 
     * @return A boolean that signals whether or not the next token that has not been consumed yet has the same
     * type of the provided type.
    **/
    bool check(TokenType type){
      if(isAtEnd()){
        return false;
      }

      return peek().type == type;
    }

    /**
     * @brief Reports a syntax error (parsing error) and returns a 'ParseError' instance back to its caller.
     *
     * This method is responsible for reporting a syntax error (parsing error) to the user through the console
     * (terminal) and also returning an instance of the 'ParseError' class back to its caller. 
     * 
     * @param token: The const reference (const Token&) to the Token that is the cause of the syntax error 
     * (parsing error) that was discovered by the parser.
     * @param errorMessage: The string (std::string_view) that has the contents of the error message that must
     * be shown to the user in the console (terminal) if the token that's about to be consumed doesn't have the
     * expected type.
     * 
     * @return A ParseError instance with an empty message.
    **/
    ParseError error(const Token& token, std::string_view errorMessage){
      ::error(token, errorMessage);
      return ParseError{""};
    }

    /**
     * @brief Discards tokens from the sequence of tokens that haven't been consumed yet by the parser until
     * it hits what is most likely a statement boundary or the end of the tokens sequence
     *
     * This method is responsible for discarding tokens that haven't been consumed yet from the sequence of
     * tokens until it hits what is most likely a statement boundary or the end of the tokens sequence.
     * 
     * @return Nothing (void).
    **/
    void synchronize(){
      advance();

      while(!isAtEnd()){
        if(previous().type == TokenType::SEMICOLON){ // If the token that has just been consumed is a semicolon (;), then it means the parser is about to enter a new statement.
          return;
        }

        switch(peek().type){ // If the next token about to be consumed is one of these shown below, then the parser is also about to enter a new statement.
          case(TokenType::CLASS):
          case(TokenType::FOR):
          case(TokenType::FUNCTION):
          case(TokenType::IF):
          case(TokenType::LET):
          case(TokenType::PRINT):
          case(TokenType::RETURN):
          case(TokenType::WHILE):
            return;
        }

        advance();
      }

      return;
    }    

    /**
     * @brief Checks to see if the next token to be consumed is of the given type. If so, it consumes the token
     * and everything is fine. Otherwise, a syntax error is reported and thrown.
     *
     * This method is responsible for checking whether the next token to be consumed by the parser is of the
     * given type. If that's the case, then the parser consumes it and goes on with the parsing process.
     * Otherwise, it means the parser has found a syntax error (a parsing error) and, thus, such error is
     * reported and thrown.
     * 
     * @param type: The TokenType that the parser expects the token that's about to be consumed to have.
     * @param errorMessage: The string (std::string_view) that has the contents of the error message that must
     * be shown to the user in the console (terminal) if the token that's about to be consumed doesn't have the
     * expected type.
     * 
     * @return The Token that has just been consumed by the parser. Note that this just happens if such token
     * is of the same type as the provided type.
     * 
     * @note If the token that's about to be consumed by the parser isn't of the same type as the provided type,
     * then a syntax error is reported and thrown by the 'Parser' class.
    **/
    Token consume(TokenType type, std::string_view errorMessage){
      if(check(type)){
        return advance();
      }

      throw error(peek(), errorMessage);
    }

    /**
     * @brief Checks whether the parser has reached the end of the token sequence, consumes the next token on
     * the sequence if any, and returns it.
     *
     * This method is responsible for checking whether the parser has reached the end of the token sequence.
     * If that's not the case, then it consumes the token pointed by the 'current' attribute and returns it
     * through the use of the 'previous' method.
     * 
     * @return The latest token that has been consumed by the parser.
    **/
    Token advance(){
      if(!isAtEnd()){
        current++;
      }

      return previous();
    }

    /**
     * @brief Checks whether the next to be consumed token has a type equal to one of the provided types.
     *
     * This method receives a list/sequence of TokenTypes and checks whether the next token (the token that has
     * not been consumed by the parser yet) has a TokenType equal to one of the provided TokenTypes. If it has
     * then, the token is consumed and returns true. Otherwise, the token is not consumed and the function
     * returns false.
     * 
     * @param type: A sequence of comma separated TokenType values.
     * 
     * @return A boolean that signals whether the next token (that hasn't been consumed by the parser yet) has
     * a TokenType that is equal to one of the TokenTypes provided as arguments to this function.
    **/
    template<class... T>
    bool match(T... type){
      assert((... || std::is_same_v<T, TokenType>));

      if((... || check(type))){
        advance();
        return true;
      }

      return false;
    }

    /**
     * @brief Represents the 'statement' rule inside the CFG of the Bleach language.
     *
     * This method is responsible for representing the 'statement' rule from the Context-Free Grammar of the
     * Bleach language. To understand better what the method is doing, take a look at Bleach's CFG.
     * 
     * @return A std::shared_ptr<Stmt> representing an Abstract Syntax Tree (AST) of the Bleach language for 
     * this rule.
     * 
     * @note: This method is the one that is called repeatedly when parsing a series of statements inside a 
     * script. Therefore, this is the right place to synchronize whenever the parser goes into panic mode 
     * (founds a parsing error).
     * You can see that the whole body of this method is wrapped inside a "try-catch" block in order to catch
     * the exception that is thrown when the parser begins the error recovery process.
     * This is what get the parser back to trying to parse the beginning of the next statement inside the 
     * script.
    **/
    std::shared_ptr<Stmt> statement(){
      try{
        if(match(TokenType::DO)){
          return doWhileStatement();
        }
        if(match(TokenType::FOR)){
          return forStatement();
        }
        if(match(TokenType::IF)){
          return ifStatement();
        }
        if(match(TokenType::LEFT_BRACE)){
          return std::make_shared<Block>(block());
        }
        if(match(TokenType::LET)){
          return varDeclStatement();
        }
        if(match(TokenType::PRINT)){
          return printStatement();
        }
        if(match(TokenType::WHILE)){
          return whileStatement();
        }
        return expressionStatement();
      }catch(ParseError error){
        synchronize();
        
        return nullptr;
      }
    }

    /**
     * @brief Represents the 'block' rule inside the CFG of the Bleach language.
     *
     * This method is responsible for representing the 'block' rule from the Context-Free Grammar of the Bleach
     * language. To understand better what the method is doing, take a look at Bleach's CFG.
     * 
     * @return A std::vector<std::shared_ptr<Stmt>> representing a series/sequence of Abstract Syntax Trees (AST)
     * of the Bleach language for this rule.
    **/
    std::vector<std::shared_ptr<Stmt>> block(){
      std::vector<std::shared_ptr<Stmt>> statements;

      while(!check(TokenType::RIGHT_BRACE) && !isAtEnd()){ // It tries to parse individual statements until it finds the '}' token or reaches the end of the file. 
        statements.push_back(statement());
      }

      consume(TokenType::RIGHT_BRACE, "Expected a '}' after a block"); // After the last statement of a block, the next expected token is '}'. If that's not the case, then the parser has found a parsing error (syntax error).

      return statements;
    }

    /**
     * @brief Represents the 'doWhileStmt' rule inside the CFG of the Bleach language.
     *
     * This method is responsible for representing the 'doWhileStmt' rule from the Context-Free Grammar of 
     * the Bleach language. To understand better what the method is doing, take a look at Bleach's CFG.
     * 
     * @return A std::shared_ptr<Stmt> representing an Abstract Syntax Tree (AST) of the Bleach language for 
     * this rule.
    **/
    std::shared_ptr<Stmt> doWhileStatement(){
      std::shared_ptr<Stmt> body = statement();

      consume(TokenType::WHILE, "Expected the 'while' keyword after the body of the 'do-while' statement");
      consume(TokenType::LEFT_PAREN, "Expected a '(' after the 'while' keyword");
      std::shared_ptr<Expr> condition = expression();
      consume(TokenType::RIGHT_PAREN, "Expected a ')' after the 'do-while' condition");
      consume(TokenType::SEMICOLON, "Expected a ';' after the 'do-while' statement");

      return std::make_shared<DoWhile>(condition, body);
    }

    /**
     * @brief Represents the 'forStmt' rule inside the CFG of the Bleach language.
     *
     * This method is responsible for representing the 'forStmt' rule from the Context-Free Grammar of 
     * the Bleach language. To understand better what the method is doing, take a look at Bleach's CFG.
     * 
     * @return A std::shared_ptr<Stmt> representing an Abstract Syntax Tree (AST) of the Bleach language for 
     * this rule.
    **/
    std::shared_ptr<Stmt> forStatement(){
      consume(TokenType::LEFT_PAREN, "Expected a '(' after the 'for' keyword");

      std::shared_ptr<Stmt> initializer;
      if(match(TokenType::SEMICOLON)){
        initializer = nullptr;
      }else if(match(TokenType::LET)){
        initializer = varDeclStatement();
      }else{
        initializer = expressionStatement();
      }

      std::shared_ptr<Expr> condition = nullptr;
      if(!check(TokenType::SEMICOLON)){
        condition = expression();
      }
      consume(TokenType::SEMICOLON, "Expected a ';' after the 'for' loop condition");


      std::shared_ptr<Expr> increment = nullptr;
      if(!check(TokenType::RIGHT_PAREN)){
        increment = expression();
      }
      consume(TokenType::RIGHT_PAREN, "Expected a ')' after the 'for' clauses");

      std::shared_ptr<Stmt> body = statement();

      // We construct the Bleach language AST node that represents the For statement node by using already
      // existing Bleach language AST nodes. In particular, we use the Block statement node, the Expression
      // statement node and the While statemente node.
      if(increment != nullptr){
        body = std::make_shared<Block>(
          std::vector<std::shared_ptr<Stmt>>{
            body,
            std::make_shared<Expression>(increment) // Pay attention to the fact that we use the "Expression" type (Expression Statement) instead of the "Expr" type.
          }
        );
      }

      if(condition == nullptr){ // Remember, if the "for" loop does not have a condition, then it works like a "while(true){ ... }" loop.
        condition = std::make_shared<Literal>(true); 
      }
      body = std::make_shared<While>(condition, body);

      if(initializer != nullptr){ // If there is an initializer (a varDeclStmt or a expressionStmt), then we have to wrap the "while" look in a block. By doing so, the variable declaration statement or expression statement will be executed before the while loop is executed.
        body = std::make_shared<Block>(
          std::vector<std::shared_ptr<Stmt>>{
            initializer,
            body
          }
        );
      }

      return body;
    }

    /**
     * @brief Represents the 'exprStmt' rule inside the CFG of the Bleach language.
     *
     * This method is responsible for representing the 'exprStmt' rule from the Context-Free Grammar of the 
     * Bleach language. To understand better what the method is doing, take a look at Bleach's CFG.
     * 
     * @return A std::shared_ptr<Stmt> representing an Abstract Syntax Tree (AST) of the Bleach language for 
     * this rule.
    **/
    std::shared_ptr<Stmt> expressionStatement(){
      std::shared_ptr<Expr> value = expression();
      consume(TokenType::SEMICOLON, "Expected a ';' after an expression");

      return std::make_shared<Expression>(value);
    }

    /**
     * @brief Represents the 'ifStmt' rule inside the CFG of the Bleach language.
     *
     * This method is responsible for representing the 'ifStmt' rule from the Context-Free Grammar of the 
     * Bleach language. To understand better what the method is doing, take a look at Bleach's CFG.
     * 
     * @return A std::shared_ptr<Stmt> representing an Abstract Syntax Tree (AST) of the Bleach language for 
     * this rule.
    **/
    std::shared_ptr<Stmt> ifStatement(){
      consume(TokenType::LEFT_PAREN, "Expected a '(' after the 'if' keyword");
      std::shared_ptr<Expr> ifCondition = expression();
      consume(TokenType::RIGHT_PAREN, "Expected a ')' after the 'if' condition");
      std::shared_ptr<Stmt> ifBranch = statement(); // This line is what makes the "else" bound to nearest "if" that precedes it.

      std::vector<std::shared_ptr<Expr>> elifConditions;
      std::vector<std::shared_ptr<Stmt>> elifBranches;
      std::shared_ptr<Stmt> elseBranch = nullptr;

      while(match(TokenType::ELIF)){
        consume(TokenType::LEFT_PAREN, "Expected a '(' after the 'elif' keyword");
        std::shared_ptr<Expr> currElifCondition = expression();
        consume(TokenType::RIGHT_PAREN, "Expected a ')' after the 'elif' condition");
        std::shared_ptr<Stmt> currElifBranch = statement();

        elifConditions.push_back(currElifCondition);
        elifBranches.push_back(currElifBranch);
      }

      if(match(TokenType::ELSE)){
        elseBranch = statement();
      }

      return std::make_shared<If>(ifCondition, ifBranch, elifConditions, elifBranches, elseBranch);
    }

    /**
     * @brief Represents the 'printStmt' rule inside the CFG of the Bleach language.
     *
     * This method is responsible for representing the 'printStmt' rule from the Context-Free Grammar of 
     * the Bleach language. To understand better what the method is doing, take a look at Bleach's CFG.
     * 
     * @return A std::shared_ptr<Stmt> representing an Abstract Syntax Tree (AST) of the Bleach language for 
     * this rule.
    **/
    std::shared_ptr<Stmt> printStatement(){
      std::shared_ptr<Expr> value = expression();
      consume(TokenType::SEMICOLON, "Expected ';' after the value of a 'print' statement");
      
      return std::make_shared<Print>(value);
    }

    /**
     * @brief Represents the 'varDeclStmt' rule inside the CFG of the Bleach language.
     *
     * This method is responsible for representing the 'varDeclStmt' rule from the Context-Free Grammar of 
     * the Bleach language. To understand better what the method is doing, take a look at Bleach's CFG.
     * 
     * @return A std::shared_ptr<Stmt> representing an Abstract Syntax Tree (AST) of the Bleach language for 
     * this rule.
     * 
     * @note: If there is no initializer expression after the variable name, then its default value is nil.
     * Look at the default value of the variable "initializer" shown below (nullptr).
    **/
    std::shared_ptr<Stmt> varDeclStatement(){
      Token name = consume(TokenType::IDENTIFIER, "Expected a variable name after the 'let' keyword");
      std::shared_ptr<Expr> initializer = nullptr;

      if(match(TokenType::EQUAL)){
        initializer = expression();
      }

      consume(TokenType::SEMICOLON, "Expected a ';' after a variable declaration statement");

      return std::make_shared<Var>(name, initializer);
    }

    /**
     * @brief Represents the 'whileStmt' rule inside the CFG of the Bleach language.
     *
     * This method is responsible for representing the 'whileStmt' rule from the Context-Free Grammar of 
     * the Bleach language. To understand better what the method is doing, take a look at Bleach's CFG.
     * 
     * @return A std::shared_ptr<Stmt> representing an Abstract Syntax Tree (AST) of the Bleach language for 
     * this rule.
    **/
    std::shared_ptr<Stmt> whileStatement(){
      consume(TokenType::LEFT_PAREN, "Expected a '(' after the 'while' keyword");
      std::shared_ptr<Expr> condition = expression();
      consume(TokenType::RIGHT_PAREN, "Expected a ')' after the 'while' condition");

      std::shared_ptr<Stmt> body = statement(); // The body of a while statement can be any kind of statement.

      return std::make_shared<While>(condition, body);
    }

    /**
     * @brief Represents the 'expression' rule inside the CFG of the Bleach language.
     *
     * This method is responsible for representing the 'expression' rule from the Context-Free Grammar of the
     * Bleach language. To understand better what the method is doing, take a look at Bleach's CFG.
     * 
     * @return A std::shared_ptr<Expr> representing an Abstract Syntax Tree (AST) of the Bleach language for 
     * this rule.
    **/
    std::shared_ptr<Expr> expression(){
      return assignment();
    }

    /**
     * @brief Represents the 'assignment' rule inside the CFG of the Bleach language.
     *
     * This method is responsible for representing the 'assignment' rule from the Context-Free Grammar of the
     * Bleach language. To understand better what the method is doing, take a look at Bleach's CFG.
     * 
     * @return A std::shared_ptr<Expr> representing an Abstract Syntax Tree (AST) of the Bleach language for 
     * this rule.
    **/
    std::shared_ptr<Expr> assignment(){
      std::shared_ptr<Expr> expr = ternary(); // It's expected that the it will encounter a 'Variable' expression. Remember, the left-hand side can be any expression of higher precedence.
    
      if(match(TokenType::EQUAL)){ // The interpreter has found the assignment operator ('='). This means we need to parse the right-hand side of the assignment expression. Remember that the assignment expression can reference itself (recursion).
        Token equals = previous(); // Grab the assignment operator ('=').
        std::shared_ptr<Expr> value = assignment(); // This here is what makes the right-to-left associativity of the assignment expression/operator evident. Recursion -> right associativity and Loop -> left associativity.

        if(Variable* e = dynamic_cast<Variable*>(expr.get())){ // This cast is what certify us that the right-hand side operand of the assignment expression is, indeed, a 'Variable' expression.
          Token name = e->name;
          return std::make_shared<Assign>(std::move(name), value); // This also makes the right-to-left associativity of the assignment expression/operator evident. Recursion -> right associativity and Loop -> left associativity.
        }
      
        error(equals, "Invalid assignment target");
      }
    
      return expr;
    }

    /**
     * @brief Represents the 'ternary' rule inside the CFG of the Bleach language.
     *
     * This method is responsible for representing the 'ternary' rule from the Context-Free Grammar of the
     * Bleach language. To understand better what the method is doing, take a look at Bleach's CFG.
     * 
     * @return A std::shared_ptr<Expr> representing an Abstract Syntax Tree (AST) of the Bleach language for 
     * this rule.
    **/
    std::shared_ptr<Expr> ternary(){
      std::shared_ptr<Expr> expr = logicalOr();

      if(match(TokenType::QUESTION_MARK)){ // Checks whether there is a "?" after "logicalOr". If that's the case, then it's expected that the parser has indeed found a ternary expression.
        std::shared_ptr<Expr> ifBranch = expression();
        consume(TokenType::COLON, "Expected a ':' after the 'if' branch of a ternary expression");
        std::shared_ptr<Expr> elseBranch = expression();
        expr = std::make_shared<Ternary>(expr, ifBranch, elseBranch);
      }

      return expr;
    }

    /**
     * @brief Represents the 'logicalOr' rule inside the CFG of the Bleach language.
     *
     * This method is responsible for representing the 'logicalOr' rule from the Context-Free Grammar of the
     * Bleach language. To understand better what the method is doing, take a look at Bleach's CFG.
     * 
     * @return A std::shared_ptr<Expr> representing an Abstract Syntax Tree (AST) of the Bleach language for 
     * this rule.
    **/
    std::shared_ptr<Expr> logicalOr(){
      std::shared_ptr<Expr> expr = logicalAnd();

      while(match(TokenType::OR)){ // If a match happens, then it's expected that the parser has indeed found a 'logicalOr' expression. This loop is what makes the left-to-right associativity of this operator evident.
        Token op = previous();
        std::shared_ptr<Expr> right = logicalAnd();
        expr = std::make_shared<Logical>(expr, std::move(op), right);
      }

      return expr;
    }

    /**
     * @brief Represents the 'logicalAnd' rule inside the CFG of the Bleach language.
     *
     * This method is responsible for representing the 'logicalAnd' rule from the Context-Free Grammar of the
     * Bleach language. To understand better what the method is doing, take a look at Bleach's CFG.
     * 
     * @return A std::shared_ptr<Expr> representing an Abstract Syntax Tree (AST) of the Bleach language for 
     * this rule.
    **/
    std::shared_ptr<Expr> logicalAnd(){
      std::shared_ptr<Expr> expr = equality();

      while(match(TokenType::AND)){ // If a match happens, then it's expected that the parser has indeed found a 'logicalAnd' expression. This loop is what makes the left-to-right associativity of this operator evident.
        Token op = previous();
        std::shared_ptr<Expr> right = equality();
        expr = std::make_shared<Logical>(expr, std::move(op), right);
      }

      return expr;
    }

    /**
     * @brief Represents the 'equality' rule inside the CFG of the Bleach language.
     *
     * This method is responsible for representing the 'equality' rule from the Context-Free Grammar of the
     * Bleach language. To understand better what the method is doing, take a look at Bleach's CFG.
     * 
     * @return A std::shared_ptr<Expr> representing an Abstract Syntax Tree (AST) of the Bleach language for 
     * this rule.
    **/
    std::shared_ptr<Expr> equality(){
      std::shared_ptr<Expr> expr = comparison();

      while(match(TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL)){
        Token op = previous();
        std::shared_ptr<Expr> right = comparison();
        expr = std::make_shared<Binary>(expr, std::move(op), right); // The left-to-right associativity of the '!=' and '==' operators is made evident here.
      }

      return expr;
    }

    /**
     * @brief Represents the 'comparison' rule inside the CFG of the Bleach language.
     *
     * This method is responsible for representing the 'comparison' rule from the Context-Free Grammar of the
     * Bleach language. To understand better what the method is doing, take a look at Bleach's CFG.
     * 
     * @return A std::shared_ptr<Expr> representing an Abstract Syntax Tree (AST) of the Bleach language for 
     * this rule.
    **/
    std::shared_ptr<Expr> comparison(){
      std::shared_ptr<Expr> expr = term();

      while(match(TokenType::LESS, TokenType::LESS_EQUAL, TokenType::GREATER, TokenType::GREATER_EQUAL)){
        Token op = previous();
        std::shared_ptr<Expr> right = term();
        expr = std::make_shared<Binary>(expr, op, right); // The left-to-right associativity of the '<', '<=', '>' and '>=' operators is made evident here.
      }

      return expr;
    }

    /**
     * @brief Represents the 'term' rule inside the CFG of the Bleach language.
     *
     * This method is responsible for representing the 'term' rule from the Context-Free Grammar of the
     * Bleach language. To understand better what the method is doing, take a look at Bleach's CFG.
     * 
     * @return A std::shared_ptr<Expr> representing an Abstract Syntax Tree (AST) of the Bleach language for 
     * this rule.
    **/
    std::shared_ptr<Expr> term(){
      std::shared_ptr<Expr> expr = factor();

      while(match(TokenType::PLUS, TokenType::MINUS)){
        Token op = previous();
        std::shared_ptr<Expr> right = factor();
        expr = std::make_shared<Binary>(expr, op, right); // The left-to-right associativity of the '+' and '-' operators is made evident here.
      }

      return expr;
    }

    /**
     * @brief Represents the 'factor' rule inside the CFG of the Bleach language.
     *
     * This method is responsible for representing the 'factor' rule from the Context-Free Grammar of the
     * Bleach language. To understand better what the method is doing, take a look at Bleach's CFG.
     * 
     * @return A std::shared_ptr<Expr> representing an Abstract Syntax Tree (AST) of the Bleach language for 
     * this rule.
    **/
    std::shared_ptr<Expr> factor(){
      std::shared_ptr<Expr> expr = unary();

      while(match(TokenType::STAR, TokenType::SLASH)){
        Token op = previous();
        std::shared_ptr<Expr> right = unary();
        expr = std::make_shared<Binary>(expr, op, right); // The left-to-right associativity of the '*' and '/' operators is made evident here.
      }

      return expr;
    }

    /**
     * @brief Represents the 'unary' rule inside the CFG of the Bleach language.
     *
     * This method is responsible for representing the 'unary' rule from the Context-Free Grammar of the
     * Bleach language. To understand better what the method is doing, take a look at Bleach's CFG.
     * 
     * @return A std::shared_ptr<Expr> representing an Abstract Syntax Tree (AST) of the Bleach language for 
     * this rule.
    **/
    std::shared_ptr<Expr> unary(){
      if(match(TokenType::BANG, TokenType::MINUS)){
        Token op = previous();
        std::shared_ptr<Expr> right = unary();
        return std::make_shared<Unary>(op, right); // The right-to-left associativity of the '!' and '-' operators is made evident here.
      }

      // Think like this is an "else" branch to the "if" branch above.
      return call();
    }

    // Auxiliary method to the "call" method
    std::shared_ptr<Expr> finishCallExpr(std::shared_ptr<Expr> callee){
      std::vector<std::shared_ptr<Expr>> arguments;

      if(!check(TokenType::RIGHT_PAREN)){ // This "if" statement here is responsible for dealing with the parsing of the arguments inside a function call.
        do{
          arguments.push_back(expression());
        }while(match(TokenType::COMMA));
      }

      Token paren = consume(TokenType::RIGHT_PAREN, "Expected a ')' after the arguments of a function call");

      return std::make_shared<Call>(callee, std::move(paren), std::move(arguments));
    }

    std::shared_ptr<Expr> call(){
      std::shared_ptr<Expr> expr = primary();

      while(true){ // This loop along with the "finishCallExpr" is what allows the user to write sequential function call.
        if(match(TokenType::LEFT_PAREN)){
          expr = finishCallExpr(expr); // Calls an auxiliary method to finish the parsing of a call expression.
        }else{
          break;
        }
      }

      return expr;
    }

    /**
     * @brief Represents the 'primary' rule inside the CFG of the Bleach language.
     *
     * This method is responsible for representing the 'primary' rule from the Context-Free Grammar of the
     * Bleach language. To understand better what the method is doing, take a look at Bleach's CFG.
     * 
     * @return A std::shared_ptr<Expr> representing an Abstract Syntax Tree (AST) of the Bleach language for 
     * this rule.
    **/
    std::shared_ptr<Expr> primary(){
      if(match(TokenType::FALSE)){
        return std::make_shared<Literal>(false);
      }
      if(match(TokenType::TRUE)){
        return std::make_shared<Literal>(true);
      }
      if(match(TokenType::NIL)){
        return std::make_shared<Literal>(nullptr);
      }
      if(match(TokenType::NUMBER, TokenType::STRING)){
        return std::make_shared<Literal>(previous().literal);
      }
      if(match(TokenType::IDENTIFIER)){ // Parses a variable expression, which is an expression that is responsible for getting the value bound to a variable during runtime.
        return std::make_shared<Variable>(previous());
      }
      if(match(TokenType::LEFT_PAREN)){
        std::shared_ptr<Expr> expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expect a ')' after an expression");
        return std::make_shared<Grouping>(expr);
      }

      throw error(peek(), "Expected an expression");
    }

  public:
    /**
     * @brief Constructs a Parser with the sequence of tokens (std::vector<Token>) that needs to be parsed. 
     *
     * This constructor initializes a Parser object with the given sequence of tokens generated by a Lexer 
     * object, represented as a std::vector<Token>.
     *
     * @param tokens The sequence of tokens, produced by the Lexer, that is going to be parsed by a Parser
     * object in order to create an Abstract Syntax Tree (AST).
    **/
    Parser(const std::vector<Token>& tokens)
      : tokens{tokens}
    {}

    /**
     * @brief Starts the parsing step of the interpreter. Also, it represents the 'program' rule from the
     * Bleach CFG.
     *
     * This method is responsible for starting the parsing process of the interpreter. Furthermore, this
     * method works as a direct translation of the 'program' rule from the Bleach CFG.
     * 
     * @return A std::vector<std::shared_ptr<Stmt>> representing the list of ASTs of the Bleach language 
     * generated from the whole sequence of tokens. In this scenario, each of these ASTs is the syntax tree
     * representation of a statement.
    **/
    std::vector<std::shared_ptr<Stmt>> parse(){
      std::vector<std::shared_ptr<Stmt>> statements;

      while(!isAtEnd()){
        statements.push_back(statement());
      }

      return statements;
    }
};
