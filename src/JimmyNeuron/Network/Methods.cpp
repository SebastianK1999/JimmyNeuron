#include "Methods.hpp"

double Jimmy::Methods::TANH(double x){
    return std::tanh(x);
}
double Jimmy::Methods::DERTANH(double x, double r){
    return 1 - r*r;
} 
const Jimmy::TransformationFunction Jimmy::Methods::tanh = TransformationFunction(Methods::TANH,Methods::DERTANH);


double Jimmy::Methods::SIGMOID(double x){
    return 1/(1+std::exp(-x));
}
double Jimmy::Methods::DERSIGMOID(double x, double r){
    return r*(1-r);
}
const Jimmy::TransformationFunction Jimmy::Methods::sigmoid = TransformationFunction(Methods::SIGMOID,Methods::DERSIGMOID);