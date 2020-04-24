#ifndef LOSSFUNCTION_INCLUDE
#define LOSSFUNCTION_INCLUDE

#include "Layer.hpp"
#include <vector>


namespace Jimmy{
    class LossFunction{
    private:
        double (*foo)(const std::vector<double>&, const Jimmy::LAYER&); // Main-class function
        double error; // Result of the main-class function
    public:
        double run(const std::vector<double>&, const Jimmy::LAYER&); // Executes Class-main function, saves the error and returns it
        double getError()const; // Outputs error calculededbu the run function
        LossFunction(double (*)(const std::vector<double>&, const Jimmy::LAYER&));
        LossFunction(const LossFunction&);
        void operator=(const LossFunction&);
    };

}


#endif // LOSSFUNCTION_INCLUDE
