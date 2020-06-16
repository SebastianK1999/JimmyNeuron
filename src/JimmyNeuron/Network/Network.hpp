#ifndef NETWORK_INCLUDE
#define NETWORK_INCLUDE

#include "Layer.hpp"
#include "TransferFunction.hpp"
#include "LossFunction.hpp"

#include <JimmyNeuron/Jimmy.hpp>
#include <iostream>
#include <vector>
#include <functional>
#include <cassert>

namespace Jimmy{
    class Net{ // Class for training and useage of Network
        friend class Critic; // defying Jimmy::Critic as a friend class
        friend class RECORDS_IO; // defying Jimmy::RECORDS_IO as record
        private:
        std::vector<Jimmy::LAYER> layers; // Layes of the net
        Jimmy::TransferFunction transFunc; // copy of the transfer function
        Jimmy::LossFunction lossFunc; // Copy of the loss unction
        double averageError; // aveege error of the net
        std::vector<std::reference_wrapper<double>> outputValuesReferences; // reference to the output valeus in the last layer of the net
        std::vector<std::reference_wrapper<Jimmy::NEURON>> outputReferences; // references to the neurons in the the last layer

        public:
        double getLoss() const;
        double learningRate; // Variable that controlls speed and precision of learning // usualy between 0.01 and 0,0001
        void feedForward(const std::vector<double>&); // Takes reference, to input data of the first layer
        void backProp(const std::vector<double>&); // Takes reference, to what the results shold be in the output layer
        const double& getResult(unsigned int) const; // returns output vale of the indexed neuron in last layer
        const std::vector<std::reference_wrapper<double>>& getResult() const; // return vector on the

        Net(std::vector<int>, const Jimmy::TransferFunction&, const Jimmy::LossFunction&, double); // Create network with, n naurons in each layer
    };
}

#endif

