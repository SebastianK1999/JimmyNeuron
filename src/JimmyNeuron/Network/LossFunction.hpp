#ifndef LOSSFUNCTION_INCLUDE
#define LOSSFUNCTION_INCLUDE

#include "Layer.hpp"
#include <vector>
#include <functional>


namespace Jimmy{
    class LossFunction{
    private:
        float_t (*foo)(const std::vector<float_t>&, const std::vector<std::reference_wrapper<Jimmy::NEURON>>&); // Main-class function
        float_t error; // Result of the main-class function
    public:
        float_t run(const std::vector<float_t>&, const std::vector<std::reference_wrapper<Jimmy::NEURON>>&); // Executes Class-main function, saves the error and returns it
        float_t getError()const; // Outputs error calculededbu the run function
        LossFunction(float_t (*)(const std::vector<float_t>&, const std::vector<std::reference_wrapper<Jimmy::NEURON>>&));
        LossFunction(const LossFunction&);
        void operator=(const LossFunction&);
    };

}


#endif // LOSSFUNCTION_INCLUDE
