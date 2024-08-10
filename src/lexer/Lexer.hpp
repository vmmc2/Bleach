#pragma once

#include <any>
#include <map>
#include <set>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "../error/Error.hpp"
#include "../utils/Token.hpp"


/**
 * @class Lexer
 * 
 * @brief Performs the lexical analysis stage of the Bleach Interpreter.
 *
 * The Lexer class is responsible for scanning the contents of the source code from a Bleach file
 * (which is represented as a string) and breaking it down into a series of tokens that can be used
 * by the parser later on. It handles various aspects of lexical analysis, including recognizing 
 * keywords, identifiers, literals, and operators.
 *
 * @note The lexer assumes that the input is syntactically valid up to the point of lexical analysis
 * and does not handle syntax errors beyond tokenization.
**/
class Lexer{
  private:
    std::string_view sourceCode; /**< Variable that represents the source code written inside a Bleach file, which the Lexer will lex. */
    std::vector<Token> tokens; /**< Variable that represents the list of tokens that will be generated after the lexical analysis is done by the Lexer. */
    int start = 0; /**< Variable that points to the first/start character of the lexeme that is being consumed (This index is in relation to the 'sourceCode' variable). */
    int current = 0; /**< Variable that points to the character that is the next one to be consumed by the lexer. */
    int line = 1; /**< Variable that holds the information about which line the lexer is currently at with respect to the source code file. It helps the lexer to generate tokens that know their location in the source code file. */
    std::set<std::string> nativeFunctions = { /** Variable that stores the names of Bleach native functions. */
      "std::chrono::clock", 
      "std::io::readLine", "std::io::print", "std::io::fileRead", "std::io::fileWrite",
      "std::math::abs", "std::math::pow", "std::math::log", "std::math::sqrt", "std::random::random"
    };
    std::map<std::string, TokenType> keywords = { /** Variable that maps string values of Bleach keywords to its respective TokenType enum values. */
      {"and",           TokenType::AND},
      {"break",         TokenType::BREAK},
      {"class",         TokenType::CLASS},
      {"continue",      TokenType::CONTINUE},
      {"do",            TokenType::DO},
      {"elif",          TokenType::ELIF},
      {"else",          TokenType::ELSE},
      {"false",         TokenType::FALSE},
      {"for",           TokenType::FOR},
      {"function",      TokenType::FUNCTION},
      {"if",            TokenType::IF},
      {"inherits",      TokenType::INHERITS},
      {"lambda",        TokenType::LAMBDA},
      {"let",           TokenType::LET},
      {"method",        TokenType::METHOD},
      {"nil",           TokenType::NIL},
      {"or",            TokenType::OR},
      {"print",         TokenType::PRINT},
      {"return",        TokenType::RETURN},
      {"self",          TokenType::SELF},
      {"super",         TokenType::SUPER},
      {"true",          TokenType::TRUE},
      {"while",         TokenType::WHILE},
    };

    /**
     * @brief Signals whether the lexer has reached the end of the source code file or not.
     *
     * This method checks whether the 'current' variable value is equal or bigger than the length
     * of the string present inside the 'sourceCode' variable. If that's the case, then it means the
     * lexer has reached the end of the source code file and, thus, the lexical analysis is finished
     * and the sequence of tokens with respect to the given program has been generated. Otherwise, it
     * means the lexical analysis needs to keep going.
     * 
     * @return A boolean that signals whether the lexer has reached the end of the string of the
     * source code or not.
    **/
    bool isAtEnd(){
      return current >= sourceCode.length();
    }

    /**
     * @brief Consumes the next unconsumed character in the source code file and returns it.
     * 
     * This method is responsible for consuming the character pointed by the 'current' variable
     * (which is the next unconsumed character), increments 'current' by 1 and returns this character
     * that was just consumed.
     * 
     * @return A char that is the latest character that has been just consumed by the lexer.
    **/
    char advance(){
      current++;

      return sourceCode[current - 1];
    }

