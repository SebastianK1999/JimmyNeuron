#ifndef LAYER_INCLUDE
#define LAYER_INCLUDE

#include "Neuron.hpp"

#include <vector>
#include <iostream>

namespace Jimmy{
    class LAYER{ // Stores and maneges neurons
        public:
        std::vector<Jimmy::NEURON> neurons;
        

        Jimmy::NEURON& operator[](int); // reurns reference to n-th neuron
        
        LAYER(unsigned int); // Create Layer with n  naurons
    };
}

#endif