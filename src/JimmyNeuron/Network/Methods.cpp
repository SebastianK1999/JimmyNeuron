#include "Methods.hpp"

// TransferFunctions

//      TANH
float_t Jimmy::Methods::transFuncs::TANH(const float_t& x){
    return std::tanh(x);
}
float_t Jimmy::Methods::transFuncs::DERTANH(const float_t& r){
    return 1 - r * r;
} 
const Jimmy::TransferFunction Jimmy::Methods::transFuncs::tanh = TransferFunction(Methods::transFuncs::TANH,Methods::transFuncs::DERTANH);


//      Sigmiod
float_t Jimmy::Methods::transFuncs::SIGMOID(const float_t& x){
    return x/(1 + abs(-x)) + 0.5;
}
float_t Jimmy::Methods::transFuncs::DERSIGMOID(const float_t& r){
    return r * (1 - r);
}
const Jimmy::TransferFunction Jimmy::Methods::transFuncs::sigmoid = TransferFunction(Methods::transFuncs::SIGMOID,Methods::transFuncs::DERSIGMOID);

//      Fast Sigmiod
float_t Jimmy::Methods::transFuncs::FASTSIGMOID(const float_t& x){
    return x/(1 + abs(-x));
}
const Jimmy::TransferFunction Jimmy::Methods::transFuncs::fastSigmoid = TransferFunction(Methods::transFuncs::FASTSIGMOID,Methods::transFuncs::DERTANH);

//      Linear
float_t Jimmy::Methods::transFuncs::LINEAR(const float_t& x){
    return x;
}
float_t Jimmy::Methods::transFuncs::DERLINEAR(const float_t& r){
    return 1;
}
const Jimmy::TransferFunction Jimmy::Methods::transFuncs::linear = TransferFunction(Methods::transFuncs::LINEAR,Methods::transFuncs::DERLINEAR);

// //      Piecewise Linear
// float_t Jimmy::Methods::transFuncs::LINEAR(const float_t& x){
//     if(x >= -1 && x <= 1){
//         return x;
//     }
//     else if(x > 1){
//         return 1;
//     }
//     return -1;
// }
// float_t Jimmy::Methods::transFuncs::DERLINEAR(const float_t& r){
//     if (x >= -1 && x <= 1){
//         return 1;
//     }
//     return 0;
// }
// const Jimmy::TransferFunction Jimmy::Methods::transFuncs::linear = TransferFunction(Methods::transFuncs::LINEAR,Methods::transFuncs::DERLINEAR);

//      ReLU
float_t Jimmy::Methods::transFuncs::RELU(const float_t& x){
    if (x >= 0){
        return x;
    }
    return 0;
}
float_t Jimmy::Methods::transFuncs::DERRELU(const float_t& r){
    if (r >= 0){
        return 1;
    }
    return 0;
}
const Jimmy::TransferFunction Jimmy::Methods::transFuncs::reLU = TransferFunction(Methods::transFuncs::RELU,Methods::transFuncs::DERRELU);

//      Softmax
float_t Jimmy::Methods::transFuncs::SOFTMAX(const float_t& x){
    if (x >= 0){
        return x;
    }
    return 0;
}
float_t Jimmy::Methods::transFuncs::DERSOFTMAX(const float_t& r){
    if (r >= 0){
        return 1;
    }
    return 0;
}
const Jimmy::TransferFunction Jimmy::Methods::transFuncs::softmax = TransferFunction(Methods::transFuncs::SOFTMAX,Methods::transFuncs::DERSOFTMAX);




// LossFunctions

//      RMSE
float_t Jimmy::Methods::lossFuncs::RMSE(const std::vector<float_t>& realValue,const std::vector<std::reference_wrapper<Jimmy::NEURON>>& outputLayer){
    float_t rmseError = 0;
    for(int i = 0; i < outputLayer.size(); i++){
       rmseError += pow(outputLayer[i].get().outValue - realValue[i], 2);
    }
    rmseError = sqrt(rmseError);
    return rmseError;
}
const Jimmy::LossFunction Jimmy::Methods::lossFuncs::rmse = LossFunction(Methods::lossFuncs::RMSE);

//      BCE
float_t Jimmy::Methods::lossFuncs::BCE(const std::vector<float_t>& realValue,const std::vector<std::reference_wrapper<Jimmy::NEURON>>& outputLayer){
    float_t bceError;
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
float_t Jimmy::Methods::lossFuncs::CCE(const std::vector<float_t>& realValue,const std::vector<std::reference_wrapper<Jimmy::NEURON>>& outputLayer){
    float_t cceError;
    for(int i = 0; i < outputLayer.size(); i++){
        cceError += - realValue[i] * log(outputLayer[i].get().outValue);
    }
    cceError /= -outputLayer.size();
    return cceError;
}
const Jimmy::LossFunction Jimmy::Methods::lossFuncs::cce = LossFunction(Methods::lossFuncs::CCE);

//      SCCE
/*float_t Jimmy::Methods::lossFuncs::SCCE(const std::vector<float_t>& realValue,const std::vector<std::reference_wrapper<Jimmy::NEURON>>& outputLayer){
    float_t bce_error;
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