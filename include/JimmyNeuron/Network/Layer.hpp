#pragma once

#include <vector>
#include <iostream>

#include "JimmyNeuron/Network/Neuron.hpp"

namespace Jimmy{
    class Layer{ // Stores and manages neurons
        public:
        std::vector<Jimmy::Neuron> neurons; // vector of neurons stored in layer
        
        Jimmy::Neuron& operator[](int); // reurns reference to n-th neuron
        Layer(int); // Create Layer with n neurons
        Layer(int, Jimmy::Layer &); // Create Layer with n neurons, and pass references to their neurons connections
    };
}