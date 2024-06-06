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
#include "../utils/Token.hpp"


/**
 * @class Parser
 * @brief Performs 
 *
 * The Parser class is responsible for 
 * @note The parser
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
     * @brief Represents the 'expression' rule inside the CFG of the Bleach language.
     *
     * This method is responsible for representing the 'expression' rule from the Context-Free Grammar of the
     * Bleach language. To understand better what the method is doing, take a look at Bleach's CFG.
     * 
     * @return A std::shared_ptr<Expr> representing the Abstract Syntax Tree (AST) of the Bleach language for this rule.
    **/
    std::shared_ptr<Expr> expression(){
      return equality();
    }

    /**
     * @brief Represents the 'equality' rule inside the CFG of the Bleach language.
     *
     * This method is responsible for representing the 'equality' rule from the Context-Free Grammar of the
     * Bleach language. To understand better what the method is doing, take a look at Bleach's CFG.
     * 
     * @return A std::shared_ptr<Expr> representing the Abstract Syntax Tree (AST) of the Bleach language for this rule.
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
     * @return A std::shared_ptr<Expr> representing the Abstract Syntax Tree (AST) of the Bleach language for this rule.
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
     * @return A std::shared_ptr<Expr> representing the Abstract Syntax Tree (AST) of the Bleach language for this rule.
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
     * @return A std::shared_ptr<Expr> representing the Abstract Syntax Tree (AST) of the Bleach language for this rule.
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
     * @return A std::shared_ptr<Expr> representing the Abstract Syntax Tree (AST) of the Bleach language for this rule.
    **/
    std::shared_ptr<Expr> unary(){
      if(match(TokenType::BANG, TokenType::MINUS)){
        Token op = previous();
        std::shared_ptr<Expr> right = unary();
        return std::make_shared<Unary>(op, right); // The right-to-left associativity of the '!' and '-' operators is made evident here.
      }

      return primary();
    }

    /**
     * @brief Represents the 'primary' rule inside the CFG of the Bleach language.
     *
     * This method is responsible for representing the 'primary' rule from the Context-Free Grammar of the
     * Bleach language. To understand better what the method is doing, take a look at Bleach's CFG.
     * 
     * @return A std::shared_ptr<Expr> representing the Abstract Syntax Tree (AST) of the Bleach language for this rule.
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
      if(match(TokenType::LEFT_PAREN)){
        std::shared_ptr<Expr> expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expect a ')' after an expression.");
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
     * @brief Starts the parsing step of the interpreter.
     *
     * This method is responsible for starting the parsing process of the interpreter.
     * 
     * @return A std::shared_ptr<Expr> representing the Abstract Syntax Tree (AST) of the Bleach language 
     * generated from the whole sequence of tokens.
    **/
    std::shared_ptr<Expr> parse(){
      try{
        return expression();
      }catch(ParseError parseError){
        return nullptr;
      }
    }
};
