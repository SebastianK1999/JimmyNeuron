#include "Neuron.hpp"

void Jimmy::NEURON::think(Jimmy::TransformationFunction transfoo){
    int weightedSum = this->selfWeight;
    for(int i = 0; i < this->inputValuePointers.size(); i++){
        weightedSum += *this->inputValuePointers[i] * this->inputWeights[i];
    }
    transfoo.run(weightedSum);
    this->outValue = transfoo.result;
}

Jimmy::NEURON::NEURON(){
    // this->outValue = 1;
    this->selfWeight = 0;
}

Jimmy::NEURON::NEURON(std::vector<Jimmy::NEURON>& refNeurons)
:Jimmy::NEURON::NEURON() // Basic init{
{
    std::srand(std::chrono::steady_clock::now().time_since_epoch().count());
    for(int i = 0; i < refNeurons.size(); i++){
        this->inputValuePointers.push_back(&refNeurons[i].outValue);
        this->inputWeights.push_back((double) std::rand() / INT_MAX);// / std::numeric_limits<double>::max());
    }
}