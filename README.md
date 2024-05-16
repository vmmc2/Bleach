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
* The language implementation will be one that is of clean and simple understanding. Thus, we can focus on the semantics of Bleach, instead of wasting time coding hacks and tricks or "leaving thigs as an exercise for the student".
* By adopting this approach, it's expected that it will make us, students, more comfortable with basic and essential techniques used in the implementation of a programming language. Moreover, this approach will also improve our understanding of how exactly the language is supposed to behave.
* The implementation provided here is written is C++.


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
