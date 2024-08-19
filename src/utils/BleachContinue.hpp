#pragma once

/**
 * @struct BleachContinue
 * @brief Represents a continue statement during runtime (the execution of a Bleach program).
 *
 * The BleachContinue struct is used to represent the behavior of a continue statement during runtime. During 
 * the execution of a Bleach program, if a continue statement is found, then such struct will be thrown as if it 
 * was a runtime error. Inside a loop structure, the Bleach Interpreter tries to catch this type of "runtime 
 * error" in order to signal that a loop has stopped the execution of its body abruptly and needs to immediately
 * go to its next iteration.
 */
struct BleachContinue{};
