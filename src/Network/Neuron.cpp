#include "JimmyNeuron/Network/Neuron.hpp"

void Jimmy::Neuron::think(Jimmy::TransferFunction& transfoo){
    this->weightedSum = this->selfWeight;
    for(int i = 0; i < this->inputValuePointers.size(); i++){
        this->weightedSum += *this->inputValuePointers[i] * this->inputWeights[i];
    }
    this->outValue = transfoo.run(this->weightedSum);
}

void Jimmy::Neuron::updateInputWeights(){
    // updating input Weights
    for(int i = 0; i < this->inputWeights.size(); i++){
        this->deltaWeights[i] = (*this->inputValuePointers[i]) * this->gradient;// + this->alpha * this->deltaWeights[i];
        this->inputWeights[i] += this->deltaWeights[i];
    }
    // updating the constant added to input neurons
    this->selfDeltaWeight = this->gradient + this->alpha * this->selfDeltaWeight;
    this->selfWeight += this->selfDeltaWeight;
}

Jimmy::Neuron::Neuron(int index){
    this->index = index;
    this->alpha = 0.05;
    this->selfWeight = 0;
}

Jimmy::Neuron::Neuron(int index, std::vector<Jimmy::Neuron>& refNeurons)
:Jimmy::Neuron::Neuron(index) // Basic init{
{
    for(int i = 0; i < refNeurons.size(); i++){
        this->inputValuePointers.push_back(&(refNeurons[i].outValue));
        this->inputWeights.push_back(Jimmy::Misc::rand::get().Decimal()/refNeurons.size());
        // this->inputWeights.push_back((Jimmy::Misc::rand::get().Decimal()*2-1)/refNeurons.size()*2);
        this->selfWeight = Jimmy::Misc::rand::get().Decimal();
        this->gradient = 0.0;
        this->deltaWeights.push_back(0.0);
    }
}