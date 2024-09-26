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

## Who is the target audience of this project?
* The main audience are professors, instructors and teaching assistants that are responsible for lecturing any undergraduate level 'Compilers' course.
* Hobbists and people interested in the Compilers, Interpreters or Programming Languages.
* __A note for the main audience: Please note that even if this interpreter for the Bleach language was written in C++, it does not limit any professor or instructor to choose another language (whether it's C, Haskell, Ocaml, Python, Racket, Scheme, SML or any other) to make assignments for their students so they can make their own implementation of Bleach. Feel free and encouraged to do so.__


## Why use Bleach?
* As said previously, Bleach is a programming language whose focus is on concepts. This means that the language will have features (mentioned above) that make it interesting for the students.
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

2. Factorial
```ts
function factorial(n){
  if(n == 0){
    return 1;
  }

  return n * factorial(n - 1);
}

std::io::print(factorial(5)); // 120
```

3. Fibonacci
```ts
function fibonacci(n) {
  if(n <= 0){
    return 0;
  }elif(n == 1){
    return 1;
  }else{
    return fibonacci(n - 1) + fibonacci(n - 2);
  }
}

let n = 10;
let result = fibonacci(n);
std::io::print("The", n, "th Fibonacci number is:", result);
```

4. Basic Arithmetic Operations
```ts
let a = 5;
let b = 6;

let sum = a + b;
let difference = a - b;
let product = a * b;
let quotient = a / b;
let remainder = a % b;

std::io::print("Sum:", sum); // 11
std::io::print("Difference:", difference); // -1
std::io::print("Product:", product); // 30
std::io::print("Quotient:", quotient); // 0.833333333333333
std::io::print("Remainder:", remainder); // 5
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
    std::io::print("Countdown:", count);
    count = count - 1;
}
std::io::print("BOOM!"); // "BOOM!"
```

7. Loop Control Flow II
```ts
let counter = 1;

do{
  std::io::print("Counter is:", counter);
  counter = counter + 1;
}while(count <= 5);
```

8. Loop Control Flow III
```ts
for(let i = 1; i <= 5; i = i + 1){
    std::io::print("Current Iteration:", i);
}

std::io::print("For loop execution has ended!");
```

9. Get User Input
```ts
std::io::print("Enter your name:");

let name = std::io::readLine();

let greeting = "Hello, " + name + "!";

std::io::print(greeting);
```

10. String Manipulation
```ts
let first_name = "John";
let last_name = "Doe";

let full_name = first_name + " " + last_name;
std::io::print("The full name of the person is:", full_name);
```

11. Class, Inheritance and Instances Usage
```ts
// Base class
class Shape {
  method init(name){
    self.name = name;
  }

  method area(){
    // To be overridden by subclasses
    return 0;
  }

  method describe() {
    return "This is a " + self.name;
  }
}

// Derived class: Circle
class Circle inherits Shape {
  method init(radius){
    // Call the base class constructor
    super.init("Circle");
    self.radius = radius;
  }

  method area(){
    return std::math::pow(self.radius, 2) * 3.14159;
  }
}

// Derived class: Rectangle
class Rectangle inherits Shape {
  method init(width, height){
    // Call the base class constructor
    super.init("Rectangle");
    self.width = width;
    self.height = height;
  }

  method area(){
    return self.width * self.height;
  }
}

// Derived class: Triangle
class Triangle inherits Shape {
  method init(base, height) {
    // Call the base class constructor
    super.init("Triangle");
    self.base = base;
    self.height = height;
  }

  method area(){
    return (self.base * self.height) / 2;
  }
}

// Create instances of each shape
let circle = Circle(5);
let rectangle = Rectangle(4, 6);
let triangle = Triangle(3, 7);

// Print descriptions and areas.
std::io::print(circle.describe(), "with area:", circle.area());
std::io::print(rectangle.describe(), "with area:", rectangle.area());
std::io::print(triangle.describe(), "with area:", triangle.area());
```


## Bleach Language Documentation
* You can read the official documentation of the Bleach language here: [Bleach Official Documentation](https://vmmc2.github.io/Bleach-Docs/)


## Bleach Language Syntax Highlight Visual Studio Code Extension
* You can install the official Bleach Syntax Highlight extension for Visual Studio Code here: [Bleach Official Syntax Highlight Extension for VS Code](https://marketplace.visualstudio.com/items?itemName=VictorMigueldeMoraisCosta.bleach-language-syntax-highlight)
* __Remember that a Bleach file must have the ```.bch``` extension. Otherwise, the extension won't work properly.__


## How to build the Bleach Tree-Walk Interpreter?
1. Clone this repository in your local machine.
2. Go to the ```Bleach``` root directory. Then, execute the following commands inside it at the console/terminal:
```sh
cd scripts
chmod +x bleach_build.sh
```
3. Execute the script that builds the Bleach Tree-Walk Interpreter:
```sh
./bleach_build.sh
```


## How to run the Bleach Tree-Walk Interpreter?
1. Execute the script that starts up the Bleach Tree-Walk Interpreter:
```sh
./bleach_run.sh # Executes the interpreter in the interactive mode (REPL mode).
./bleach_run.sh absolute_or_relative_path_to_a_bch_file # Executes the interpreter with the code written inside a Bleach file (".bch" extension).
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
  - Expression Nodes
    - [x] Assign
    - [x] Binary
    - [ ] Call
    - [ ] Get
    - [ ] Grouping
    - [ ] LambdaFunction
    - [ ] ListLiteral
    - [ ] Literal
    - [ ] Logical
    - [ ] Self
    - [ ] Set
    - [ ] Super
    - [ ] Ternary
    - [x] Unary
    - [ ] Variable
  - Statements Nodes
    - [x] Block
    - [ ] Break
    - [ ] Class
    - [ ] Continue
    - [ ] DoWhile
    - [ ] Expression
    - [ ] For
    - [ ] Function
    - [x] If
    - [x] Print
    - [ ] Return
    - [x] Var
    - [ ] While
- [ ] Verify Bleach's performance by using the "The Computer Languages Benchmark Game".


## The undergraduate thesis
* If you are interested in checking out what motivated me to do this project, the thought process to make the project decisions during the whole thing, then you are more than welcomed to take a look at: __Insert link later on.__


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
factor → unary ( ( "/" | "*" | "%" ) unary )*
unary → ( "!" | "-" ) unary | call
call → primary ( "(" arguments? ")" | "." IDENTIFIER )*
arguments → expression ( "," expression )*
primary → "true" | "false" | "nil" | NUMBER | STRING | "(" expression ")" | "[" (expression ( "," expression )*)? "]" | lambdaFunctionExpr | IDENTIFIER | "super" . IDENTIFIER
lambdaFunctionExpr → "lambda" "->" "(" parameters? ")" block
```


## Why was Bleach written in C++?
* The implementation of the Bleach Interpreter was written in C++ due to a couple of reasons which include, but are not restricted to:
  * __Educational Value:__  C++ provides a solid foundation in many concepts of Computer Science, such as: memory management, data structures, and low-level programming concepts. These concepts are transferable to many other languages and also deepen one's understanding of programming in general.
  * __Execution Speed:__ While the interpreter itself might not need to execute code as quickly as an application built for real-time processing or high-performance computing, it's still desirable for it to be efficient. This efficiency can manifest in faster parsing, optimization, and execution of the code written in the interpreted language. Since C++ is a high-performance language, it can contribute to faster execution of these tasks, resulting in quicker interpretation of code.
  * __Familiarity:__ One could argue that another language like Haskell, OCaml, Racket, Scheme or SML is more suitable for this task. However I don't consider myself proficient enough in such language to tackle a project of this scope. Moreover, in my point of view, implementing Bleach in C++ is a good idea because most students have already worked with either in their "Introduction to Programming", "Algorithms & Data Structures" or "Operating Systems" course. Thus, they don't need to learn a new language just to understand the code written here. Also, the performance of an implementation is an important aspect, so it makes sense for, me at least, to use C++.
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
