# Bleach

## Intro
* My undergraduate thesis in Computer Engineering
  * Author: Victor Miguel de Morais Costa
  * Advisor: Leopoldo Motta Teixeira

## What is Bleach?

## Why use Bleach?

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
cmake ../llvm -G "Unix Makefiles" -DCMAKE_INSTALL_PREFIX=~/llvm-project/build -DBUILD_SHARED_LIBS=on -DLLVM_ENABLE_PROJECTS=clang -DCMAKE_BUILD_TYPE=Debug
```
4. Finally:
```shell
make
```
