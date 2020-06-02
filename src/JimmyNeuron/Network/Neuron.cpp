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
        this->deltaWeights[i] = (*this->inputValuePointers[i]) * this->gradient;// + this->alpha * this->deltaWeights[i];
        this->inputWeights[i] += this->deltaWeights[i];
    }
    // updating the constant added to input neurons
    this->selfDeltaWeight = this->gradient + this->alpha * this->selfDeltaWeight;
    this->selfWeight += this->selfDeltaWeight;
}

Jimmy::NEURON::NEURON(){
    this->alpha = 0.05;
    this->selfWeight = 0;
}

Jimmy::NEURON::NEURON(std::vector<Jimmy::NEURON>& refNeurons)
:Jimmy::NEURON::NEURON() // Basic init{
{
    for(int i = 0; i < refNeurons.size(); i++){
        this->inputValuePointers.push_back(&(refNeurons[i].outValue));
        this->inputWeights.push_back(Jimmy::Misc::rand::get().Double());
        this->selfWeight = Jimmy::Misc::rand::get().Double();
        this->gradient = 0.0;
        this->deltaWeights.push_back(0.0);
    }
}