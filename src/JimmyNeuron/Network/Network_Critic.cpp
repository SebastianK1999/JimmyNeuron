#include "Network.hpp"

Jimmy::Net::CRITIC::CRITIC(Jimmy::Net* network){
    this->networkPointer = network;
}

void Jimmy::Net::CRITIC::reward(){
    this->backProp(1);
}
void Jimmy::Net::CRITIC::punish(){
    this->backProp(-1);
}
void Jimmy::Net::CRITIC::observe(){
    this->backProp(0);
}

int Jimmy::Net::CRITIC::logicValue(double& output){
    if(output >= this->trigger){
        return 1;
    }
    return -1;// this->downValue;
}

void Jimmy::Net::CRITIC::backProp(int treat){
    double gama = 0.2;
    // network gradient
    //double error = this->networkPointer->lossFunc.run(realValues, this->networkPointer->layers.back());
    //this->networkPointer->averageError = (this->networkPointer->averageError * 100 + error) / (100 + 1.0);

    // output gradient
    for(int i = 0; i < this->watchListNeurons.size(); i++){
        this->watchListNeurons[i].get().gradient = (this->logicValue(this->watchListNeurons[i].get().outValue) * treat - this->watchListNeurons[i].get().outValue);// * this->networkPointer->transFunc.runDervative(this->watchListNeurons[i].get().weightedSum, this->watchListNeurons[i].get().outValue);
    }

    // hiden gradient
    int i = this->networkPointer->layers.size()-2; // hiden gradient [-1]
    for(int j = 0; j < this->networkPointer->layers[i].neurons.size(); j++){
        double sumGradientWeights = 0.0;
        for(int k = 0; k < this->watchListNeurons.size(); k++){
            sumGradientWeights += this->watchListNeurons[k].get().inputWeights[j] * this->watchListNeurons[k].get().gradient;
        }
        this->networkPointer->layers[i][j].gradient = sumGradientWeights * this->networkPointer->transFunc.runDervative(this->networkPointer->layers[i][j].weightedSum, this->networkPointer->layers[i][j].outValue);
    }
    for(int i = this->networkPointer->layers.size()-3; i > 0; i--){ // hiden gradient [1:-2]
        for(int j = 0; j < this->networkPointer->layers[i].neurons.size(); j++){
            double sumGradientWeights = 0.0;
            for(int k = 0; k < this->networkPointer->layers[i+1].neurons.size(); k++){
                sumGradientWeights += this->networkPointer->layers[i+1][k].inputWeights[j] * this->networkPointer->layers[i+1][k].gradient;
            }
            this->networkPointer->layers[i][j].gradient = sumGradientWeights * this->networkPointer->transFunc.runDervative(this->networkPointer->layers[i][j].weightedSum, this->networkPointer->layers[i][j].outValue);
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

void Jimmy::Net::CRITIC::chooseHighest(){
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

void Jimmy::Net::CRITIC::chooseLowest(){
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

void Jimmy::Net::CRITIC::chooseAll(){   
    this->watchListNeurons.clear();
    for(int i = 0; i < this->networkPointer->layers.back().neurons.size(); i++){\
        this->watchListNeurons.push_back(this->networkPointer->layers.back()[i]);
    }
}
 void Jimmy::Net::CRITIC::chooseSelected(const std::vector<unsigned int>& outputNeuronIndex){
     this->watchListNeurons.clear();
     for(int vecIndex = 0; vecIndex < outputNeuronIndex.size(); vecIndex++){
         if(outputNeuronIndex[vecIndex] < this->networkPointer->layers.back().neurons.size()){
             this->watchListNeurons.push_back(this->networkPointer->layers.back()[outputNeuronIndex[vecIndex]]);
         }
     }
 }
