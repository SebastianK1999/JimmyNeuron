#ifndef NETWORK_INCLUDE
#define NETWORK_INCLUDE

#include "Layer.hpp"

#include <iostream>
#include <vector>

namespace Jimmy{
    class Net{ // Class for training and useage of Network
        private:
        std:: vector<Jimmy::LAYER> layers;

        public:
        void feedForward(std::vector<double>&); // Takes reference, to input data of the first layer
        void backProp(std::vector<double>&); // Takes reference, to what the results shold be in the output layer
        void getResult(std::vector<double>) const; 

        Net(std::vector<unsigned int>); // Create network with, n naurons in each layer
    };
}

#endif

