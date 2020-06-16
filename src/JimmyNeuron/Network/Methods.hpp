#ifndef METHODS_INCLUDE
#define METHODS_INCLUDE

#include "TransferFunction.hpp"
#include "LossFunction.hpp"
#include "Layer.hpp"
#include <vector>
#include <cmath>
#include <iostream>
#include <functional>

namespace Jimmy{
    class Methods{
    public:
        class transFuncs{ // contains all build-in Transfer functions
        public:
            static const TransferFunction tanh; // "Tanh" TransfotmationFunction and derivative
            static const TransferFunction sigmoid; // "Sgmoid" TransfotmationFunction and derivative
            static const TransferFunction fastSigmoid; // "FastSgmoid" TransfotmationFunction and derivative
            static const TransferFunction linear; // "Linear" TransfotmationFunction and derivative
            static const TransferFunction reLU; // "ReLU" TransfotmationFunction and derivative
            static const TransferFunction softmax; // TO DO
        private:
            // Functions and derivatives to put inside TransfotmationFunction
            static double TANH(const double&);
            static double DERTANH(const double&);
            
            static double SIGMOID(const double&);
            static double DERSIGMOID(const double&);

            static double FASTSIGMOID(const double&);
                // DERFASTSIGMOID IS THE SAME AS DERTANH

            static double LINEAR(const double&);
            static double DERLINEAR(const double&);

            static double RELU(const double&);
            static double DERRELU(const double&);

            static double SOFTMAX(const double&); // TO DO
            static double DERSOFTMAX(const double&); // TO DO
        };


        class lossFuncs{ // contains all build-in Loss functions
        public:
            static const LossFunction rmse; // "Root Mean Sqared Error" LossFunction
            static const LossFunction bce; // "Binary Cross-Entropy" LossFunction
            static const LossFunction cce; // TO DO
            static const LossFunction scce; // TO DO

        private:
            static double RMSE(const std::vector<double>&, const std::vector<std::reference_wrapper<Jimmy::NEURON>>&);
            static double BCE(const std::vector<double>&, const std::vector<std::reference_wrapper<Jimmy::NEURON>>&);
            static double CCE(const std::vector<double>&, const std::vector<std::reference_wrapper<Jimmy::NEURON>>&);
            static double SCCE(const std::vector<double>&, const std::vector<std::reference_wrapper<Jimmy::NEURON>>&);
        };
    };
}

#endif