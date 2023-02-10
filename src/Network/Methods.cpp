#include "JimmyNeuron/Network/Methods.hpp"

// TransferFunctions

//      TANH
double Jimmy::Methods::transFuncs::TANH(const double& x){
    return std::tanh(x);
}
double Jimmy::Methods::transFuncs::TANH_DER(const double& r){
    return 1 - r * r;
} 
const Jimmy::TransferFunction Jimmy::Methods::transFuncs::tanh = TransferFunction(Methods::transFuncs::TANH,Methods::transFuncs::TANH_DER);


//      Sigmoid
double Jimmy::Methods::transFuncs::SIGMOID(const double& x){
    return x/(1 + abs(-x)) + 0.5;
}
double Jimmy::Methods::transFuncs::SIGMOID_DER(const double& r){
    return r * (1 - r);
}
const Jimmy::TransferFunction Jimmy::Methods::transFuncs::sigmoid = TransferFunction(Methods::transFuncs::SIGMOID,Methods::transFuncs::SIGMOID_DER);

//      Fast Sigmoid
double Jimmy::Methods::transFuncs::FASTSIGMOID(const double& x){
    return x/(1 + abs(-x));
}
const Jimmy::TransferFunction Jimmy::Methods::transFuncs::fastSigmoid = TransferFunction(Methods::transFuncs::FASTSIGMOID,Methods::transFuncs::TANH_DER);

//      Linear
double Jimmy::Methods::transFuncs::LINEAR(const double& x){
    return x;
}
double Jimmy::Methods::transFuncs::LINEAR_DER(const double& r){
    return 1;
}
const Jimmy::TransferFunction Jimmy::Methods::transFuncs::linear = TransferFunction(Methods::transFuncs::LINEAR,Methods::transFuncs::LINEAR_DER);

// //      Piecewise Linear
// double Jimmy::Methods::transFuncs::LINEAR(const double& x){
//     if(x >= -1 && x <= 1){
//         return x;
//     }
//     else if(x > 1){
//         return 1;
//     }
//     return -1;
// }
// double Jimmy::Methods::transFuncs::LINEAR_DER(const double& r){
//     if (x >= -1 && x <= 1){
//         return 1;
//     }
//     return 0;
// }
// const Jimmy::TransferFunction Jimmy::Methods::transFuncs::linear = TransferFunction(Methods::transFuncs::LINEAR,Methods::transFuncs::LINEAR_DER);

//      ReLU
double Jimmy::Methods::transFuncs::RELU(const double& x){
    if (x >= 0){
        return x;
    }
    return 0;
}
double Jimmy::Methods::transFuncs::RELU_DER(const double& r){
    if (r >= 0){
        return 1;
    }
    return 0;
}
const Jimmy::TransferFunction Jimmy::Methods::transFuncs::reLU = TransferFunction(Methods::transFuncs::RELU,Methods::transFuncs::RELU_DER);

// //      Softmax
// double Jimmy::Methods::transFuncs::SOFTMAX(const double& x){
//     if (x >= 0){
//         return x;
//     }
//     return 0;
// }
// double Jimmy::Methods::transFuncs::SOFTMAX_DER(const double& r){
//     if (r >= 0){
//         return 1;
//     }
//     return 0;
// }
// const Jimmy::TransferFunction Jimmy::Methods::transFuncs::softmax = TransferFunction(Methods::transFuncs::SOFTMAX,Methods::transFuncs::SOFTMAX_DER);




// LossFunctions

//      RMSE
double Jimmy::Methods::lossFuncs::RMSE(const std::vector<double>& realValue,const std::vector<std::reference_wrapper<Jimmy::Neuron>>& outputLayer){
    double rmseError = 0;
    for(int i = 0; i < outputLayer.size(); i++){
       rmseError += pow(outputLayer[i].get().outValue - realValue[i], 2);
    }
    rmseError = sqrt(rmseError);
    return rmseError;
}
const Jimmy::LossFunction Jimmy::Methods::lossFuncs::rmse = LossFunction(Methods::lossFuncs::RMSE);

//      BCE
double Jimmy::Methods::lossFuncs::BCE(const std::vector<double>& realValue,const std::vector<std::reference_wrapper<Jimmy::Neuron>>& outputLayer){
    double bceError;
    for(int i = 0; i < outputLayer.size(); i++){
        if(realValue[i] == 1){
            bceError += log(outputLayer[i].get().outValue);
        }
        else{
            bceError += log(1-outputLayer[i].get().outValue);
        }
    }
    bceError /= -outputLayer.size();
    return bceError;
}
const Jimmy::LossFunction Jimmy::Methods::lossFuncs::bce = LossFunction(Methods::lossFuncs::BCE);

//      CCE
double Jimmy::Methods::lossFuncs::CCE(const std::vector<double>& realValue,const std::vector<std::reference_wrapper<Jimmy::Neuron>>& outputLayer){
    double cceError;
    for(int i = 0; i < outputLayer.size(); i++){
        cceError += - realValue[i] * log(outputLayer[i].get().outValue);
    }
    cceError /= -outputLayer.size();
    return cceError;
}
const Jimmy::LossFunction Jimmy::Methods::lossFuncs::cce = LossFunction(Methods::lossFuncs::CCE);

//      SCCE
/*double Jimmy::Methods::lossFuncs::SCCE(const std::vector<double>& realValue,const std::vector<std::reference_wrapper<Jimmy::Neuron>>& outputLayer){
    double bce_error;
    for(int i = 0; i < outputLayer.size(); i++){
        if(realValue[i] == 1){
            bce_error += log(outputLayer[i].get().outValue);
        }
        else{
            bce_error += log(1-outputLayer[i].get().outValue);
        }
    }
    bce_error /= -outputLayer.size();
    return bce_error;
}
const Jimmy::LossFunction Jimmy::Methods::lossFuncs::scce = LossFunction(Methods::lossFuncs::SCCE);
*/