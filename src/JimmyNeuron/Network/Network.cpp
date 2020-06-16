#include "Network.hpp"

Jimmy::Net::Net(std::vector<int> vec,const Jimmy::TransferFunction& transFoo,const Jimmy::LossFunction& lossFoo, float_t learningrate):
    transFunc(transFoo),
    lossFunc(lossFoo)
{  
    
    this->learningRate = learningrate;
    this->layers.reserve(vec.size());
    this->layers.emplace_back(Jimmy::LAYER(vec[0])); // initializes imput layer
    for(int i = 1; i < vec.size(); i++){ // does the rest and sets theri connections
        assert(vec[i] >= 0);
        this->layers.emplace_back(Jimmy::LAYER(vec[i], this->layers.back()));
    }
    this->outputValuesReferences.reserve(vec.back());
    this->outputReferences.reserve(vec.back());
    for(int i = 0; i <  vec.back(); i++){
        this->outputValuesReferences.emplace_back(this->layers.back().neurons[i].outValue);
        this->outputReferences.emplace_back(this->layers.back().neurons[i]);
    }
}

void Jimmy::Net::feedForward(const std::vector<float_t>& inputs){
    assert(inputs.size() == this->layers[0].neurons.size());

    for(int i = 0; i < this->layers[0].neurons.size(); i++){
        this->layers[0][i].outValue = inputs[i];
    }
    for(int i = 1; i < this->layers.size(); i++){
        for(int j = 0; j < this->layers[i].neurons.size(); j++){
            this->layers[i][j].think(this->transFunc);
        }
    }
}

void Jimmy::Net::backProp(const std::vector<float_t>& realValues){
    // network gradient 
    float_t error = this->lossFunc.run(realValues, this->outputReferences);
    this->averageError = (this->averageError * 100 + error) / (100 + 1.0);

    // output gradient
    for(int i = 0; i < this->layers.back().neurons.size(); i++){
        this->layers.back()[i].gradient = this->learningRate * (realValues[i] - this->layers.back()[i].outValue) * this->transFunc.runDervative(this->layers.back()[i].outValue);
    }

    // hiden gradient
    for(int i = this->layers.size()-2; i > 0; i--){
        for(int j = 0; j < this->layers[i].neurons.size(); j++){
            float_t sumGradientWeights = 0.0;
            for(int k = 0; k < this->layers[i+1].neurons.size(); k++){
                sumGradientWeights += this->layers[i+1][k].inputWeights[j] * this->layers[i+1][k].gradient;
            }
            this->layers[i][j].gradient = this->learningRate * sumGradientWeights * this->transFunc.runDervative(this->layers[i][j].outValue);
        }
    }

    // update weights
    for(unsigned i  = this->layers.size()-1; i > 0; i--){
        for(int j = 0; j < this->layers[i].neurons.size(); j++){
            this->layers[i][j].updateInputWeights();
        }
    }
}

const float_t& Jimmy::Net::getResult(unsigned int i) const {
    return this->outputValuesReferences[i];
}
const std::vector<std::reference_wrapper<float_t>>& Jimmy::Net::getResult() const {
    return this->outputValuesReferences;
}

float_t Jimmy::Net::getLoss() const{
    return this->averageError;
}