#include "TransferFunction.hpp"

double Jimmy::TransferFunction::run(double neuronOutput){
    return this->foo(neuronOutput);
}
double Jimmy::TransferFunction::runDervative(double neuronOutput, double neuronResult){
    return this->derivativeFoo(neuronOutput,neuronResult);
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