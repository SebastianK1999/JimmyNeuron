#include "Critic.hpp"

Jimmy::Critic::Critic(std::reference_wrapper<Jimmy::Net> network){
    this->networkPointer = &network.get();
}

void Jimmy::Critic::reward(){
    this->backProp(1);
}
void Jimmy::Critic::punish(){
    this->backProp(-1);
}

int Jimmy::Critic::logicValue(double& output){
    if(output >= this->trigger){
        return 1;
    }
    return -1;
}


void Jimmy::Critic::backProp(int treat){
    // creating vector of desired values
    std::vector<double> desiredValues;
    for(int i = 0; i < this->watchListNeurons.size(); i++){
        desiredValues.push_back(this->logicValue(this->watchListNeurons[i].get().outValue) * treat);
    }

    // network gradient
    double error = this->networkPointer->lossFunc.run(desiredValues, this->watchListNeurons);
    this->networkPointer->averageError = (this->networkPointer->averageError * 100 + error) / (100 + 1.0);

    // output gradient
    for(int i = 0; i < this->watchListNeurons.size(); i++){
        this->watchListNeurons[i].get().gradient = this->networkPointer->learningRate * (desiredValues[i] - this->watchListNeurons[i].get().outValue)  * this->networkPointer->transFunc.runDervative(this->watchListNeurons[i].get().outValue);
    }

    // hiden gradient
    int i = this->networkPointer->layers.size()-2; // hiden gradient [-2]
    for(int j = 0; j < this->networkPointer->layers[i].neurons.size(); j++){
        double sumGradientWeights = 0.0;
        for(int k = 0; k < this->watchListNeurons.size(); k++){
            sumGradientWeights += this->watchListNeurons[k].get().inputWeights[j] * this->watchListNeurons[k].get().gradient;
        }
        this->networkPointer->layers[i][j].gradient = sumGradientWeights * this->networkPointer->transFunc.runDervative(this->networkPointer->layers[i][j].outValue);
    }
    for(int i = this->networkPointer->layers.size()-3; i > 0; i--){ // hiden gradient [1:-3]
        for(int j = 0; j < this->networkPointer->layers[i].neurons.size(); j++){
            double sumGradientWeights = 0.0;
            for(int k = 0; k < this->networkPointer->layers[i+1].neurons.size(); k++){
                sumGradientWeights += this->networkPointer->layers[i+1][k].inputWeights[j] * this->networkPointer->layers[i+1][k].gradient;
            }
            this->networkPointer->layers[i][j].gradient = sumGradientWeights * this->networkPointer->transFunc.runDervative(this->networkPointer->layers[i][j].outValue);
        }
    }

    // update weights
    for(int j = 0; j < this->watchListNeurons.size(); j++){
        this->watchListNeurons[j].get().updateInputWeights();
    }
    for(unsigned i  = this->networkPointer->layers.size()-2; i > 0; i--){
        for(int j = 0; j < this->networkPointer->layers[i].neurons.size(); j++){
            this->networkPointer->layers[i][j].updateInputWeights();
        }
    }
}

void Jimmy::Critic::chooseHighest(){
    this->watchListNeurons.clear();
    this->watchListNeurons.push_back(this->networkPointer->layers.back()[0]);
    double maximum = this->networkPointer->layers.back()[0].outValue;    
    for(int i = 1; i < this->networkPointer->layers.back().neurons.size(); i++){
        if(maximum > this->networkPointer->layers.back()[i].outValue){
            this->watchListNeurons.clear();
            this->watchListNeurons.push_back(this->networkPointer->layers.back()[i]);
            maximum = this->networkPointer->layers.back()[i].outValue;
        }
    }
}


void Jimmy::Critic::chooseLowest(){
    this->watchListNeurons.clear();
    this->watchListNeurons.push_back(this->networkPointer->layers.back()[0]);
    double minimum = this->networkPointer->layers.back()[0].outValue;    
    for(int i = 1; i < this->networkPointer->layers.back().neurons.size(); i++){
        if(minimum < this->networkPointer->layers.back()[i].outValue){
            this->watchListNeurons.clear();
            this->watchListNeurons.push_back(this->networkPointer->layers.back()[i]);
            minimum = this->networkPointer->layers.back()[i].outValue;
        }
    }
}

void Jimmy::Critic::chooseAll(){   
    this->watchListNeurons.clear();
    for(int i = 0; i < this->networkPointer->layers.back().neurons.size(); i++){\
        this->watchListNeurons.push_back(this->networkPointer->layers.back()[i]);
    }
}
 void Jimmy::Critic::chooseSelected(const std::vector<unsigned int>& outputNeuronIndex){
     this->watchListNeurons.clear();
     for(int vecIndex = 0; vecIndex < outputNeuronIndex.size(); vecIndex++){
         if(outputNeuronIndex[vecIndex] < this->networkPointer->layers.back().neurons.size()){
             this->watchListNeurons.push_back(this->networkPointer->layers.back()[outputNeuronIndex[vecIndex]]);
         }
     }
 }
