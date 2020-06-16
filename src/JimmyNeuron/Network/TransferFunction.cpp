#include "TransferFunction.hpp"

double Jimmy::TransferFunction::run(const double& neuronOutput){
    return this->foo(neuronOutput);
}
double Jimmy::TransferFunction::runDervative(const double& neuronResult){
    return this->derivativeFoo(neuronResult);
}

Jimmy::TransferFunction::TransferFunction(double (*function)(const double&), double (*derFoo)(const double&)){
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