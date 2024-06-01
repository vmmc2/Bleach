#pragma once

#include <cassert>
#include <memory>
#include <utility>
#include <vector>

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
    int current = 0; /**< Variable that points to the next token that has not been consumed yet by the parser. */
    const std::vector<Token>& tokens; /**< Variable that represents the sequence of tokens received by the parser from the lexer. Such sequence will be parsed into an AST. */

    /**
     * @brief 
     *
     * This method 
     * 
     * @return 
    **/
    Token previous(){
      return tokens[current - 1];
    }

    /**
     * @brief 
     *
     * This method 
     * 
     * @return 
    **/
    Token peek(){
      return tokens[current];
    }

    /**
     * @brief 
     *
     * This method 
     * 
     * @return 
    **/
    bool isAtEnd(){
      return peek().type == TokenType::FILE_END;
    }

    /**
     * @brief 
     *
     * This method 
     * 
     * @return 
    **/
    bool check(TokenType type){
      if(isAtEnd()){
        return false;
      }

      return peek().type == type;
    }

    /**
     * @brief 
     *
     * This method 
     * 
     * @return 
    **/
    Token advance(){
      if(!isAtEnd()){
        current++;
      }

      return previous();
    }

    /**
     * @brief 
     *
     * This method 
     * 
     * @return 
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
     * @brief 
     *
     * This method 
     * 
     * @return 
    **/
    std::shared_ptr<Expr> expression(){
      return equality();
    }

    /**
     * @brief 
     *
     * This method 
     * 
     * @return 
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
     * @brief 
     *
     * This method 
     * 
     * @return 
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
     * @brief 
     *
     * This method 
     * 
     * @return 
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
     * @brief 
     *
     * This method 
     * 
     * @return 
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
     * @brief 
     *
     * This method 
     * 
     * @return 
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
     * @brief 
     *
     * This method 
     * 
     * @return 
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
};
