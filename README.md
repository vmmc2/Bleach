# Bleach
* A programming language aimed at teaching compilers. 


## Intro
* __The implementation of my undergraduate thesis to obtain my Bachelor's degree in Computer Engineering: "Bleach: A programming language aimed for teaching introductory 'Compilers' courses at universities."__ 
  * __Author/Student: Victor Miguel de Morais Costa__
  * __Advisor: Leopoldo Motta Teixeira__


## What is Bleach?
* Bleach is programming language whose main purpose is help students learn and practice concepts from an introductory "Compilers" or "Programming Language Implementation" course in a way that feels more interesting and rewarding to the students. By using Bleach, the students will be able to learn the foundational ideas of compilers, programming language design and programming language implementation, such as:
  01. Scanning (Lexing)
  02. Code Representation
  03. Parsing
  04. Expression Evaluation
  05. Statements and State
  06. Control Flow Structures
  07. Functions
  08. Resolving and Binding
  09. Classes
  10. Inheritance


## Why use Bleach?
* As said previously, Bleach is a programming language whose focus is on concepts. This means that the language will have features (mentioned above) that make it minimally interesting.
* The language implementation is made with the purpose of being clean and simple to understand. Thus, we can focus on the semantics of Bleach, instead of wasting time coding hacks and tricks or "leaving thigs as an exercise for the reader/student".
* By adopting this approach, it's expected that it will make us, students, more comfortable with basic and essential techniques used in the implementation of a programming language. Moreover, this approach will also improve our understanding of how exactly the language is supposed to behave.


## Examples of simple yet useful programs written in Bleach
1. Hello, World!
```ts
function greet(){
  print "Hello, World!"; // "Hello, World!"
  std::io::print("Hello, World!"); // "Hello, World!"
}

greet();
```

2. Fatorial
```ts
function fat(n){
  if(n == 0){
    return 1;
  }

  return n * fat(n - 1);
}

std::io::print(fat(5)); // 120
```

3. Fibonacci
```ts

```

4. Basic Arithmetic Operations
```ts
let a = 5;
let b = 6;

let sum = a + b;
let difference = a - b;
let product = a * b;
let quotient = a / b;

std::io::print("Sum: ", sum) // 11
std::io::print("Difference: ", difference) // -1
std::io::print("Product: ", product) // 30
std::io::print("Quotient: ", quotient) // 0.833333333333333
```

5. Logical Control Flow
```ts
let number = 42;

if(number > 0){
  std::io::print("Positive Number."); // "Positive Number."
}elif(number < 0){
  std::io::print("Negative Number.");
}else{
  std::io::print("Zero.");
}
```

6. Loop Control Flow I
```ts
let count = 5;

while(count > 0){
    std::io::print("Countdown: ", count);
    count = count - 1;
}
std::io::print("BOOM!"); // "BOOM!"
```

7. Loop Control Flow II

8. Loop Control Flow III

9. Get User Input

10. String Manipulation

11. Class and Instances Usage

12. Linked-List Implementation


## The undergraduate thesis
* If you are interested in checking out what motivated me to do this project, the thought process to make the project decisions during the whole thing, then you are more than welcomed to take a look at: __Insert link later on.__


## Bleach Language Documentation
* You can read the official documentation of the Bleach language here: [Bleach Official Documentation](vmmc2.github.io/Bleach-Docs/)


