#pragma once

#include <string>


/**
 * @enum TokenType
 * @brief Represents the type of each valid token that can be generated by the Lexer class.
**/
enum TokenType{
  // Single-Character tokens.
  LEFT_PAREN, RIGHT_PAREN,
  LEFT_BRACE, RIGHT_BRACE,
  LEFT_BRACKET, RIGHT_BRACKET,
  COMMA, DOT, QUESTION_MARK, COLON, SEMICOLON,
  PLUS, MINUS, STAR, SLASH,

  // One or two character tokens.
  BANG, BANG_EQUAL,
  EQUAL, EQUAL_EQUAL,
  GREATER, GREATER_EQUAL,
  LESS, LESS_EQUAL,

  // Literals.
  IDENTIFIER, NUMBER, STRING,

  // Keywords.
  AND, BREAK, CLASS, CONTINUE, DO, ELSE, ELIF,
  FALSE, FOR, FUNCTION, IF, IN, LAMBDA, LET, METHOD,
  NIL, OR, PRINT, RETURN, STATIC, SUPER, THIS, TRUE, WHILE,

  // File End.
  FILE_END
};

/**
 * @brief Returns the string representation of a type of Token.
 * 
 * This function receives a value of TokenType and returns its string representation based
 * on an static array of std::string and a conversion to the TokenType (which is an enum) to an int value.
 *
 * @param type The TokenType for which we want to get its string representation.
 * @return The string representation of the given TokenType value.
**/
std::string toString(TokenType type){
  static const std::string strings[] = {
    "LEFT_PAREN", "RIGHT_PAREN",
    "LEFT_BRACE", "RIGHT_BRACE",
    "LEFT_BRACKET", "RIGHT_BRACKET",
    "COMMA", "DOT", "QUESTION_MARK", "COLON", "SEMICOLON",
    "PLUS", "MINUS", "STAR", "SLASH",
    "BANG", "BANG_EQUAL",
    "EQUAL", "EQUAL_EQUAL",
    "GREATER", "GREATER_EQUAL",
    "LESS", "LESS_EQUAL",
    "IDENTIFIER", "NUMBER", "STRING",
    "AND", "BREAK", "CLASS", "CONTINUE", "DO", "ELSE", "ELIF",
    "FALSE", "FOR", "FUNCTION", "IF", "IN", "LAMBDA", "LET", "METHOD",
    "NIL", "OR", "PRINT", "RETURN", "STATIC", "SUPER", "THIS", "TRUE", "WHILE",
    "FILE_END"
  };

  return strings[static_cast<int>(type)];
}