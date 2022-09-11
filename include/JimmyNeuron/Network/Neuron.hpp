#pragma once

#include <iostream>
#include <climits>
#include <vector>

#include "JimmyNeuron/Network/TransferFunction.hpp"
#include "JimmyNeuron/Misc/Misc.hpp"

namespace Jimmy{
    class Neuron{
        public:
        int index;
        double alpha; // momentum learning variable
        double gradient; // gradient of the neuron
        double weightedSum; // sum af all neuron inputs with weights and self Weight
        double outValue; // Value the neuron outputs
        std::vector<double*> inputValuePointers; // Vector of references to input values
        std::vector<double> inputWeights; // Vector of weights of the references
        std::vector<double> deltaWeights;
        double selfWeight;// weight that is added to the output, that helps some neurons work better
        double selfDeltaWeight;
        void think(Jimmy::TransferFunction&); // takes the data and calculates output
        void updateInputWeights(); // updates input weights of the neuron

        Neuron(int); // Initializes a neuron without references to other neurons
        Neuron(int,std::vector<Jimmy::Neuron>&); // Initializes a neuron with references to other neurons
    };
}
