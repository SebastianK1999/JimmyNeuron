#ifndef TRANSFERFUNCTION_INCLUDE
#define TRANSFERFUNCTION_INCLUDE

#include <math.h>

namespace Jimmy{
    class TransferFunction{
    private:
        float_t (*foo)(const float_t&); // Class-main function
        float_t (*derivativeFoo)(const float_t&); // Derivarive of the class-main function

    public:
        //float_t derivativeResult; // Output of the derivative function, executed in rum member function
        //float_t result; // Output of the class-main function, executed in rum member function
        float_t run(const float_t&); // Executes Class-main, return the result
        float_t runDervative(const float_t&); // Executes dervarive function, returns the result
        TransferFunction(float_t (*)(const float_t&),float_t (*)(const float_t&)); // constuctor taht allows to create a class instance using functions
        TransferFunction(const TransferFunction&); // constuctor that allows crate a copy OF ANOTHER CLASS INSTANCE
        
        void operator=(const TransferFunction&);
    };
}
#endif