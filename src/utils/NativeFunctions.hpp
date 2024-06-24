#pragma once

#include "./BleachCallable.hpp"

// std::chrono
class NativeClock : public BleachCallable{

};

// std::io
class NativeInput : public BleachCallable{

};

class NativePrint : public BleachCallable{

};

// std::math
class NativeExponentiation : public BleachCallable{

};

class NativeLogarithm : public BleachCallable{

};

class NativeSquareRoot : public BleachCallable{

};