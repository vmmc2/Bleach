#pragma once

#include <string>

enum TokenType{
  // Single-character tokens.
  LEFT_PAREN, RIGHT_PAREN,
  LEFT_BRACE, RIGHT_BRACE,
  LEFT_BRACKET, RIGHT_BRACKET,
  COMMA, DOT, INTERROGATION, COLON, SEMICOLON,
  PLUS, MINUS, STAR, SLASH,

  // One or two character tokens.
  BANG, BANG_EQUAL,
  EQUAL, EQUAL_EQUAL,
  GREATER, GREATER_EQUAL,
  LESS, LESS_EQUAL,

  // Literals.
  IDENTIFIER, NUMBER, STRING,

  // Keywords.
  AND, BREAK, CLASS, CONTINUE, DO, ELSE, ELSIF,
  FALSE, FUNCTION, FOR, IF, LAMBDA, LET, METHOD,
  NIL, OR, PRINT, RETURN, SUPER, THIS, TRUE, WHILE,

  // File End.
  FILE_END
};

std::string toString(TokenType type){
  static const std::string strings[] = {
    "LEFT_PAREN", "RIGHT_PAREN",
    "LEFT_BRACE", "RIGHT_BRACE",
    "LEFT_BRACKET", "RIGHT_BRACKET",
    "COMMA", "DOT", "INTERROGATION", "COLON", "SEMICOLON",
    "PLUS", "MINUS", "STAR", "SLASH",
    "BANG", "BANG_EQUAL",
    "EQUAL", "EQUAL_EQUAL",
    "GREATER", "GREATER_EQUAL",
    "LESS", "LESS_EQUAL",
    "IDENTIFIER", "NUMBER", "STRING",
    "AND", "BREAK", "CLASS", "CONTINUE", "DO", "ELSE", "ELIF",
    "FALSE", "FUNCTION", "FOR", "IF", "LAMBDA", "LET", "METHOD",
    "NIL", "OR", "PRINT", "RETURN", "SUPER", "THIS", "TRUE", "WHILE",
    "FILE_END"
  };

  return strings[static_cast<int>(type)];
}