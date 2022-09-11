# include "JimmyNeuron/Network/RecordsIO.hpp"

void Jimmy::RECORDS_IO::clear(){
    this->inputs.clear();
    this->inputWeights.clear();
    this->outputIndexes.clear();

}

void Jimmy::RECORDS_IO::saveInputWeights(Jimmy::Net* networkPointer){
    this->inputWeights.reserve(networkPointer->layers.size());
    this->inputWeights.emplace_back(std::vector<Jimmy::NEURON_WEIGHTS>{});
    for(int layerIndex = 1; layerIndex < networkPointer->layers.size(); layerIndex++){
        this->inputWeights.emplace_back(std::vector<Jimmy::NEURON_WEIGHTS>{});
        this->inputWeights[layerIndex].reserve(networkPointer->layers[layerIndex].neurons.size());
        for(int neuronIndex = 0; neuronIndex < networkPointer->layers[layerIndex].neurons.size(); neuronIndex++){        
            this->inputWeights.back().emplace_back(Jimmy::NEURON_WEIGHTS());
            this->inputWeights[layerIndex][neuronIndex].inputWeightsRecords.reserve(networkPointer->layers[layerIndex][neuronIndex].inputWeights.size());
            for(unsigned weightIndex = 0; weightIndex < networkPointer->layers[layerIndex][neuronIndex].inputWeights.size(); weightIndex++){              
                this->inputWeights.back().back().inputWeightsRecords.emplace_back(networkPointer->layers[layerIndex][neuronIndex].inputWeights[weightIndex]);
            }
            this->inputWeights.back().back().selfWeightRecord = networkPointer->layers[layerIndex][neuronIndex].selfWeight;                     
        }
    }
}