#include "LossFunction.hpp"

float_t Jimmy::LossFunction::run(const std::vector<float_t>& realValues, const std::vector<std::reference_wrapper<Jimmy::NEURON>>& outputLayer){
    this->error = this->foo(realValues, outputLayer);
    return this->error;
}
float_t Jimmy::LossFunction::getError()const{
    return this->error;
}

Jimmy::LossFunction::LossFunction(float_t (*bar)(const std::vector<float_t>&, const std::vector<std::reference_wrapper<Jimmy::NEURON>>&)){
    this->foo = bar;
}
Jimmy::LossFunction::LossFunction(const Jimmy::LossFunction& lossFunc){
    this->foo = lossFunc.foo;
}
void Jimmy::LossFunction::operator=(const Jimmy::LossFunction& lossFunc){
    this->foo = lossFunc.foo;
}