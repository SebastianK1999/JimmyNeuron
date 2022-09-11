#pragma once

#include <vector>
#include <functional>

#include "JimmyNeuron/Network/Layer.hpp"

namespace Jimmy{
    class LossFunction{
    private:
        double (*foo)(const std::vector<double>&, const std::vector<std::reference_wrapper<Jimmy::Neuron>>&); // Main-class function
        double error; // Result of the main-class function
    public:
        double run(const std::vector<double>&, const std::vector<std::reference_wrapper<Jimmy::Neuron>>&); // Executes Class-main function, saves the error and returns it
        double getError()const; // Outputs error calculated by the run function
        LossFunction(double (*)(const std::vector<double>&, const std::vector<std::reference_wrapper<Jimmy::Neuron>>&));
        LossFunction(const LossFunction&);
        void operator=(const LossFunction&);
    };

}
