#include "TransferFunction.hpp"

float_t Jimmy::TransferFunction::run(const float_t& neuronOutput){
    return this->foo(neuronOutput);
}
float_t Jimmy::TransferFunction::runDervative(const float_t& neuronResult){
    return this->derivativeFoo(neuronResult);
}

Jimmy::TransferFunction::TransferFunction(float_t (*function)(const float_t&), float_t (*derFoo)(const float_t&)){
    this->foo = function;
    this->derivativeFoo = derFoo; 
}

Jimmy::TransferFunction::TransferFunction(const TransferFunction& transFoo){
    *this = transFoo; 
}

void Jimmy::TransferFunction::operator=(const TransferFunction& transFoo){
    this->foo = transFoo.foo;
    this->derivativeFoo = transFoo.derivativeFoo;
}