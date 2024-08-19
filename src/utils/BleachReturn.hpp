#pragma once

#include <any>

/**
 * @struct BleachReturn
 * @brief Represents a return statement during runtime (the execution of a Bleach program).
 *
 * The BleachReturn struct is used to represent the behavior of a return statement during runtime. During the
 * execution of a Bleach program, if a return statement is found, then such struct will be thrown as if it was a
 * runtime error. Moreover, it's important to remember that a return statement might have (or might not have) a
 * value. Therefore, the need for an attribute called "value". Such attribute is responsible for storing the
 * value present in a return statement, if any. If that's not the case, then such attribute will store the
 * value nullptr by default.
 */
struct BleachReturn{
  std::any value; // Attribute responsible for storing the value that might be present in a return statement.
};
