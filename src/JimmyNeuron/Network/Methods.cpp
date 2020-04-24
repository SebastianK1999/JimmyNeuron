#include "Methods.hpp"

// TransferFunctions

//      TANH
double Jimmy::Methods::transFuncs::TANH(double x){
    return std::tanh(x);
}
double Jimmy::Methods::transFuncs::DERTANH(double x, double r){
    return 1 - r*r;
} 
const Jimmy::TransferFunction Jimmy::Methods::transFuncs::tanh = TransferFunction(Methods::transFuncs::TANH,Methods::transFuncs::DERTANH);


//      Sigmiod
double Jimmy::Methods::transFuncs::SIGMOID(double x){
    return 1/(1+std::exp(-x));
}
double Jimmy::Methods::transFuncs::DERSIGMOID(double x, double r){
    return r*(1-r);
}
const Jimmy::TransferFunction Jimmy::Methods::transFuncs::sigmoid = TransferFunction(Methods::transFuncs::SIGMOID,Methods::transFuncs::DERSIGMOID);


//      Linear
double Jimmy::Methods::transFuncs::LINEAR(double x){
    return x;
}
double Jimmy::Methods::transFuncs::DERLINEAR(double x, double r){
    return 1;
}
const Jimmy::TransferFunction Jimmy::Methods::transFuncs::linear = TransferFunction(Methods::transFuncs::LINEAR,Methods::transFuncs::DERLINEAR);


//      ReLU
double Jimmy::Methods::transFuncs::RELU(double x){
    if (x >= 0){
        return x;
    }
    return 0;
}
double Jimmy::Methods::transFuncs::DERRELU(double x, double r){
    if (x >= 0){
        return 1;
    }
    return 0;
}
const Jimmy::TransferFunction Jimmy::Methods::transFuncs::reLU = TransferFunction(Methods::transFuncs::RELU,Methods::transFuncs::DERRELU);

//      Softmax
double Jimmy::Methods::transFuncs::SOFTMAX(double x){
    if (x >= 0){
        return x;
    }
    return 0;
}
double Jimmy::Methods::transFuncs::DERSOFTMAX(double x, double r){
    if (x >= 0){
        return 1;
    }
    return 0;
}
const Jimmy::TransferFunction Jimmy::Methods::transFuncs::softmax = TransferFunction(Methods::transFuncs::SOFTMAX,Methods::transFuncs::DERSOFTMAX);




// LossFunctions

//      RMSE
double Jimmy::Methods::lossFuncs::RMSE(const std::vector<double>& realValue,const Jimmy::LAYER& outputLayer){
    double rmseError = 0;
    for(int i = 0; i < outputLayer.neurons.size(); i++){
       rmseError += pow(outputLayer.neurons[i].outValue - realValue[i], 2);
    }
    rmseError = sqrt(rmseError);
    return rmseError;
}
const Jimmy::LossFunction Jimmy::Methods::lossFuncs::rmse = LossFunction(Methods::lossFuncs::RMSE);

//      BCE
double Jimmy::Methods::lossFuncs::BCE(const std::vector<double>& realValue,const Jimmy::LAYER& outputLayer){
    double bceError;
    for(int i = 0; i < outputLayer.neurons.size(); i++){
        if(realValue[i] == 1){
            bceError += log(outputLayer.neurons[i].outValue);
        }
        else{
            bceError += log(1-outputLayer.neurons[i].outValue);
        }
    }
    bceError /= -outputLayer.neurons.size();
    return bceError;
}
const Jimmy::LossFunction Jimmy::Methods::lossFuncs::bce = LossFunction(Methods::lossFuncs::BCE);

//      CCE
double Jimmy::Methods::lossFuncs::CCE(const std::vector<double>& realValue,const Jimmy::LAYER& outputLayer){
    double cceError;
    for(int i = 0; i < outputLayer.neurons.size(); i++){
        cceError += - realValue[i] * log(outputLayer.neurons[i].outValue);
    }
    cceError /= -outputLayer.neurons.size();
    return cceError;
}
const Jimmy::LossFunction Jimmy::Methods::lossFuncs::cce = LossFunction(Methods::lossFuncs::CCE);

//      SCCE
/*double Jimmy::Methods::lossFuncs::SCCE(const std::vector<double>& realValue,const Jimmy::LAYER& outputLayer){
    double bce_error;
    for(int i = 0; i < outputLayer.neurons.size(); i++){
        if(realValue[i] == 1){
            bce_error += log(outputLayer.neurons[i].outValue);
        }
        else{
            bce_error += log(1-outputLayer.neurons[i].outValue);
        }
    }
    bce_error /= -outputLayer.neurons.size();
    return bce_error;
}
const Jimmy::LossFunction Jimmy::Methods::lossFuncs::scce = LossFunction(Methods::lossFuncs::SCCE);
*/