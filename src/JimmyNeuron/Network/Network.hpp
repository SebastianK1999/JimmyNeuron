#ifndef NETWORK_INCLUDE
#define NETWORK_INCLUDE

#include "Layer.hpp"
#include "TransformationFunction.hpp"

#include <JimmyNeuron/Jimmy.hpp>
#include <iostream>
#include <vector>

namespace Jimmy{
    class Net{ // Class for training and useage of Network
        private:
        std:: vector<Jimmy::LAYER> layers;
        Jimmy::TransformationFunction transFunc;

        public:
        double learningRate; // Variable that controlls speed and precision of learning // usualy between 0.01 and 0,0001
        void feedForward(const std::vector<double>&); // Takes reference, to input data of the first layer
        void backProp(const std::vector<double>&); // Takes reference, to what the results shold be in the output layer
        void getResult(std::vector<double>) const;

        Net(std::vector<unsigned int>,const Jimmy::TransformationFunction&); // Create network with, n naurons in each layer
    };
}

#endif

