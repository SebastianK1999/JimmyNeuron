#pragma once

#include <vector>
#include <cmath>
#include <iostream>
#include <functional>

#include "JimmyNeuron/Network/LossFunction.hpp"
#include "JimmyNeuron/Network/Layer.hpp"
#include "JimmyNeuron/Network/TransferFunction.hpp"

namespace Jimmy{
    class Methods{
    public:
        class transFuncs{ // contains all build-in Transfer functions
        public:
            static const TransferFunction tanh; // "Tanh" TransformationFunction and derivative
            static const TransferFunction sigmoid; // "Sigmoid" TransformationFunction and derivative
            static const TransferFunction fastSigmoid; // "FastSigmoid" TransformationFunction and derivative
            static const TransferFunction linear; // "Linear" TransformationFunction and derivative
            static const TransferFunction reLU; // "ReLU" TransformationFunction and derivative
            static const TransferFunction softmax; // TO DO
        private:
            // Functions and derivatives to put inside TransformationFunction
            static double TANH(const double&);
            static double TANH_DER(const double&);
            
            static double SIGMOID(const double&);
            static double SIGMOID_DER(const double&);

            static double FASTSIGMOID(const double&);
                // FASTSIGMOID_DER IS THE SAME AS TANH_DER

            static double LINEAR(const double&);
            static double LINEAR_DER(const double&);

            static double RELU(const double&);
            static double RELU_DER(const double&);

            static double SOFTMAX(const double&); // TO DO
            static double SOFTMAX_DER(const double&); // TO DO
        };


        class lossFuncs{ // contains all build-in Loss functions
        public:
            static const LossFunction rmse; // "Root Mean Squared Error" LossFunction
            static const LossFunction bce; // "Binary Cross-Entropy" LossFunction
            static const LossFunction cce; // TO DO
            static const LossFunction scce; // TO DO

        private:
            static double RMSE(const std::vector<double>&, const std::vector<std::reference_wrapper<Jimmy::Neuron>>&);
            static double BCE(const std::vector<double>&, const std::vector<std::reference_wrapper<Jimmy::Neuron>>&);
            static double CCE(const std::vector<double>&, const std::vector<std::reference_wrapper<Jimmy::Neuron>>&);
            static double SCCE(const std::vector<double>&, const std::vector<std::reference_wrapper<Jimmy::Neuron>>&);
        };
    };
}
