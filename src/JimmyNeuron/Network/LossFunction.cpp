#include "LossFunction.hpp"

double Jimmy::LossFunction::run(const std::vector<double>& realValues, const Jimmy::LAYER& outputLayer){
    this->error = this->foo(realValues, outputLayer);
    return this->error;
}
double Jimmy::LossFunction::getError()const{
    return this->error;
}

Jimmy::LossFunction::LossFunction(double (*bar)(const std::vector<double>&, const Jimmy::LAYER&)){
    this->foo = bar;
}
Jimmy::LossFunction::LossFunction(const Jimmy::LossFunction& lossFunc){
    this->foo = lossFunc.foo;
}
void Jimmy::LossFunction::operator=(const Jimmy::LossFunction& lossFunc){
    this->foo = lossFunc.foo;
}