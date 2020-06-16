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
        int index;
        float_t alpha; // momentum learning variable
        float_t gradient; // gradient of the neuron
        float_t weightedSum; // sum af all neuron inputs with weigts and self Weight
        float_t outValue; // Value the neuron outputs
        std::vector<float_t*> inputValuePointers; // Vector of references to input values
        std::vector<float_t> inputWeights; // Vector of weights of the references
        std::vector<float_t> deltaWeights;
        float_t selfWeight;// weight that is added to the output, that helps some neurosn work better
        float_t selfDeltaWeight;
        void think(Jimmy::TransferFunction&); // makes akes the data and calculates output
        void updateInputWeights(); // upates input weights of the neuron

        NEURON(int); // Initializes a neuron without references to other neurons
        NEURON(int,std::vector<Jimmy::NEURON>&); // Initializes a neuron with references to other neurons
    };
}
#endif
