#include "Neuron.hpp"

void Jimmy::NEURON::think(Jimmy::TransferFunction& transfoo){
    this->weightedSum = this->selfWeight;
    for(int i = 0; i < this->inputValuePointers.size(); i++){
        this->weightedSum += *this->inputValuePointers[i] * this->inputWeights[i];
    }
    this->outValue = transfoo.run(this->weightedSum);
}

void Jimmy::NEURON::updateInputWeights(){
    // updating input Weights
    for(int i = 0; i < this->inputWeights.size(); i++){
        this->deltaWeights[i] = this->eta * (*this->inputValuePointers[i]) * this->gradient + this->alpha * this->deltaWeights[i];
        this->inputWeights[i] += this->deltaWeights[i];
    }
    // updating the constant added to input neurons
    this->selfDeltaWeight = this->eta * this->gradient + this->alpha * this->selfDeltaWeight;
    this->selfWeight += this->selfDeltaWeight;
}

Jimmy::NEURON::NEURON(){
    this->alpha = 0.05;
    this->eta = 0.01;
    this->selfWeight = 0;
}

Jimmy::NEURON::NEURON(std::vector<Jimmy::NEURON>& refNeurons)
:Jimmy::NEURON::NEURON() // Basic init{
{
    std::srand(std::chrono::steady_clock::now().time_since_epoch().count());
    for(int i = 0; i < refNeurons.size(); i++){
        this->inputValuePointers.push_back(&refNeurons[i].outValue);
        this->inputWeights.push_back((double) std::rand() / INT_MAX);// / std::numeric_limits<double>::max());
        this->deltaWeights.push_back(0.0);
    }
}