    /**
     * @brief Based on the latest consumed lexeme, creates its token and inserts it into the sequence of
     * token ('tokens' variable) that is being generated by the lexer.
     * 
     * This method is responsible for creating a new token and inserting into the 'tokens' variable,
     * which is the sequence of tokens that will be generated by the lexer during the process of lexical analysis.
     * Such method creates the new token based on the latest consumed lexeme, along with its literal value, its 
     * token type and the line that such lexeme appears in the source code file.
     * 
     * @param type The type of the token (based on the TokenType enum) that is about to be generated by the 
     * lexer.
     * @param literal The living runtime object related to the lexeme that has been just consumed and its 
     * about to be used to generate the next token by the lexer.
     * 
     * @return Nothing (void).
    **/
    void addToken(TokenType type, std::any literal){
      std::string lexeme{sourceCode.substr(start, current - start)}; // To understand how and why this works, pay attention to the flow of the lexing process.
      tokens.emplace_back(Token(type, std::move(lexeme), std::move(literal), line));

      return;
    }

    /**
     * @brief Indirectly creates a token, whose literal value is guaranteed to be null, and inserting
     * into the sequence of tokens.
     * 
     * This method, based on the latest consumed lexeme (which is guaranteed to be a token whose literal 
     * object is nullptr), is responsible for calling the method that actually creates such token and 
     * inserts it at the end of the 'tokens' variable.
     * 
     * @param type The type of the token (based on the TokenType enum) that is about to be generated by the 
     * lexer.
     * 
     * @return Nothing (void).
    **/
    void addToken(TokenType type){
      addToken(type, nullptr);

      return;
    }

    /**
     * @brief Works like a conditional version of the 'advance' method from this same class.
     * 
     * This method is responsible for working like a conditional 'advance' method. It receives an expected
     * character and checks whether the next unconsumed character by the lexer (the one pointed by the variable
     * 'current') is equal to the expected one. If that's the case, then the character pointed by 'current' is
     * also consumed. Otherwise, it's not consumed.
     * 
     * @param expected A char that represents the next character expected to be consumed by the lexer (the one
     * currently being pointed by the 'current' variable).
     * 
     * @return A boolean that signals whether or not the expected character was consumed by the lexer.
    **/
    bool match(char expected){
      if(isAtEnd()){
        return false;
      }
      if(sourceCode[current] != expected){
        return false;
      }
      current++;

      return true;
    }

    /**
     * @brief Receives an integer that will be stored inside the 'offset' variable and returns a character
     * that has not been consumed by the lexer yet by creating a temporary pointer to an index of the 
     * 'sourceCode' variable by adding the values of 'offset' and 'current'.
     * 
     * This method is responsible for receiving an integer which will be stored inside the 'offset' variable.
     * This value is used to return a character that has not been consumed by the lexer yet, by adding its 
     * value to the value currently stored inside the 'current' variable.
     * 
     * @param offset An integer that is used to create a new temporary pointer to an index of 'sourceCode'
     * by adding its value to the value currently stored inside the 'current' variable.
     * 
     * @return The char that is being stored at 'sourceCode[current + offset]'.
     * 
     * @note If the sum 'current + offset' is equal to (or bigger than) the length of the string stored inside
     * the 'sourceCode' variable, then this method returns the '\0' character.
    **/
    char peek(int offset){
      if(current + offset >= sourceCode.length()){
        return '\0';
      }

      return sourceCode[current + offset];
    }

    /**
     * @brief Receives a char and returns true or false if it's a lowercase letter, an uppercase letter or an
     * underscore.
     * 
     * This method is responsible for receiving a char as its input and simply makes a check to see if its 
     * value is a lowercase letter, an uppercase letter or an underscore.
     * 
     * @param c The character the method is going to check to see if it's a lowercase letter, an uppercase
     * letter or an underscore.
     * 
     * @return A boolean that signals if the received character is a lowercase letter, an uppercase letter or
     * an underscore.
    **/
    bool isAlpha(char c){
      return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_'));
    }

