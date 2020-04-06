#include "TransformationFunction.hpp"

double* Jimmy::TransformationFunction::run(double x){
    this->result = this->foo(x);
    this->derivativeResult = this->derivativeFoo(x,this->result);
    
    double ans[2] = {this->result,this->derivativeResult};
    return ans;
}

Jimmy::TransformationFunction::TransformationFunction(double (*function)(double), double (*derFoo)(double, double)){
    this->foo = function;
    this->derivativeFoo = derFoo; 
}

Jimmy::TransformationFunction::TransformationFunction(const TransformationFunction& transFoo){
    *this = transFoo; 
}

void Jimmy::TransformationFunction::operator=(const TransformationFunction& transFoo){
    this->foo = transFoo.foo;
    this->derivativeFoo = transFoo.derivativeFoo;
}