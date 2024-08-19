#pragma once


/**
 * @struct BleachBreak
 * @brief Represents a break statement during runtime (the execution of a Bleach program).
 *
 * The BleachBreak struct is used to represent the behavior of a break statement during runtime. During the
 * execution of a Bleach program, if a break statement is found, then such struct will be thrown as if it was a
 * runtime error. Out of a loop structure, the Bleach Interpreter tries to catch this type of "runtime error"
 * in order to signal that a loop has finished its execution due to a break statement.
 */
struct BleachBreak{};