    /**
     * @brief Receives a char and returns true or false if it's a digit between '0' and '9' (inclusive in both
     * sides).
     * 
     * This method is responsible for receiving a char as its input and making a check to see if its value is
     * a digit between '0' and '9' (inclusive in both sides).
     * 
     * @param c The character the method is going to check to see if it's a digit between '0' and '9'
     * (inclusive in both sides).
     * 
     * @return A boolean that signals if the received character is a digit between '0' and '9' (inclusive
     * in both sides).
    **/
    bool isDigit(char c){
      return (c >= '0' && c <= '9');
    }

    /**
     * @brief Receives a char and returns true or false if it's a lowercase letter, an uppercase letter, an
     * underscore or a digit between '0' and '9' (inclusive in both sides).
     * 
     * This method is responsible for receiving a char as its input and making a check to see if its value
     * is a lowercase letter, an uppercase letter, an underscore or a digit between '0' and '9' (inclusive
     * in both sides).
     * 
     * @param c The character the method is going to check to see if it's a lowercase letter, an uppercase
     * letter, an underscore or a digit between '0' and '9' (inclusive in both sides).
     * 
     * @return A boolean that signals if the received character is a lowercase letter, an uppercase letter,
     * an underscore or a digit between '0' and '9' (inclusive in both sides).
    **/
    bool isAlphaNumeric(char c){
      return (isDigit(c) || isAlpha(c) || (c == ':'));
    }

    /**
     * @brief Consumes a multi-line comment. After its execution, the 'current' variable is pointing to the
     * character right after the multi-line comment.
     * 
     * This method is responsible for consuming a multi-line comment that appears inside the 'sourceCode'
     * variable. Since a multi-line comment must be ignored by the lexer, no token is generated after its
     * comsumption.
     * 
     * @return Nothing (void).
    **/
    void multilineComment(){ // Bleach addopts the same rules of C++ when it comes to multi-line comments.
      while(!isAtEnd()){
        if(peek(0) == '\n'){
          line++;
        }else if(peek(0) == '*' && peek(1) == '/'){
          advance();
          advance();
          return;
        }
        advance();
      }

      if(isAtEnd()){
        error(line, "Unterminated multi-line comment");
        return;
      }
      return;
    }

    /**
     * @brief Consumes an identifier, generates its respective token and adds it to the token sequence that is
     * being generated by the lexer. After its execution, the 'current' variable is pointing to the character
     * right after the identifier.
     * 
     * This method is responsible for consuming an identifier that appears inside the 'sourceCode' variable. 
     * After the comsumption is concluded, the respective token of such identifier is generated and then added
     * to the sequence of tokens that is being generated by the lexer. After its execution, the 'current' 
     * variable is pointing to the character right after the identifier.
     * 
     * @return Nothing (void).
    **/
    void identifier(){
      while(isAlphaNumeric(peek(0))){
        advance();
      }

      TokenType type;
      std::string lexeme = std::string{sourceCode.substr(start, current - start)};
      if(keywords.find(lexeme) != keywords.end()){
        type = keywords[lexeme];
      }else if(lexeme.find(":") != std::string::npos && nativeFunctions.find(lexeme) == nativeFunctions.end()){
        error(line, "Cannot use the ':' character if not in a Bleach native function call");
      }
      else{
        type = TokenType::IDENTIFIER;
      }
      addToken(type);

      return;
    }

    /**
     * @brief Consumes a number literal (which can be an integer literal or a floating-point literal), generates
     * its respective token and adds it to the token sequence that is being generated by the lexer. After its
     * execution, the 'current' variable is pointing to the character right after the number literal.
     * 
     * This method is responsible for consuming a number literal (whether it's integer or floating-point) that 
     * appears inside the 'sourceCode' variable. After the comsumption is concluded, the respective token of
     * such number literal is generated and then added to the sequence of tokens that is being generated by 
     * the lexer. After its execution, the 'current' variable is pointing to the character right after the
     * number literal.
     * 
     * @return Nothing (void).
    **/
    void number(){
      while(isDigit(peek(0))){
        advance();
      }

      if(peek(0) == '.' && isDigit(peek(1))){ // We look to check whether the number has a fractional part (this distinguishes an integer literal from a floating-point literal).
        advance(); // Consume the '.' character and continue to pass through the fractional part of the number.
        while(isDigit(peek(0))){ // Passing through the fractional part of the number.
          advance();
        }
      }

      addToken(TokenType::NUMBER, std::stod(std::string{sourceCode.substr(start, current - start)})); // A good example of literal to understand this is '123.'

      return;
    }

