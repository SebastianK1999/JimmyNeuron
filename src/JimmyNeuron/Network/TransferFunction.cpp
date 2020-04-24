#include "TransferFunction.hpp"

void Jimmy::TransferFunction::run(double neuronOutput){
    this->result = this->foo(neuronOutput);
    this->derivativeResult = this->derivativeFoo(neuronOutput,this->result);
}

Jimmy::TransferFunction::TransferFunction(double (*function)(double), double (*derFoo)(double, double)){
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