## Bleach Language Syntax Highlight Visual Studio Code Extension
* You can install the official Bleach Syntax Highlight extension for Visual Studio Code here: [Bleach Official Syntax Highlight Extension for VS Code](https://marketplace.visualstudio.com/items?itemName=VictorMigueldeMoraisCosta.bleach-language-syntax-highlight)
* __Remember that a Bleach file must have the ```.bch``` extension. Otherwise, the extension won't work properly.__


## How to use it?
1. Clone this repository in your local machine.
2. Go to the ```Bleach``` root directory. Then, execute the following commands inside it at the console/terminal:
```sh
cd scripts
chmod +x bleach.sh
```
3. Execute the script that starts up the Bleach Tree-Walk Interpreter:
```sh
./bleach.sh # Executes the interpreter in the interactive mode (REPL mode).
./bleach.sh absolute_or_relative_path_to_a_bch_file # Executes the interpreter with the code written inside a Bleach file (".bch" extension).
```


## How to clean the built Bleach Tree-Walk Interpreter?
1. Go to the ```Bleach``` root directory. Then, execute the following commands inside it at the console/terminal:
```sh
cd scripts
chmod +x bleach_clean.sh
```
2. Execute the script that cleans the build of the Bleach Tree-Walk Interpreter (if any is present):
```sh
./bleach_clean.sh
```


## How to run the unit tests of the Bleach Tree-Walk Interpreter?
1. Go to the ```Bleach``` root directory. Then, execute the following commands inside it at the console/terminal:
```sh
cd scripts
chmod +x bleach_test_pipeline.sh
```
3. Execute the script that run all of the unit tests related to the Bleach Tree-Walk Interpreter:
```sh
./bleach_test_pipeline.sh
```


## To Do
- [ ] Add the remaining unit tests that cover every single AST node of the Bleach language.
- [ ] Add support for the remainder operator (```%```).
- [ ] Implement the remaining native functions from the ```std::io``` and ````std::math``` namespaces.
- [ ] Verify Bleach's performance by using the "The Computer Languages Benchmark Game".


## Future Steps
- [ ] Make the ```str``` type more robust.
  - [ ] Think about which methods to include in this type.
  - [ ] Think about how to index a value of string type. 
- [ ] Add the ```list``` type.
  - [ ] Think about how to lex/scan a ```list``` literal.
  - [ ] Think about how to parse a ```list``` literal (AST node).
  - [ ] Think about how the resolver should deal with a ```list``` literal.
  - [ ] Think about how this type should be represented during runtime. Think about what methods this new type should have and how each of them should behave. Think about how one could iterate through elements of a ```list``` value. Think about how the sorting of a ```list``` value should work.


## Current State of the Bleach Language Context-Free Grammar
* __Now loops (```for```, ```do-while```, ```while```) must be followed by a block.__
```txt
program → statement* EOF
statement → block | breakStmt | classDeclStmt | continueStmt | doWhileStmt | exprStmt | forStmt | funcDeclStmt | ifStmt | printStmt | returnStmt | varDeclStmt | whileStmt
block → "{" statement* "}"
breakStmt → "break" ";"
classDeclStmt → "class" IDENTIFIER ( "inherits" IDENTIFIER )? "{" methodDeclStmt* "}"
methodDeclStmt → "method" method
method → IDENTIFIER "(" parameters? ")" block
continueStmt → "continue" ";"
doWhileStmt → "do" block "while" "(" expression ")" ";"
exprStmt → expression ";"
forStmt → "for" "(" ( varDecl | exprStmt | ";" ) expression? ";" expression? ")" block
funcDeclStmt → "function" function
function → IDENTIFIER "(" parameters? ")" block
parameters → IDENTIFIER ( "," IDENTIFIER )*
ifStmt → "if" "(" expression ")" statement
         ( "elif" "(" expression ")" statement )*
         ( "else" statement )?
printStmt → "print" expression ";"
returnStmt → "return" expression? ";"
varDeclStmt → "let" IDENTIFIER ( "=" expression )? ";"
whileStmt → "while" "(" expression ")" block
expression → assignment
assignment → ( call "." )? IDENTIFIER "=" assignment | ternary
ternary → logic_or ( "?" expression ":" expression )*
logic_or → logic_and ( "or" logic_and )*
logic_and → equality ( "and" equality )*
equality → comparison ( ( "!=" | "==" ) comparison )*
comparison → term ( ( ">" | ">=" | "<" | "<=" ) term )*
term → factor ( ( "-" | "+" ) factor )*
factor → unary ( ( "/" | "*" ) unary )*
unary → ( "!" | "-" ) unary | call
call → primary ( "(" arguments? ")" | "." IDENTIFIER )*
arguments → expression ( "," expression )*
primary → "true" | "false" | "nil" | NUMBER | STRING | "(" expression ")" | lambdaFunctionExpr | IDENTIFIER | "super" . IDENTIFIER
lambdaFunctionExpr → "lambda" "->" "(" parameters? ")" block
```


## Why was Bleach written in C++?
* The implementation of the Bleach Interpreter was written in C++ due to a couple of reasons which include, but are not restricted to:
  * __Educational Value:__  C++ provides a solid foundation in many concepts of Computer Science, such as: memory management, data structures, and low-level programming concepts. These concepts are transferable to many other languages and also deepen one's understanding of programming in general.
  * __Execution Speed:__ While the interpreter itself might not need to execute code as quickly as an application built for real-time processing or high-performance computing, it's still desirable for it to be efficient. This efficiency can manifest in faster parsing, optimization, and execution of the code written in the interpreted language. Since C++ is a high-performance language, it can contribute to faster execution of these tasks, resulting in quicker interpretation of code.
  * __Memory Management:__ Compilers and interpreters often deal with large amounts of data, including source code, abstract syntax trees, intermediate representations, and compiled code. Efficient memory management is crucial to handle this data without excessive memory consumption or leaks. C++ offers manual memory management capabilities, along with smart pointers and memory management libraries, allowing developers to fine-tune memory usage for optimal performance.
  * __Familiarity:__ I could use another language like Haskell, OCaml, Rust of Python. However, in my point of view, implementing Bleach in C++ is a better idea because most students have already some previous contact with it in their Algorithm & Data Structures course or in their Operating Systems course. Also, the performance of an implementation is an important aspect, so it makes sense for us to use this language.
  * __Industry Adoption:__ This reason is specially aimed to those that, like me, want to work in this field of Computer Science. Nowadays, C++ is the 'lingua franca' in it. Some examples to highlight this are:
    * __Clang:__ C, C++, and Objective-C compiler front-end for the LLVM compiler infrastructure. Known for its fast compile times and excelent error diagnostics.
    * __Chromium V8:__ Google's open-source JavaScript runtime engine, used in the Chrome browser and Node.js.
    * __g++:__ The C++ front-end of the GCC compiler, also written in C++.
    * __LLVM (Low Level Virtual Machine):__ A compiler infrastructure project that includes a collection of modular and reusable compiler and toolchain components. While LLVM itself is written in C++, it provides support for multiple programming languages.
    * __MSVC (Microsoft Visual C++ Compiler):__ A part of the famous Visual Studio IDE.


<!--
## LLVM Version of the Bleach Language (Not priority right now)
### Initial Setup
1. On the root folder of your machine:
```shell
git clone git@github.com:llvm/llvm-project.git
```
2. Then run the following commands:
```shell
cd llvm-project
mkdir build
cd build
```
3. Run the following build command: 
```shell
cmake ../llvm -G "Unix Makefiles" -DCMAKE_INSTALL_PREFIX=~/llvm-project/build -DBUILD_SHARED_LIBS=on -DLLVM_ENABLE_PROJECTS=clang -DCMAKE_BUILD_TYPE=Release
```
4. Finally:
```shell
make
```
-->

<!--
## Doubts
* __What is the diference between using a variable and refering to a variable?__
* __Is it really okay to let a variable be re-declared in the global scope? Not convinced by the author.__
* __Differences between scopes and environments (the author says they are "close cousins"). Also, the author later mentions that scope is a concept/idea and environment is something concrete that implements such concept.__
* __What is necessary to claim that a programming language is Turing-Complete? In other words, what exactly is Turing-Completeness? According to the author, the programming language needs to have the following features implemented:__
  * __Arithmetic__
  * __A little control-flow (what does "little" even mean in this context?)__
  * __The ability to allocate arbitrary amounts of memory__
* __What is the formal and precise definition of scope in the programming language field? The author, Bob Nystrom, says, in chapter 11, that scope is a set of declarations. Therefore, if two sets don't have the same declarations, then they are not the same scope. Is this correct?__
* When dealing with "Get" expression, the author says the following: "Since properties are looked up dynamically, they don’t get resolved". What does this mean?
-->
