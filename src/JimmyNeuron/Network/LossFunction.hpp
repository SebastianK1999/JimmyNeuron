#ifndef LOSSFUNCTION_INCLUDE
#define LOSSFUNCTION_INCLUDE

#include "Layer.hpp"
#include <vector>
#include <functional>


namespace Jimmy{
    class LossFunction{
    private:
        double (*foo)(const std::vector<double>&, const std::vector<std::reference_wrapper<Jimmy::NEURON>>&); // Main-class function
        double error; // Result of the main-class function
    public:
        double run(const std::vector<double>&, const std::vector<std::reference_wrapper<Jimmy::NEURON>>&); // Executes Class-main function, saves the error and returns it
        double getError()const; // Outputs error calculededbu the run function
        LossFunction(double (*)(const std::vector<double>&, const std::vector<std::reference_wrapper<Jimmy::NEURON>>&));
        LossFunction(const LossFunction&);
        void operator=(const LossFunction&);
    };

}


#endif // LOSSFUNCTION_INCLUDE
