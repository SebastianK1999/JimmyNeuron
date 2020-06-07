# include "RecordsIO.hpp"

void Jimmy::RECORDS_IO::clear(){
    this->inputs.clear();
    this->inputWeights.clear();
    this->outputIndexes.clear();

}

void Jimmy::RECORDS_IO::saveInputWeights(Jimmy::Net* networkPointer){
    this->inputWeights.push_back(std::vector<Jimmy::NEURON_WEIGHTS>{});
    for(unsigned int layerIndex = 1; layerIndex < networkPointer->layers.size(); layerIndex++){    
        this->inputWeights.push_back(std::vector<Jimmy::NEURON_WEIGHTS>{});
        for(unsigned int neuronIndex = 0; neuronIndex < networkPointer->layers[layerIndex].neurons.size(); neuronIndex++){        
            this->inputWeights.back().push_back(Jimmy::NEURON_WEIGHTS());
            for(unsigned weightIndex = 0; weightIndex < networkPointer->layers[layerIndex][neuronIndex].inputWeights.size(); weightIndex++){              
                this->inputWeights.back().back().inputWeightsRecords.push_back(networkPointer->layers[layerIndex][neuronIndex].inputWeights[weightIndex]);
            }
            this->inputWeights.back().back().selfWeightRecord = networkPointer->layers[layerIndex][neuronIndex].selfWeight;                     
        }
    }
}