#ifndef TRANSFERFUNCTION_INCLUDE
#define TRANSFERFUNCTION_INCLUDE

namespace Jimmy{
    class TransferFunction{
    private:
        double (*foo)(const double&); // Class-main function
        double (*derivativeFoo)(const double&); // Derivarive of the class-main function

    public:
        //double derivativeResult; // Output of the derivative function, executed in rum member function
        //double result; // Output of the class-main function, executed in rum member function
        double run(const double&); // Executes Class-main, return the result
        double runDervative(const double&); // Executes dervarive function, returns the result
        TransferFunction(double (*)(const double&),double (*)(const double&)); // constuctor taht allows to create a class instance using functions
        TransferFunction(const TransferFunction&); // constuctor that allows crate a copy OF ANOTHER CLASS INSTANCE
        
        void operator=(const TransferFunction&);
    };
}
#endif