#ifndef TRANSFERFUNCTION_INCLUDE
#define TRANSFERFUNCTION_INCLUDE

namespace Jimmy{
    class TransferFunction{
    private:
        double (*foo)(double); // Class-main function
        double (*derivativeFoo)(double,double); // Derivarive of the class-main function

    public:
        //double derivativeResult; // Output of the derivative function, executed in rum member function
        //double result; // Output of the class-main function, executed in rum member function
        double run(double); // Executes Class-main, return the result
        double runDervative(double,double); // Executes dervarive function, returns the result
        TransferFunction(double (*)(double),double (*)(double, double)); // constuctor taht allows to create a class instance using functions
        TransferFunction(const TransferFunction&); // constuctor that allows crate a copy OF ANOTHER CLASS INSTANCE
        
        void operator=(const TransferFunction&);
    };
}
#endif