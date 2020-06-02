#ifndef NEURON_INCLUDE
#define NEURON_INCLUDE

#include <iostream>
#include <climits>
#include <vector>
#include "TransferFunction.hpp"
#include "../Misc/Misc.hpp"

namespace Jimmy{
    class NEURON{
        public:
        double alpha; // momentum learning variable
        double gradient; // gradient of the neuron
        double weightedSum; // sum af all neuron inputs with weigts and self Weight
        double outValue; // Value the neuron outputs
        std::vector<double*> inputValuePointers; // Vector of references to input values
        std::vector<double> inputWeights; // Vector of weights of the references
        std::vector<double> deltaWeights;
        double selfWeight;// weight that is added to the output, that helps some neurosn work better
        double selfDeltaWeight;
        void think(Jimmy::TransferFunction&); // makes akes the data and calculates output
        void updateInputWeights(); // upates input weights of the neuron

        NEURON(); // Initializes a neuron without references to other neurons
        NEURON(std::vector<Jimmy::NEURON>&); // Initializes a neuron with references to other neurons
    };
}
#endif
