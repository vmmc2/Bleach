#pragma once

#include <map>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "../utils/Token.hpp"


/**
 * @class Lexer
 * @brief Performs the lexical analysis stage of the Bleach Interpreter.
 *
 * The Lexer class is responsible for scanning the contents of the source code from a Bleach file
 * (which is represented as a string) and breaking it down into a series of tokens that can be used
 * by the parser later on. It handles various aspects of lexical analysis, including recognizing 
 * keywords, identifiers, literals, and operators.
 *
 * @note The lexer assumes that the input is syntactically valid up to the point of lexical analysis
 * and does not handle syntax errors beyond tokenization.
 */
class Lexer{
  private:
    std::string_view sourceCode; /**< Variable that represents the source code written inside a Bleach file, which the Lexer will lex. */
    std::vector<Token> tokens; /**< Variable that represents the list of tokens that will be generated after the lexical analysis is done by the Lexer. */
    int start = 0; /**< Variable that points to the first/start character of the lexeme that is being consumed (This index is in relation to the 'sourceCode' variable). */
    int current = 0; /**< Variable that points to the character that is currently being considered. */
    int line = 1; /**< Variable that holds the information about which line the lexer is currently at with respect to the source code file. It helps the lexer to generate tokens that know their location in the source code file. */

    /**
     * @brief Signals whether the lexer has reached the end of the source code file or not.
     *
     * This method checks whether the 'current' variable value is equal or bigger than the length
     * of the string present inside the 'sourceCode' variable. If that's the case, then it means the
     * lexer has reached the end of the source code file and, thus, the lexical analysis is finished
     * and the sequence of tokens with respect to the given program has been generated. Otherwise, it
     * means the lexical analysis needs to keep going.
     * 
     * @return A boolean value that signals whether the lexer has reached the end of the string of the source code or not.
     */
    bool isAtEnd(){
      return current >= sourceCode.length();
    }

  public:
    /**
     * @brief Constructs a Lexer with the source code (as a string) present inside a Bleach (.bah) file. 
     *
     * This constructor initializes a Lexer object with the given source code of a Bleach file,
     * represented as a string.
     *
     * @param sourceCode The source code of the Bleach file intended to be interpreted. Such source code is provided as a string.
     *
     * @note The sourceCode parameter is moved into the Lexer object to avoid unnecessary copies.
     */
    Lexer(std::string_view sourceCode)
      : sourceCode{std::move(sourceCode)}
    {}

    /**
     * @brief Performs the lexical analysis on the string present inside the 'sourceCode' variable
     * and returns a list of all tokens generated by the execution of such analysis.
     * 
     * This method is responsible for performing the lexical analysis on the string that resides inside
     * the 'sourceCode' variable. The lexical analysis is responsible for performing a linear scan on
     * such string and group the characters of the string into sequences of characters that have a meaning.
     * These sequence of characters are called lexemes and with them and other information it creates tokens.
     * When the lexical analysis reaches the end of the string, a sequence of tokens is generated and returned
     * by this method.
     * 
     * @return The list of tokens generated by the lexical analysis.
    **/
    std::vector<Token> lexTokens(){
      while(!isAtEnd()){ // The lexer is at the beginning of the next lexeme to be built.
        start = current;
        lexToken();
      }
      tokens.emplace_back(Token(TokenType::FILE_END, "", nullptr, line)); // For easiness purposes, the lexer always adds a 'FILE_END' token as the last token in the sequence of tokens to signal the end of the program.

      return tokens;
    }

};