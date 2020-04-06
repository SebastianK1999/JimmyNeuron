#ifndef METHODS_INCLUDE
#define METHODS_INCLUDE

#include "TransformationFunction.hpp"
#include <cmath>

namespace Jimmy{
    class Methods{
    public:
        static const TransformationFunction tanh;
        static const TransformationFunction sigmoid;

    private:
        static double TANH(double);
        static double DERTANH(double, double);
        
        static double SIGMOID(double);
        static double DERSIGMOID(double, double);
    };
}

#endif