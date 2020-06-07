#include "Critic.hpp"

Jimmy::Critic::Critic(std::reference_wrapper<Jimmy::Net> network){
    this->networkPointer = &network.get();
    this->recPointer = new Jimmy::RECORDS_IO();
}
Jimmy::Critic::~Critic(){
    delete this->recPointer;
}

void Jimmy::Critic::reward(double power){
    this->backProp(power,1);
    this->recPointer->clear();
}
void Jimmy::Critic::punish(double power){
    this->backProp(power,-1);
    this->recPointer->clear();
}

int Jimmy::Critic::logicValue(const double& output){
    if(output >= this->trigger){
        return 1;
    }
    return -1;
}


void Jimmy::Critic::recordMove(){
    bool moveMade = 0;
    int iForContinue;
    for(iForContinue = 0; iForContinue < this->watchListNeurons.size(); iForContinue++){
        if(this->logicValue(this->watchListNeurons[iForContinue].get().outValue) == 1){
            moveMade = 1;
            std::cout <<this->watchListNeurons[iForContinue].get().outValue << " ";
            break;
        }
    }
    if(moveMade){
        this->recPointer->inputs.push_back(std::vector<double>{});
        this->recPointer->outputIndexes.push_back(std::vector<unsigned int>{});

        for(int i = 0; i < this->networkPointer->layers[0].neurons.size(); i++){
            this->recPointer->inputs.back().push_back( this->networkPointer->layers[0][i].outValue);
        }

        for(int i = iForContinue; i < this->watchListNeurons.size(); i++){
            if(this->logicValue(this->watchListNeurons[i].get().outValue) == 1){
                this->recPointer->outputIndexes.back().push_back( this->watchListNeurons[i].get().index);
            }
        }
    }
}

void Jimmy::Critic::record(){
    this->recPointer->inputs.push_back(std::vector<double>{});
    this->recPointer->outputIndexes.push_back(std::vector<unsigned int>{});
    for(int i = 0; i < this->networkPointer->layers[0].neurons.size(); i++){
        this->recPointer->inputs.back().push_back( this->networkPointer->layers[0][i].outValue);
    }
    for(int i = 0; i < this->watchListNeurons.size(); i++){
        this->recPointer->outputIndexes.back().push_back( this->watchListNeurons[i].get().index);
    }
}

void Jimmy::Critic::replay(unsigned int recordIndex){
    if(this->recPointer->inputs.size() == 0){
        this->chooseAll();
    }
    for(int i = 0; i < this->networkPointer->layers[0].neurons.size(); i++){
        this->networkPointer->layers[0][i].outValue = this->recPointer->inputs[recordIndex][i];
    }
    for(int i = 1; i < this->networkPointer->layers.size(); i++){  
        for(int j = 0; j < this->networkPointer->layers[i].neurons.size(); j++){
            this->networkPointer->layers[i][j].weightedSum = this->recPointer->inputWeights[i][j].selfWeightRecord;          
            for(int k = 0; k < this->networkPointer->layers[i][j].inputValuePointers.size(); k++){  
                this->networkPointer->layers[i][j].weightedSum += *this->networkPointer->layers[i][j].inputValuePointers[k] * this->recPointer->inputWeights[i][j].inputWeightsRecords[k];
            }
            this->networkPointer->layers[i][j].outValue = this->networkPointer->transFunc.run(this->networkPointer->layers[i][j].weightedSum);
        }
    }
    this->chooseSelected(this->recPointer->outputIndexes[recordIndex]);
}

void Jimmy::Critic::punishRecords(double power){
    if(this->recPointer->inputs.size() > 0){

        this->recPointer->saveInputWeights(this->networkPointer);
        for(unsigned int recordIndex = 0; recordIndex< this->recPointer->inputs.size(); recordIndex++){
            this->replay(recordIndex);
            this->backPropRecords(power, -1, recordIndex);
        }
    }
    else{

        this->backProp(power, -1);    
    }
    this->recPointer->clear();
}

void Jimmy::Critic::rewardRecords(double power){
    if(this->recPointer->inputs.size() > 0){
        this->recPointer->saveInputWeights(this->networkPointer);
        for(unsigned int recordIndex = 0; recordIndex< this->recPointer->inputs.size(); recordIndex++){
            this->replay(recordIndex);
            this->backPropRecords(power, 1, recordIndex);
        }
    }
    else{
        this->backProp(power, 1);
    }
    this->recPointer->clear();
}


void Jimmy::Critic::backProp(double power, int treat){
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
        this->watchListNeurons[i].get().gradient = this->networkPointer->learningRate * power * (desiredValues[i] - this->watchListNeurons[i].get().outValue)  * this->networkPointer->transFunc.runDervative(this->watchListNeurons[i].get().outValue);
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

void Jimmy::Critic::backPropRecords(double power, int treat, unsigned int recordIndex){
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
        this->watchListNeurons[i].get().gradient = this->networkPointer->learningRate * power * (desiredValues[i] - this->watchListNeurons[i].get().outValue)  * this->networkPointer->transFunc.runDervative(this->watchListNeurons[i].get().outValue);
    }

    // hiden gradient
    int i = this->networkPointer->layers.size()-2; // hiden gradient [-2]
    for(int j = 0; j < this->networkPointer->layers[i].neurons.size(); j++){
        double sumGradientWeights = 0.0;
        for(int k = 0; k < this->watchListNeurons.size(); k++){
            sumGradientWeights += this->recPointer->inputWeights.back()[k].inputWeightsRecords[j] * this->watchListNeurons[k].get().gradient;
        }
        this->networkPointer->layers[i][j].gradient = sumGradientWeights * this->networkPointer->transFunc.runDervative(this->networkPointer->layers[i][j].outValue);
    }
    for(int i = this->networkPointer->layers.size()-3; i > 0; i--){ // hiden gradient [1:-3]
        for(int j = 0; j < this->networkPointer->layers[i].neurons.size(); j++){
            double sumGradientWeights = 0.0;
            for(int k = 0; k < this->networkPointer->layers[i+1].neurons.size(); k++){
                sumGradientWeights += this->recPointer->inputWeights[i+1][k].inputWeightsRecords[j] * this->networkPointer->layers[i+1][k].gradient;
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
void Jimmy::Critic::chooseActive(){   
    this->watchListNeurons.clear();
    for(int i = 0; i < this->networkPointer->layers.back().neurons.size(); i++){
        if(this->logicValue(this->networkPointer->layers.back()[i].outValue) == 1){
            this->watchListNeurons.push_back(this->networkPointer->layers.back()[i]);
        }
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
