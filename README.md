# Bleach

## Intro
* __The implementation of my undergraduate thesis to obtain my Bachelor's degree in Computer Engineering at Universidade Federal de Pernambuco (UFPE), located in Brazil: "Bleach: A programming language aimed for teaching Compilers."__ 
  * __Author/Student: Victor Miguel de Morais Costa__
  * __Advisor: Leopoldo Teixeira__


## Paper
* This repository is the reference implementation (research artifact) accompanying the paper __"Bleach: A Programming Language for Teaching Compilers"__, submitted to SBLP 2026 (30th Brazilian Symposium on Programming Languages), part of CBSoft 2026.
* Paper link: **TODO — add the link to the camera-ready/published paper (SBLP 2026 proceedings or a preprint) once it is available.**

## Repository Structure
```
Bleach/
├─ src/                              # C++17 interpreter source (single translation unit, main.cpp)
│  ├─ lexer/                         # Scanner
│  ├─ parser/                        # Recursive-descent parser
│  ├─ resolver/                      # Static resolver (variable binding, error checks)
│  ├─ interpreter/                   # Tree-walk evaluator and runtime
│  ├─ error/                         # Error types
│  ├─ utils/                         # Shared runtime data structures
│  └─ Makefile                       # Builds the `BleachInterpreter` executable
├─ scripts/                          # Build/run/clean/test entry points (see Installation)
├─ tests/                            # 44-case test suite + 11 algorithm/data-structure programs
│  ├─ valid_bleach_programs/         # .bch test inputs (expressions, native_functions, statements)
│  ├─ valid_bleach_programs_output/  # Expected stdout for each test input
│  └─ algorithms_and_data_structures/ # Larger .bch programs (sorting, search, graph algorithms, etc.)
├─ Dockerfile                        # Reproducible build/run environment (see Installation)
├─ LICENSE                           # MIT License
└─ README.md                         # This file
```


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

## Requirements
Bleach can be used either through Docker (no local toolchain needed) or by building it natively.

* __Hardware:__ No special hardware. The interpreter is a single-threaded tree-walk interpreter compiled from one translation unit; any machine able to run a C++ compiler and hold well under 1&nbsp;GB of RAM and 100&nbsp;MB of disk space is sufficient.
* __Operating system:__ Linux and macOS are supported natively. On Windows, use Docker or WSL2 — the build/run scripts are Bash scripts and rely on a POSIX shell.
* __Software (native build):__
  * A C++17-capable compiler exposed as `g++` (GCC 9 or newer; the Docker image pins GCC 13).
  * `make` (GNU Make).
  * `bash`.
  * No external libraries: the interpreter has zero third-party dependencies (C++ standard library only), so there is no `requirements.txt`/package manifest to install.
* __Software (Docker route):__ Docker Engine 20.10 or newer. This is the recommended, most reproducible route since it pins the exact compiler version (`gcc:13-bookworm`) used to validate this artifact.


## Installation
Both routes below end with the exact same `BleachInterpreter` executable; use whichever is more convenient.

### Option A — Docker (recommended)
1. Clone this repository and move into it:
```sh
git clone https://github.com/vmmc2/Bleach.git
cd Bleach
```
2. Build the image (this compiles the interpreter as part of the image build):
```sh
docker build -t bleach-interpreter .
```
3. Run a container. It starts an interactive shell already positioned inside `scripts/`, with a working `BleachInterpreter` build, so the scripts below can be invoked directly:
```sh
docker run -it bleach-interpreter
```
```sh
# (1) Rebuild the interpreter (already built once during `docker build`):
./bleach_build.sh

# (2) Run the interpreter:
./bleach_run.sh                                   # Interactive REPL mode.
./bleach_run.sh relative_or_absolute_path_to.bch   # Execute a Bleach file (".bch" extension).

# (3) Remove the build artifacts:
./bleach_clean.sh

# (4) Run the full test suite:
./bleach_test_pipeline.sh
```
4. To run a `.bch` file that lives on your host machine, mount it into the container, e.g.:
```sh
docker run -it -v "$(pwd)/my_program.bch:/app/scripts/my_program.bch" bleach-interpreter ./bleach_run.sh my_program.bch
```

### Option B — Build from source
1. Clone this repository and move into its `scripts` directory:
```sh
git clone https://github.com/vmmc2/Bleach.git
cd Bleach/scripts
chmod +x *.sh
```
2. Build the interpreter:
```sh
./bleach_build.sh
```
3. Run it:
```sh
./bleach_run.sh                                   # Interactive REPL mode.
./bleach_run.sh relative_or_absolute_path_to.bch   # Execute a Bleach file (".bch" extension).
```
4. Remove build artifacts, when needed:
```sh
./bleach_clean.sh
```

### Verifying the installation
Run the automated test suite (works identically under Docker or a native build; from the `scripts/` directory):
```sh
./bleach_test_pipeline.sh
```
This builds the interpreter and runs the 44 test cases discussed in the paper (`tests/valid_bleach_programs/{expressions,native_functions,statements}`), diffing actual output against the `.expected` files under `tests/valid_bleach_programs_output/`. A correctly installed interpreter ends with the following summary:
```
Bleach Test Suite Execution Summary
Total valid tests: 44
Passed valid tests: 44
Failed valid tests: 0
```
As a minimal smoke test, you can also run the REPL directly and evaluate a single expression:
```sh
./bleach_run.sh
> print "Hello, World!";
Hello, World!
```


## The undergraduate thesis
* If you are interested in checking out what motivated me to do this project, the thought process to make the project decisions during the whole thing, then you are more than welcomed to take a look at the full [Bleach Undergraduate Thesis Repo](https://github.com/vmmc2/Bleach-Thesis)


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
primary → "true" | "false" | "nil" | "self" | NUMBER | STRING | "(" expression ")" | "[" (expression ( "," expression )*)? "]" | lambdaFunctionExpr | IDENTIFIER | "super" . IDENTIFIER
lambdaFunctionExpr → "lambda" "->" "(" parameters? ")" block
```

## License
* This project is licensed under the MIT License — see [LICENSE](./LICENSE) for the full text.