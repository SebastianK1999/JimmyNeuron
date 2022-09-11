#pragma once

#include <math.h>

namespace Jimmy{
    class TransferFunction{
    private:
        double (*foo)(const double&); // Class-main function
        double (*derivativeFoo)(const double&); // Derivative of the class-main function

    public:
        double run(const double&); // Executes Class-main, return the result
        double runDerivative(const double&); // Executes derivative function, returns the result
        TransferFunction(double (*)(const double&),double (*)(const double&)); // constructor that allows to create a class instance using functions
        TransferFunction(const TransferFunction&); // constructor that allows crate a copy OF ANOTHER CLASS INSTANCE
        
        void operator=(const TransferFunction&);
    };
}
