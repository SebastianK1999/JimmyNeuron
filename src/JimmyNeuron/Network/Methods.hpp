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
            static float_t TANH(const float_t&);
            static float_t DERTANH(const float_t&);
            
            static float_t SIGMOID(const float_t&);
            static float_t DERSIGMOID(const float_t&);

            static float_t FASTSIGMOID(const float_t&);
                // DERFASTSIGMOID IS THE SAME AS DERTANH

            static float_t LINEAR(const float_t&);
            static float_t DERLINEAR(const float_t&);

            static float_t RELU(const float_t&);
            static float_t DERRELU(const float_t&);

            static float_t SOFTMAX(const float_t&); // TO DO
            static float_t DERSOFTMAX(const float_t&); // TO DO
        };


        class lossFuncs{ // contains all build-in Loss functions
        public:
            static const LossFunction rmse; // "Root Mean Sqared Error" LossFunction
            static const LossFunction bce; // "Binary Cross-Entropy" LossFunction
            static const LossFunction cce; // TO DO
            static const LossFunction scce; // TO DO

        private:
            static float_t RMSE(const std::vector<float_t>&, const std::vector<std::reference_wrapper<Jimmy::NEURON>>&);
            static float_t BCE(const std::vector<float_t>&, const std::vector<std::reference_wrapper<Jimmy::NEURON>>&);
            static float_t CCE(const std::vector<float_t>&, const std::vector<std::reference_wrapper<Jimmy::NEURON>>&);
            static float_t SCCE(const std::vector<float_t>&, const std::vector<std::reference_wrapper<Jimmy::NEURON>>&);
        };
    };
}

#endif