#ifndef METHODS_INCLUDE
#define METHODS_INCLUDE

#include "TransferFunction.hpp"
#include "LossFunction.hpp"
#include "Layer.hpp"
#include <vector>
#include <cmath>
#include <iostream>

namespace Jimmy{
    class Methods{
    public:
        class transFuncs{ // contains all build-in Transfer functions
        public:
            static const TransferFunction tanh; // "Tanh" TransfotmationFunction and derivative
            static const TransferFunction sigmoid; // "Sgmoid" TransfotmationFunction and derivative
            static const TransferFunction linear; // "Linear" TransfotmationFunction and derivative
            static const TransferFunction reLU; // "ReLU" TransfotmationFunction and derivative
            static const TransferFunction softmax; // TO DO
        private:
            // Functions and derivatives to put inside TransfotmationFunction
            static double TANH(double);
            static double DERTANH(double, double);
            
            static double SIGMOID(double);
            static double DERSIGMOID(double, double);

            static double LINEAR(double);
            static double DERLINEAR(double, double);

            static double RELU(double);
            static double DERRELU(double, double);

            static double SOFTMAX(double); // TO DO
            static double DERSOFTMAX(double, double); // TO DO
        };


        class lossFuncs{ // contains all build-in Loss functions
        public:
            static const LossFunction rmse; // "Root Mean Sqared Error" LossFunction
            static const LossFunction bce; // "Binary Cross-Entropy" LossFunction
            static const LossFunction cce; // TO DO
            static const LossFunction scce; // TO DO

        private:
            static double RMSE(const std::vector<double>&, const Jimmy::LAYER&);
            static double BCE(const std::vector<double>&, const Jimmy::LAYER&);
            static double CCE(const std::vector<double>&, const Jimmy::LAYER&);
            static double SCCE(const std::vector<double>&, const Jimmy::LAYER&);
        };
    };
}

#endif