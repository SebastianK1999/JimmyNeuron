#include "Network.hpp"

Jimmy::Net::Net(std::vector<unsigned int> vec,const Jimmy::TransferFunction& transFoo,const Jimmy::LossFunction& lossFoo, double learningrate):
    transFunc(transFoo),
    lossFunc(lossFoo)
{  
    this->learningRate = learningrate;
    this->layers.push_back(Jimmy::LAYER(vec[0])); // initializes imput layer
    for(int i = 1; i < vec.size(); i++){ // does the rest and sets theri connections
        this->layers.push_back(Jimmy::LAYER(vec[i], this->layers.back()));
    }
    for(int i = 0; i <  this->layers.back().neurons.size(); i++){
        this->outputValuesReferences.push_back(this->layers.back().neurons[i].outValue);
        this->outputReferences.push_back(this->layers.back().neurons[i]);
    }
}

void Jimmy::Net::feedForward(const std::vector<double>& inputs){
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

void Jimmy::Net::backProp(const std::vector<double>& realValues){
    // network gradient 
    double error = this->lossFunc.run(realValues, this->outputReferences);
    this->averageError = (this->averageError * 100 + error) / (100 + 1.0);

    // output gradient
    for(int i = 0; i < this->layers.back().neurons.size(); i++){
        this->layers.back()[i].gradient = this->learningRate * (realValues[i] - this->layers.back()[i].outValue) * this->transFunc.runDervative(this->layers.back()[i].outValue);
    }

    // hiden gradient
    for(int i = this->layers.size()-2; i > 0; i--){
        for(int j = 0; j < this->layers[i].neurons.size(); j++){
            double sumGradientWeights = 0.0;
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

const double& Jimmy::Net::getResult(int i) const {
    return this->outputValuesReferences[i];
}
const std::vector<std::reference_wrapper<double>>& Jimmy::Net::getResult() const {
    return this->outputValuesReferences;
}

double Jimmy::Net::getLoss() const{
    return this->averageError;
}