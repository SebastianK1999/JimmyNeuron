#include "JimmyNeuron/Network/Net.hpp"

Jimmy::Net::Net(std::vector<int> vec,const Jimmy::TransferFunction& transFoo,const Jimmy::LossFunction& lossFoo, double learningRate):
    transFunc(transFoo),
    lossFunc(lossFoo)
{   
    this->learningRate = learningRate;
    this->layers.reserve(vec.size());
    this->layers.emplace_back(Jimmy::Layer(vec[0])); // initializes input layer
    for(int i = 1; i < vec.size(); i++){ // does the rest and sets their connections
        assert(vec[i] >= 0);
        this->layers.emplace_back(Jimmy::Layer(vec[i], this->layers.back()));
    }
    this->outputValuesReferences.reserve(vec.back());
    this->outputReferences.reserve(vec.back());
    for(int i = 0; i <  vec.back(); i++){
        this->outputValuesReferences.emplace_back(this->layers.back().neurons[i].outValue);
        this->outputReferences.emplace_back(this->layers.back().neurons[i]);
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
        Jimmy::Neuron& outputNeuron = this->layers.back()[i];
        outputNeuron.gradient = this->learningRate * (realValues[i] - outputNeuron.outValue) * this->transFunc.runDerivative(outputNeuron.outValue);
    }

    // hidden gradient
    for(int i = this->layers.size()-2; i > 0; i--){
        Jimmy::Layer& currentLayer = this->layers[i];
        Jimmy::Layer& nextLayer = this->layers[i+1];
        for(int j = 0; j < this->layers[i].neurons.size(); j++){
            Jimmy::Neuron& currentNeuron = currentLayer[j];
            double sumGradientWeights = 0.0;
            for(int k = 0; k < nextLayer.neurons.size(); k++){
                Jimmy::Neuron& nextNeuron = nextLayer[k];
                sumGradientWeights += nextNeuron.inputWeights[j] * nextNeuron.gradient;
            }
            currentNeuron.gradient = this->learningRate * sumGradientWeights * this->transFunc.runDerivative(currentNeuron.outValue);
        }
    }

    // update weights
    for(unsigned i  = this->layers.size()-1; i > 0; i--){
        Jimmy::Layer& currentLayer = this->layers[i];
        for(int j = 0; j < currentLayer.neurons.size(); j++){
            currentLayer[j].updateInputWeights();
        }
    }
}

const double& Jimmy::Net::getResult(unsigned int i) const {
    return this->outputValuesReferences[i];
}
const std::vector<std::reference_wrapper<double>>& Jimmy::Net::getResult() const {
    return this->outputValuesReferences;
}

double Jimmy::Net::getLoss() const{
    return this->averageError;
}