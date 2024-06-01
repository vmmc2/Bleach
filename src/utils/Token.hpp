#pragma once

#include <any>
#include <string>
#include <utility>

#include "./TokenType.hpp"


/**
 * @class Token
 * @brief Represents a token with a type, lexeme, literal value, and line number.
 *
 * The Token class is used to store information about tokens encountered
 * during lexical analysis (lexing/scanning) phase in the Bleach Interpreter.
 */
class Token{
  public:
    const TokenType type; /**< Variable that represents the type of the token. */
    const std::string lexeme; /**< Variable that represents the string representation of the token. */
    const std::any literal; /**< Variable that contains the lexeme's runtime value of the token. */
    const int line; /**< Variable that indicates at which line of the source code the token appeared. Useful for error messages. */

    /**
     * @brief Constructs a Token with the specified type, lexeme, literal, and line number.
     *
     * This constructor initializes a Token object with the given type, lexeme,
     * literal value, and the line number on which the token was found.
     *
     * @param type The type of the token. This specifies the category of the token (e.g., keyword, identifier).
     * @param lexeme The lexeme of the token. This is the actual string representation of the token.
     * @param literal The literal value of the token. This can be any type and is stored as std::any.
     * @param line The line number where the token was found in the source code.
     *
     * @note The lexeme and literal parameters are moved into the Token object to avoid unnecessary copies.
     */
    Token(TokenType type, std::string lexeme, std::any literal, int line)
      : type{type}, lexeme{std::move(lexeme)}, literal{std::move(literal)}, line{line}
    {}

    /**
     * @brief Returns the string representation of a Token instance.
     * 
     * This method is responsible for generating a string representation of 
     * the Token instance based on its type value, lexeme value and literal
     * value converted to a string.
     *
     * @return The string representation of the Token instance.
     * 
     * @note This method is a const method. Which means it doesn't modify the state of the object it is called on.
    **/
    std::string toString() const{
      std::string literalAsStr;

      switch(type){
        case(TokenType::IDENTIFIER):
          literalAsStr = lexeme;
          break;
        case(TokenType::STRING):
          literalAsStr = std::any_cast<std::string>(literal);
          break;
        case(TokenType::NUMBER):
          literalAsStr = std::to_string(std::any_cast<double>(literal));
          break;
        case(TokenType::TRUE):
          literalAsStr = "true";
          break;
        case(TokenType::FALSE):
          literalAsStr = "false";
          break;
        default: // Pay attention to the fact that this includes the case where type == TokenType::NIL.
          literalAsStr = "nil";
      }
      
      return ::toString(type) + " " + lexeme + " " + literalAsStr; 
    }
};
