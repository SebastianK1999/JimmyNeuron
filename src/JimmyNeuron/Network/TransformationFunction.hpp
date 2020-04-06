#ifndef TRANSFORMATIONFUNCTION_INCLUDE
#define TRANSFORMATIONFUNCTION_INCLUDE

namespace Jimmy{
    class TransformationFunction{
    private:
        double (*foo)(double);
        double (*derivativeFoo)(double,double);
    public:
        double result;
        double derivativeResult;
        double* run(double);
        TransformationFunction(double (*)(double),double (*)(double, double));
        TransformationFunction(const TransformationFunction&);
        
        void operator=(const TransformationFunction&);
    };
}
#endif