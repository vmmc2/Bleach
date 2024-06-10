# Bleach

## Intro
* My undergraduate thesis in Computer Engineering
  * Author: Victor Miguel de Morais Costa
  * Advisor: Leopoldo Motta Teixeira


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
* The language implementation will be one that is clean and simple to understand. Thus, we can focus on the semantics of Bleach, instead of wasting time coding hacks and tricks or "leaving thigs as an exercise for the reader/student".
* By adopting this approach, it's expected that it will make us, students, more comfortable with basic and essential techniques used in the implementation of a programming language. Moreover, this approach will also improve our understanding of how exactly the language is supposed to behave.
* The implementation provided here is written in C++. Contrary to the common claims that C++ is an old and dying language not worthy of one's investment, I'd say that C++ is not only an extremely important language but also that it's not going anywhere soon. Moreover, there are strong reasons for this specific choice, such as:
  * __Educational Value:__  C++ provides a solid foundation in many concepts of Computer Science, such as: memory management, data structures, and low-level programming concepts. These concepts are transferable to many other languages and also deepen one's understanding of programming in general.
  * __Continuous Development:__ Contrary to what many might think, C++ did not "stop in time". The language is in a constant and steady state of evolution in order to meet the changing needs of software development. To address that, the C++ standards committee regularly introduces new features and improvements to it, ensuring that it remains modern and relevant.
  * __Execution Speed:__ While the interpreter itself might not need to execute code as quickly as an application built for real-time processing or high-performance computing, it's still desirable for it to be efficient. This efficiency can manifest in faster parsing, optimization, and execution of the code written in the interpreted language. Since C++ is a high-performance language, it can contribute to faster execution of these tasks, resulting in quicker interpretation of code.
  * __Memory Management:__ Compilers and interpreters often deal with large amounts of data, including source code, abstract syntax trees, intermediate representations, and compiled code. Efficient memory management is crucial to handle this data without excessive memory consumption or leaks. C++ offers manual memory management capabilities, along with smart pointers and memory management libraries, allowing developers to fine-tune memory usage for optimal performance.
  * __Familiarity:__
  * __Industry Adoption:__ This reason is specially aimed to those that, like me, found themselves in this area of Computer Science. Nowadays, C++ is the 'lingua franca' when it comes to the implementation of compilers, interpreters and runtimes. Some examples to highlight this are:
    * __Clang:__ C, C++, and Objective-C compiler front-end for the LLVM compiler infrastructure. Known for its fast compile times and excelent error diagnostics.
    * __Chromium V8:__ Google's open-source JavaScript runtime engine, used in the Chrome browser and Node.js.
    * __Emscripten:__ An LLVM-to-WebAssembly Compiler
    * __glow:__ A compiler for Neural Network hardware accelerators
    * __g++:__ The C++ front-end of the GCC compiler, also written in C++.
    * __ICC (Intel C++ Compiler):__ This one, often used for optimizing performance on Intel architectures, is implemented majorly in C++.
    * __LLVM (Low Level Virtual Machine):__ A compiler infrastructure project that includes a collection of modular and reusable compiler and toolchain components. While LLVM itself is written in C++, it provides support for multiple programming languages.
    * __MSVC (Microsoft Visual C++ Compiler):__ A part of the famous Visual Studio IDE.
    * __Swift Compiler:__ A compiler to the famous programming language developed by Apple, is written in C++.


## Bleach Language Documentation and Bleach Tree-Walk Interpreter Documentation
* Both documentations can be found here (Insert a link later on).


## Roadmap
- [x] Chapter 4: Scanning (Review Implementation)
  - [x] Add support Multi-line comments (```/* */```)
- [x] Chapter 5: Representing Code (Review Implementation)
- [x] Chapter 6: Parsing Expressions (Review Implementation)
- [x] Chapter 7: Evaluating Expressions (Review Implementation)
  - [ ] Add support to the ternary Operator (```expression ? value1 : value2```)
  - [ ] Make comparison and equality operators more robust with respect to its operands (allow them to be any type of operand).
- [x] Chapter 8: Statements and State (Review Implementation)
- [ ] Chapter 9: Control Flow (Review Implementation)
  - [ ] Add support to the ```elif``` keyword available along with ```if``` and ```else``` keywords
  - [ ] Add support to ```do-while``` loop
  - [ ] Add ```break``` and ```continue``` keywords make them usable only inside loops
  - [ ] Make the ```str``` type more robuts (immutability like python, not allow assignment, which methods to include)
  - [ ] Add the ```list``` type (which methods to include and how to traverse values of such type)
  - [ ] Add the ```dict``` type (which methods to include and how to traverse values of such type)
  - [ ] Make if-elif-else statements and for-dowhile-while loops have their own environment/scope
- [ ] Chapter 10: Functions (Review Implementation)
  - [ ] Add support to the ```lambda``` keyword, which allow the creation of anonymous functions
  - [ ] Add support for some built-in/native functions (```std::chrono```, ```std::io```, ```std::math```, ```std::utils```)
- [ ] Chapter 11: Resolving and Binding (Review Implementation)
- [ ] Chapter 12: Classes (Review Implementation)
- [ ] Chapter 13: Inheritance (Review Implementation)
  - [ ] Add support to the definition of static methods inside a class.
  - [ ] Add support to multiple inheritance (is it worth it? Take a look at how Python implements it)


## Setup
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
