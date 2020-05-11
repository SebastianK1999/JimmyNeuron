#ifndef NEURON_INCLUDE
#define NEURON_INCLUDE

#include <iostream>
#include <chrono>
#include <climits>
#include <vector>
#include "TransferFunction.hpp"

namespace Jimmy{
    class NEURON{
        public:
        double eta;
        double alpha;
        double gradient;
        double weightedSum;
        double outValue; // Value the neuron outputs
        std::vector<double*> inputValuePointers; // Vector of references to input values
        std::vector<double> inputWeights; // Vector of weights of the references
        std::vector<double> deltaWeights;
        double selfWeight;// weight that is added to the output, that helps some neurosn work better
        double selfDeltaWeight;
        void think(Jimmy::TransferFunction&); // makes akes the data and calculates output
        void updateInputWeights();

        NEURON(); // Initializes a neuron without references to other neurons
        NEURON(std::vector<Jimmy::NEURON>&); // Initializes a neuron with references to other neurons
    };
}
#endif