    /**
     * @brief Consumes a sring literal, generates its respective token and adds it to the token sequence that
     * is being generated by the lexer. After its execution, the 'current' variable is pointing to the
     * character right after the string literal.
     * 
     * This method is responsible for consuming a string literal that appears inside the 'sourceCode'
     * variable. After the comsumption is concluded, the respective token of such string literal is generated
     * and then added to the sequence of tokens that is being generated by the lexer. After its execution, the
     * 'current' variable is pointing to the character right after the string literal.
     * 
     * @return Nothing (void).
    **/
    void string(){
      while(peek(0) != '"' && !isAtEnd()){
        if(peek(0) == '\n'){ // This is interesting. This is what allows multi-line string in the Bleach language.
          line++;
        }
        advance();
      }

      if(isAtEnd()){
        error(line, "Unterminated string");
        return;
      }

      advance();
      std::string literal{sourceCode.substr(start + 1, current - start - 2)};
      addToken(TokenType::STRING, literal);

      return;
    }

    /**
     * @brief Consumes a lexeme, generates its respective token (if needed) and add such token to the sequence
     * of tokens that is being generated by the lexer.
     * 
     * This method is responsible for consuming a lexeme, generating its respective token (if needed) and
     * adding such token to the sequence of tokens that is generated by the lexer.
     * 
     * @return Nothing (void).
    **/
    void lexToken(){
      char c = advance();
      switch(c){
        case('('):
          addToken(TokenType::LEFT_PAREN);
          break;
        case(')'):
          addToken(TokenType::RIGHT_PAREN);
          break;
        case('['):
          addToken(TokenType::LEFT_BRACKET);
          break;
        case(']'):
          addToken(TokenType::RIGHT_BRACKET);
          break;
        case('{'):
          addToken(TokenType::LEFT_BRACE);
          break;
        case('}'):
          addToken(TokenType::RIGHT_BRACE);
          break;
        case(','):
          addToken(TokenType::COMMA);
          break;
        case('.'):
          addToken(TokenType::DOT);
          break;
        case('?'):
          addToken(TokenType::QUESTION_MARK);
          break;
        case(':'):
          addToken(TokenType::COLON);
          break;
        case(';'):
          addToken(TokenType::SEMICOLON);
          break;
        case('+'):
          addToken(TokenType::PLUS);
          break;
        case('-'):
          if(match('>')){
            addToken(TokenType::ARROW);
          }else{
            addToken(TokenType::MINUS);
          }
          break;
        case('*'):
          addToken(TokenType::STAR);
          break;
        case('/'):
          if(match('/')){ // Single-Line comment.
            while(peek(0) != '\n' && !isAtEnd()) advance();
          }else if(match('*')){ // Multi-Line comment.
            multilineComment();
          }else{ // Division operator.
            addToken(TokenType::SLASH);
          }
          break;
        case('!'):
          addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
          break;
        case('='):
          addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
          break;
        case('>'):
          addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
          break;
        case('<'):
          addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
          break;
        case(' '):
        case('\r'):
        case('\t'):
          break; // The lexer ignore whitespace characters.
        case('\n'):
          line++;
          break;
        case('"'):
          string();
          break;
        default:
          if(isDigit(c)){
            number();
          }else if(isAlpha(c)){
            identifier();
          }else{
            error(line, "Unexpected character not supported by the Bleach language: " + std::string{c});
          }
          break;
      }

      return;
    }

  public:
    /**
     * @brief Constructs a Lexer with the source code (as a string) present inside a Bleach (.bah) file. 
     *
     * This constructor initializes a Lexer object with the given source code of a Bleach file,
     * represented as a string.
     *
     * @param sourceCode The source code of the Bleach file intended to be interpreted. Such source code is 
     * provided as a string.
     *
     * @note The sourceCode parameter is moved into the Lexer object to avoid unnecessary copies.
    **/
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
     * @return The list of tokens (std::vector<Token>) generated by the lexical analysis.
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
