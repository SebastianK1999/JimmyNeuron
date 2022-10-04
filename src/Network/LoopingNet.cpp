/*
* MIT License
* 
* Copyright (c) 2022 Sebastian Kwaśniak
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/


#include <utility>
#include <cassert>

#include "JimmyNeuron/Misc/Misc.hpp"

#include "JimmyNeuron/Network/LoopingNet.hpp"


Jimmy::LoopingNet::LoopingNet(const LoopingNet& other ) noexcept 
: inputNeurons(other.inputNeurons)
, hiddenNeurons(other.hiddenNeurons)
, outputNeurons(other.outputNeurons)
, outputDoublePtrs(other.outputDoublePtrs)
, susceptibilityToMutations(other.susceptibilityToMutations)
, transferFunction(other.transferFunction)
, score(other.score)
, outputVector(other.outputVector)
{
    
}

Jimmy::LoopingNet::LoopingNet(LoopingNet&& other) noexcept 
: inputNeurons(std::move(other.inputNeurons))
, hiddenNeurons(std::move(other.hiddenNeurons))
, outputNeurons(std::move(other.outputNeurons))
, outputDoublePtrs(std::move(other.outputDoublePtrs))
, susceptibilityToMutations(std::move(other.susceptibilityToMutations))
, transferFunction(std::move(other.transferFunction))
, score(std::move(other.score))
, outputVector(std::move(other.outputVector))
{
    
}

Jimmy::LoopingNet& Jimmy::LoopingNet::operator=(const LoopingNet& other) noexcept {
    inputNeurons = other.inputNeurons;
    hiddenNeurons = other.hiddenNeurons;
    outputNeurons = other.outputNeurons;
    outputDoublePtrs = other.outputDoublePtrs;
    susceptibilityToMutations = other.susceptibilityToMutations;
    transferFunction = other.transferFunction;
    score = other.score;
    outputVector = other.outputVector;
    
    return *this;
}

Jimmy::LoopingNet& Jimmy::LoopingNet::operator=(LoopingNet&& other) noexcept {
    if(this != &other){
        inputNeurons = std::move(other.inputNeurons);
        hiddenNeurons = std::move(other.hiddenNeurons);
        outputNeurons = std::move(other.outputNeurons);
        outputDoublePtrs = std::move(other.outputDoublePtrs);
        susceptibilityToMutations = std::move(other.susceptibilityToMutations);
        transferFunction = std::move(other.transferFunction);
        score = std::move(other.score);
        outputVector = std::move(other.outputVector);

    }
    return *this;
}

Jimmy::LoopingNet::~LoopingNet(){
    
}

Jimmy::LoopingNet::LoopingNet(size_t inputLayerSize, size_t hiddenLayerSize, size_t outputLayerSize, Jimmy::TransferFunction _transferFunction, double _susceptibilityToMutations)
: inputNeurons(inputLayerSize, Jimmy::LoopingNeuron(inputLayerSize,hiddenLayerSize,outputLayerSize))
, hiddenNeurons(hiddenLayerSize, Jimmy::LoopingNeuron(inputLayerSize,hiddenLayerSize,outputLayerSize))
, outputNeurons(outputLayerSize, Jimmy::LoopingNeuron(inputLayerSize,hiddenLayerSize,outputLayerSize))
, outputDoublePtrs(0)
, susceptibilityToMutations(_susceptibilityToMutations)
, transferFunction(_transferFunction)
, score(0)
, outputVector(outputLayerSize)
{
    
}

void Jimmy::LoopingNet::setInput(const std::vector<double>& inputs){
    assert(inputs.size() == inputNeurons.size());

    for(unsigned int neuronIndex = 0; neuronIndex < inputNeurons.size(); neuronIndex++){
        inputNeurons[neuronIndex].value = inputs[neuronIndex];
    }
}

const std::vector<double>& Jimmy::LoopingNet::getOutput(){
    for(unsigned int neuronIndex = 0; neuronIndex < outputNeurons.size(); neuronIndex++){
        outputVector[neuronIndex] = outputNeurons[neuronIndex].outputValue;
    }
    return outputVector;
}

void Jimmy::LoopingNet::run(){
    neuronsProcess(hiddenNeurons,true, true, true, false, true);
    setOutputValueAsValue(hiddenNeurons);
    neuronsProcess(hiddenNeurons,false, false, false, true, false);
    setOutputValueAsValue(hiddenNeurons);
    neuronsProcess(outputNeurons,true, true, true, true, true);
    setOutputValueAsValue(outputNeurons);
}

void Jimmy::LoopingNet::run(const std::vector<double>& inputs){
    setInput(inputs);
    for(unsigned int neuronIndex = 0; neuronIndex < inputNeurons.size(); neuronIndex++){
        if(inputNeurons[neuronIndex].value != inputs[neuronIndex]){
            std::cout << "warning " << inputNeurons[neuronIndex].value << " " << inputs[neuronIndex] << "\n";
        }
    }
    run();
}

void Jimmy::LoopingNet::mutate(){
    for(std::vector<Jimmy::LoopingNeuron>::iterator iter = inputNeurons.begin(); iter != inputNeurons.end(); iter = std::next(iter)){
        Jimmy::LoopingNeuron& neuron = *iter;
        neuron.bias += Jimmy::Misc::rand().decimal(-susceptibilityToMutations, susceptibilityToMutations);
        for(std::vector<double>::iterator iterVal = neuron.inputNeuronWeights.begin(); iterVal != neuron.inputNeuronWeights.end(); iterVal = std::next(iterVal)){
            double& value = *iterVal;
            value += Jimmy::Misc::rand().decimal(-susceptibilityToMutations, susceptibilityToMutations);
        }
        for(std::vector<double>::iterator iterVal = neuron.hiddenNeuronWeights.begin(); iterVal != neuron.hiddenNeuronWeights.end(); iterVal = std::next(iterVal)){
            double& value = *iterVal;
            value += Jimmy::Misc::rand().decimal(-susceptibilityToMutations, susceptibilityToMutations);
        }
        for(std::vector<double>::iterator iterVal = neuron.outputNeuronWeights.begin(); iterVal != neuron.outputNeuronWeights.end(); iterVal = std::next(iterVal)){
            double& value = *iterVal;
            value += Jimmy::Misc::rand().decimal(-susceptibilityToMutations, susceptibilityToMutations);
        }
    }
    for(std::vector<Jimmy::LoopingNeuron>::iterator iter = hiddenNeurons.begin(); iter != hiddenNeurons.end(); iter = std::next(iter)){
        Jimmy::LoopingNeuron& neuron = *iter;
        neuron.bias += Jimmy::Misc::rand().decimal(-susceptibilityToMutations, susceptibilityToMutations);
        for(std::vector<double>::iterator iterVal = neuron.inputNeuronWeights.begin(); iterVal != neuron.inputNeuronWeights.end(); iterVal = std::next(iterVal)){
            double& value = *iterVal;
            value += Jimmy::Misc::rand().decimal(-susceptibilityToMutations, susceptibilityToMutations);
        }
        for(std::vector<double>::iterator iterVal = neuron.hiddenNeuronWeights.begin(); iterVal != neuron.hiddenNeuronWeights.end(); iterVal = std::next(iterVal)){
            double& value = *iterVal;
            value += Jimmy::Misc::rand().decimal(-susceptibilityToMutations, susceptibilityToMutations);
        }
        for(std::vector<double>::iterator iterVal = neuron.outputNeuronWeights.begin(); iterVal != neuron.outputNeuronWeights.end(); iterVal = std::next(iterVal)){
            double& value = *iterVal;
            value += Jimmy::Misc::rand().decimal(-susceptibilityToMutations, susceptibilityToMutations);
        }
    }
    for(std::vector<Jimmy::LoopingNeuron>::iterator iter = outputNeurons.begin(); iter != outputNeurons.end(); iter = std::next(iter)){
        Jimmy::LoopingNeuron& neuron = *iter;
        neuron.bias += Jimmy::Misc::rand().decimal(-susceptibilityToMutations, susceptibilityToMutations);
        for(std::vector<double>::iterator iterVal = neuron.inputNeuronWeights.begin(); iterVal != neuron.inputNeuronWeights.end(); iterVal = std::next(iterVal)){
            double& value = *iterVal;
            value += Jimmy::Misc::rand().decimal(-susceptibilityToMutations, susceptibilityToMutations);
        }
        for(std::vector<double>::iterator iterVal = neuron.hiddenNeuronWeights.begin(); iterVal != neuron.hiddenNeuronWeights.end(); iterVal = std::next(iterVal)){
            double& value = *iterVal;
            value += Jimmy::Misc::rand().decimal(-susceptibilityToMutations, susceptibilityToMutations);
        }
        for(std::vector<double>::iterator iterVal = neuron.outputNeuronWeights.begin(); iterVal != neuron.outputNeuronWeights.end(); iterVal = std::next(iterVal)){
            double& value = *iterVal;
            value += Jimmy::Misc::rand().decimal(-susceptibilityToMutations, susceptibilityToMutations);
        }
    }
}

void Jimmy::LoopingNet::randomize(double minWeight, double maxWeight){
    
    for(std::vector<Jimmy::LoopingNeuron>::iterator iter = inputNeurons.begin(); iter != inputNeurons.end(); iter = std::next(iter)){
        Jimmy::LoopingNeuron& neuron = *iter;
        neuron.bias = Jimmy::Misc::rand().decimal(minWeight,maxWeight);
        for(std::vector<double>::iterator iterVal = neuron.inputNeuronWeights.begin(); iterVal != neuron.inputNeuronWeights.end(); iterVal = std::next(iterVal)){
            double& value = *iterVal;
            value = Jimmy::Misc::rand().decimal(minWeight, maxWeight);
        }
        for(std::vector<double>::iterator iterVal = neuron.hiddenNeuronWeights.begin(); iterVal != neuron.hiddenNeuronWeights.end(); iterVal = std::next(iterVal)){
            double& value = *iterVal;
            value = Jimmy::Misc::rand().decimal(minWeight, maxWeight);
        }
        for(std::vector<double>::iterator iterVal = neuron.outputNeuronWeights.begin(); iterVal != neuron.outputNeuronWeights.end(); iterVal = std::next(iterVal)){
            double& value = *iterVal;
            value = Jimmy::Misc::rand().decimal(minWeight, maxWeight);
        }
    }
    for(std::vector<Jimmy::LoopingNeuron>::iterator iter = hiddenNeurons.begin(); iter != hiddenNeurons.end(); iter = std::next(iter)){
        Jimmy::LoopingNeuron& neuron = *iter;
        neuron.bias = Jimmy::Misc::rand().decimal(minWeight,maxWeight);
        for(std::vector<double>::iterator iterVal = neuron.inputNeuronWeights.begin(); iterVal != neuron.inputNeuronWeights.end(); iterVal = std::next(iterVal)){
            double& value = *iterVal;
            value = Jimmy::Misc::rand().decimal(minWeight, maxWeight);
        }
        for(std::vector<double>::iterator iterVal = neuron.hiddenNeuronWeights.begin(); iterVal != neuron.hiddenNeuronWeights.end(); iterVal = std::next(iterVal)){
            double& value = *iterVal;
            value = Jimmy::Misc::rand().decimal(minWeight, maxWeight);
        }
        for(std::vector<double>::iterator iterVal = neuron.outputNeuronWeights.begin(); iterVal != neuron.outputNeuronWeights.end(); iterVal = std::next(iterVal)){
            double& value = *iterVal;
            value = Jimmy::Misc::rand().decimal(minWeight, maxWeight);
        }
    }
    for(std::vector<Jimmy::LoopingNeuron>::iterator iter = outputNeurons.begin(); iter != outputNeurons.end(); iter = std::next(iter)){
        Jimmy::LoopingNeuron& neuron = *iter;
        neuron.bias = Jimmy::Misc::rand().decimal(minWeight,maxWeight);
        for(std::vector<double>::iterator iterVal = neuron.inputNeuronWeights.begin(); iterVal != neuron.inputNeuronWeights.end(); iterVal = std::next(iterVal)){
            double& value = *iterVal;
            value = Jimmy::Misc::rand().decimal(minWeight, maxWeight);
        }
        for(std::vector<double>::iterator iterVal = neuron.hiddenNeuronWeights.begin(); iterVal != neuron.hiddenNeuronWeights.end(); iterVal = std::next(iterVal)){
            double& value = *iterVal;
            value = Jimmy::Misc::rand().decimal(minWeight, maxWeight);
        }
        for(std::vector<double>::iterator iterVal = neuron.outputNeuronWeights.begin(); iterVal != neuron.outputNeuronWeights.end(); iterVal = std::next(iterVal)){
            double& value = *iterVal;
            value = Jimmy::Misc::rand().decimal(minWeight, maxWeight);
        }
    }
}

void Jimmy::LoopingNet::evolveFrom(const Jimmy::LoopingNet& networkParent){
    (*this) = networkParent;
    mutate();
}

void Jimmy::LoopingNet::evolveFrom(const Jimmy::LoopingNet& networkFemale, const Jimmy::LoopingNet& networkMale){
    // inherit neuron sizes
    if((*this).inputNeurons.size() != networkFemale.inputNeurons.size() || (*this).inputNeurons.size() != networkFemale.inputNeurons.size()){
        const bool inheritLengthFromMale = Jimmy::Misc::rand().binary();
        if(inheritLengthFromMale){
            inputNeurons.resize(networkMale.inputNeurons.size());
        }
        else{
            inputNeurons.resize(networkFemale.inputNeurons.size());
        }
    }
    if((*this).hiddenNeurons.size() != networkFemale.hiddenNeurons.size() || (*this).hiddenNeurons.size() != networkFemale.hiddenNeurons.size()){
        const bool inheritLengthFromMale = Jimmy::Misc::rand().binary();
        if(inheritLengthFromMale){
            hiddenNeurons.resize(networkMale.hiddenNeurons.size());
        }
        else{
            hiddenNeurons.resize(networkFemale.hiddenNeurons.size());
        }
    }
    if((*this).outputNeurons.size() != networkFemale.outputNeurons.size() || (*this).outputNeurons.size() != networkFemale.outputNeurons.size()){
        const bool inheritLengthFromMale = Jimmy::Misc::rand().binary();
        if(inheritLengthFromMale){
            outputNeurons.resize(networkMale.outputNeurons.size());
        }
        else{
            outputNeurons.resize(networkFemale.outputNeurons.size());
        }
        outputVector.resize(outputNeurons.size());
    }

    // // inherit each weight (input)
    for(unsigned int neuronIndex = 0; neuronIndex < inputNeurons.size(); neuronIndex++){
        Jimmy::LoopingNeuron& currentNeuron = inputNeurons[neuronIndex];
        const Jimmy::LoopingNeuron& maleNeuron = networkMale.inputNeurons[neuronIndex];
        const Jimmy::LoopingNeuron& femaleNeuron = networkFemale.inputNeurons[neuronIndex];
        currentNeuron.inputNeuronWeights.resize(inputNeurons.size());
        currentNeuron.hiddenNeuronWeights.resize(hiddenNeurons.size());
        currentNeuron.outputNeuronWeights.resize(outputNeurons.size());
        currentNeuron.bias = (Jimmy::Misc::rand().binary() ? networkFemale.inputNeurons[neuronIndex].bias : networkMale.inputNeurons[neuronIndex].bias) + Jimmy::Misc::rand().decimal(-susceptibilityToMutations, susceptibilityToMutations);

        const std::vector<double>* shortWeightVectorPtr;
        const std::vector<double>* longWeightVectorPtr;
        /* binary way */
        // find shorter and longer weight vectors (input)
        if(maleNeuron.inputNeuronWeights.size() > femaleNeuron.inputNeuronWeights.size()){
            shortWeightVectorPtr = &(femaleNeuron.inputNeuronWeights);
            longWeightVectorPtr = &(maleNeuron.inputNeuronWeights);
        }
        else{
            shortWeightVectorPtr = &(maleNeuron.inputNeuronWeights);
            longWeightVectorPtr = &(femaleNeuron.inputNeuronWeights);
        }
        for(unsigned int weightIndex = 0; weightIndex < shortWeightVectorPtr->size(); weightIndex++){
            currentNeuron.inputNeuronWeights[weightIndex] = (Jimmy::Misc::rand().binary() ? maleNeuron.inputNeuronWeights[weightIndex] : femaleNeuron.inputNeuronWeights[weightIndex] ) + Jimmy::Misc::rand().decimal(-susceptibilityToMutations, susceptibilityToMutations);
        }
        for(unsigned int weightIndex = shortWeightVectorPtr->size(); weightIndex < longWeightVectorPtr->size(); weightIndex++){
            currentNeuron.inputNeuronWeights[weightIndex] = (*longWeightVectorPtr)[weightIndex] + Jimmy::Misc::rand().decimal(-susceptibilityToMutations, susceptibilityToMutations);
        }

        if(maleNeuron.hiddenNeuronWeights.size() > femaleNeuron.hiddenNeuronWeights.size()){
            shortWeightVectorPtr = &(femaleNeuron.hiddenNeuronWeights);
            longWeightVectorPtr = &(maleNeuron.hiddenNeuronWeights);
        }
        else{
            shortWeightVectorPtr = &(maleNeuron.hiddenNeuronWeights);
            longWeightVectorPtr = &(femaleNeuron.hiddenNeuronWeights);
        }
        for(unsigned int weightIndex = 0; weightIndex < shortWeightVectorPtr->size(); weightIndex++){
            currentNeuron.hiddenNeuronWeights[weightIndex] = (Jimmy::Misc::rand().binary() ? maleNeuron.hiddenNeuronWeights[weightIndex] : femaleNeuron.hiddenNeuronWeights[weightIndex] ) + Jimmy::Misc::rand().decimal(-susceptibilityToMutations, susceptibilityToMutations);
        }
        for(unsigned int weightIndex = shortWeightVectorPtr->size(); weightIndex < longWeightVectorPtr->size(); weightIndex++){
            currentNeuron.hiddenNeuronWeights[weightIndex] = (*longWeightVectorPtr)[weightIndex] + Jimmy::Misc::rand().decimal(-susceptibilityToMutations, susceptibilityToMutations);
        }

        if(maleNeuron.outputNeuronWeights.size() > femaleNeuron.outputNeuronWeights.size()){
            shortWeightVectorPtr = &(femaleNeuron.outputNeuronWeights);
            longWeightVectorPtr = &(maleNeuron.outputNeuronWeights);
        }
        else{
            shortWeightVectorPtr = &(maleNeuron.outputNeuronWeights);
            longWeightVectorPtr = &(femaleNeuron.outputNeuronWeights);
        }
        for(unsigned int weightIndex = 0; weightIndex < shortWeightVectorPtr->size(); weightIndex++){
            currentNeuron.outputNeuronWeights[weightIndex] = (Jimmy::Misc::rand().binary() ? maleNeuron.outputNeuronWeights[weightIndex] : femaleNeuron.outputNeuronWeights[weightIndex] ) + Jimmy::Misc::rand().decimal(-susceptibilityToMutations, susceptibilityToMutations);
        }
        for(unsigned int weightIndex = shortWeightVectorPtr->size(); weightIndex < longWeightVectorPtr->size(); weightIndex++){
            currentNeuron.outputNeuronWeights[weightIndex] = (*longWeightVectorPtr)[weightIndex] + Jimmy::Misc::rand().decimal(-susceptibilityToMutations, susceptibilityToMutations);
        }

    }

    // // inherit each weight (hidden)
    for(unsigned int neuronIndex = 0; neuronIndex < hiddenNeurons.size(); neuronIndex++){
        Jimmy::LoopingNeuron& currentNeuron = hiddenNeurons[neuronIndex];
        const Jimmy::LoopingNeuron& maleNeuron = networkMale.hiddenNeurons[neuronIndex];
        const Jimmy::LoopingNeuron& femaleNeuron = networkFemale.hiddenNeurons[neuronIndex];
        currentNeuron.inputNeuronWeights.resize(inputNeurons.size());
        currentNeuron.hiddenNeuronWeights.resize(hiddenNeurons.size());
        currentNeuron.outputNeuronWeights.resize(outputNeurons.size());
        currentNeuron.bias = (Jimmy::Misc::rand().binary() ? networkFemale.hiddenNeurons[neuronIndex].bias : networkMale.hiddenNeurons[neuronIndex].bias) + Jimmy::Misc::rand().decimal(-susceptibilityToMutations, susceptibilityToMutations);

        const std::vector<double>* shortWeightVectorPtr;
        const std::vector<double>* longWeightVectorPtr;
        /* binary way */
        // find shorter and longer weight vectors (input)
        if(maleNeuron.inputNeuronWeights.size() > femaleNeuron.inputNeuronWeights.size()){
            shortWeightVectorPtr = &(femaleNeuron.inputNeuronWeights);
            longWeightVectorPtr = &(maleNeuron.inputNeuronWeights);
        }
        else{
            shortWeightVectorPtr = &(maleNeuron.inputNeuronWeights);
            longWeightVectorPtr = &(femaleNeuron.inputNeuronWeights);
        }
        for(unsigned int weightIndex = 0; weightIndex < shortWeightVectorPtr->size(); weightIndex++){
            currentNeuron.inputNeuronWeights[weightIndex] = (Jimmy::Misc::rand().binary() ? maleNeuron.inputNeuronWeights[weightIndex] : femaleNeuron.inputNeuronWeights[weightIndex] ) + Jimmy::Misc::rand().decimal(-susceptibilityToMutations, susceptibilityToMutations);
        }
        for(unsigned int weightIndex = shortWeightVectorPtr->size(); weightIndex < longWeightVectorPtr->size(); weightIndex++){
            currentNeuron.inputNeuronWeights[weightIndex] = (*longWeightVectorPtr)[weightIndex] + Jimmy::Misc::rand().decimal(-susceptibilityToMutations, susceptibilityToMutations);
        }
        // find shorter and longer weight vectors (hidden)
        if(maleNeuron.hiddenNeuronWeights.size() > femaleNeuron.hiddenNeuronWeights.size()){
            shortWeightVectorPtr = &(femaleNeuron.hiddenNeuronWeights);
            longWeightVectorPtr = &(maleNeuron.hiddenNeuronWeights);
        }
        else{
            shortWeightVectorPtr = &(maleNeuron.hiddenNeuronWeights);
            longWeightVectorPtr = &(femaleNeuron.hiddenNeuronWeights);
        }
        for(unsigned int weightIndex = 0; weightIndex < shortWeightVectorPtr->size(); weightIndex++){
            currentNeuron.hiddenNeuronWeights[weightIndex] = (Jimmy::Misc::rand().binary() ? maleNeuron.hiddenNeuronWeights[weightIndex] : femaleNeuron.hiddenNeuronWeights[weightIndex] ) + Jimmy::Misc::rand().decimal(-susceptibilityToMutations, susceptibilityToMutations);
        }
        for(unsigned int weightIndex = shortWeightVectorPtr->size(); weightIndex < longWeightVectorPtr->size(); weightIndex++){
            currentNeuron.hiddenNeuronWeights[weightIndex] = (*longWeightVectorPtr)[weightIndex] + Jimmy::Misc::rand().decimal(-susceptibilityToMutations, susceptibilityToMutations);
        }
        // find shorter and longer weight vectors (output)
        if(maleNeuron.outputNeuronWeights.size() > femaleNeuron.outputNeuronWeights.size()){
            shortWeightVectorPtr = &(femaleNeuron.outputNeuronWeights);
            longWeightVectorPtr = &(maleNeuron.outputNeuronWeights);
        }
        else{
            shortWeightVectorPtr = &(maleNeuron.outputNeuronWeights);
            longWeightVectorPtr = &(femaleNeuron.outputNeuronWeights);
        }
        for(unsigned int weightIndex = 0; weightIndex < shortWeightVectorPtr->size(); weightIndex++){
            currentNeuron.outputNeuronWeights[weightIndex] = (Jimmy::Misc::rand().binary() ? maleNeuron.outputNeuronWeights[weightIndex] : femaleNeuron.outputNeuronWeights[weightIndex] ) + Jimmy::Misc::rand().decimal(-susceptibilityToMutations, susceptibilityToMutations);
        }
        for(unsigned int weightIndex = shortWeightVectorPtr->size(); weightIndex < longWeightVectorPtr->size(); weightIndex++){
            currentNeuron.outputNeuronWeights[weightIndex] = (*longWeightVectorPtr)[weightIndex] + Jimmy::Misc::rand().decimal(-susceptibilityToMutations, susceptibilityToMutations);
        }

    }

    // // inherit each weight (output)
    for(unsigned int neuronIndex = 0; neuronIndex < outputNeurons.size(); neuronIndex++){
        Jimmy::LoopingNeuron& currentNeuron = outputNeurons[neuronIndex];
        const Jimmy::LoopingNeuron& maleNeuron = networkMale.outputNeurons[neuronIndex];
        const Jimmy::LoopingNeuron& femaleNeuron = networkFemale.outputNeurons[neuronIndex];
        currentNeuron.inputNeuronWeights.resize(inputNeurons.size());
        currentNeuron.hiddenNeuronWeights.resize(hiddenNeurons.size());
        currentNeuron.outputNeuronWeights.resize(outputNeurons.size());
        currentNeuron.bias = (Jimmy::Misc::rand().binary() ? networkFemale.outputNeurons[neuronIndex].bias : networkMale.outputNeurons[neuronIndex].bias) + Jimmy::Misc::rand().decimal(-susceptibilityToMutations, susceptibilityToMutations);

        const std::vector<double>* shortWeightVectorPtr;
        const std::vector<double>* longWeightVectorPtr;
        /* binary way */
        // find shorter and longer weight vectors (input)
        if(maleNeuron.inputNeuronWeights.size() > femaleNeuron.inputNeuronWeights.size()){
            shortWeightVectorPtr = &(femaleNeuron.inputNeuronWeights);
            longWeightVectorPtr = &(maleNeuron.inputNeuronWeights);
        }
        else{
            shortWeightVectorPtr = &(maleNeuron.inputNeuronWeights);
            longWeightVectorPtr = &(femaleNeuron.inputNeuronWeights);
        }
        for(unsigned int weightIndex = 0; weightIndex < shortWeightVectorPtr->size(); weightIndex++){
            currentNeuron.inputNeuronWeights[weightIndex] = (Jimmy::Misc::rand().binary() ? maleNeuron.inputNeuronWeights[weightIndex] : femaleNeuron.inputNeuronWeights[weightIndex] ) + Jimmy::Misc::rand().decimal(-susceptibilityToMutations, susceptibilityToMutations);
        }
        for(unsigned int weightIndex = shortWeightVectorPtr->size(); weightIndex < longWeightVectorPtr->size(); weightIndex++){
            currentNeuron.inputNeuronWeights[weightIndex] = (*longWeightVectorPtr)[weightIndex] + Jimmy::Misc::rand().decimal(-susceptibilityToMutations, susceptibilityToMutations);
        }
        // find shorter and longer weight vectors (hidden)
        if(maleNeuron.hiddenNeuronWeights.size() > femaleNeuron.hiddenNeuronWeights.size()){
            shortWeightVectorPtr = &(femaleNeuron.hiddenNeuronWeights);
            longWeightVectorPtr = &(maleNeuron.hiddenNeuronWeights);
        }
        else{
            shortWeightVectorPtr = &(maleNeuron.hiddenNeuronWeights);
            longWeightVectorPtr = &(femaleNeuron.hiddenNeuronWeights);
        }
        for(unsigned int weightIndex = 0; weightIndex < shortWeightVectorPtr->size(); weightIndex++){
            currentNeuron.hiddenNeuronWeights[weightIndex] = (Jimmy::Misc::rand().binary() ? maleNeuron.hiddenNeuronWeights[weightIndex] : femaleNeuron.hiddenNeuronWeights[weightIndex] ) + Jimmy::Misc::rand().decimal(-susceptibilityToMutations, susceptibilityToMutations);
        }
        for(unsigned int weightIndex = shortWeightVectorPtr->size(); weightIndex < longWeightVectorPtr->size(); weightIndex++){
            currentNeuron.hiddenNeuronWeights[weightIndex] = (*longWeightVectorPtr)[weightIndex] + Jimmy::Misc::rand().decimal(-susceptibilityToMutations, susceptibilityToMutations);
        }
        // find shorter and longer weight vectors (output)
        if(maleNeuron.outputNeuronWeights.size() > femaleNeuron.outputNeuronWeights.size()){
            shortWeightVectorPtr = &(femaleNeuron.outputNeuronWeights);
            longWeightVectorPtr = &(maleNeuron.outputNeuronWeights);
        }
        else{
            shortWeightVectorPtr = &(maleNeuron.outputNeuronWeights);
            longWeightVectorPtr = &(femaleNeuron.outputNeuronWeights);
        }
        for(unsigned int weightIndex = 0; weightIndex < shortWeightVectorPtr->size(); weightIndex++){
            currentNeuron.outputNeuronWeights[weightIndex] = (Jimmy::Misc::rand().binary() ? maleNeuron.outputNeuronWeights[weightIndex] : femaleNeuron.outputNeuronWeights[weightIndex] ) + Jimmy::Misc::rand().decimal(-susceptibilityToMutations, susceptibilityToMutations);
        }
        for(unsigned int weightIndex = shortWeightVectorPtr->size(); weightIndex < longWeightVectorPtr->size(); weightIndex++){
            currentNeuron.outputNeuronWeights[weightIndex] = (*longWeightVectorPtr)[weightIndex] + Jimmy::Misc::rand().decimal(-susceptibilityToMutations, susceptibilityToMutations);
        }

    }

    /* floating way */ 
    // # FOR LOOPS
    // double minWeight;
    // double maxWeight;
    // minWeight = std::min(networkFemale.inputNeurons[neuronIndex].inputNeuronWeights[weightIndex], networkMale.inputNeurons[neuronIndex].inputNeuronWeights[weightIndex]);
    // maxWeight = std::max(networkFemale.inputNeurons[neuronIndex].inputNeuronWeights[weightIndex], networkMale.inputNeurons[neuronIndex].inputNeuronWeights[weightIndex]);
    // inputNeurons[neuronIndex].inputNeuronWeights[weightIndex] = Jimmy::Misc::rand().decimal(minWeight - susceptibilityToMutations, maxWeight + susceptibilityToMutations);
    // minWeight = std::min(networkFemale.hiddenNeurons[neuronIndex].inputNeuronWeights[weightIndex], networkMale.hiddenNeurons[neuronIndex].inputNeuronWeights[weightIndex]);
    // maxWeight = std::max(networkFemale.hiddenNeurons[neuronIndex].inputNeuronWeights[weightIndex], networkMale.hiddenNeurons[neuronIndex].inputNeuronWeights[weightIndex]);
    // hiddenNeurons[neuronIndex].inputNeuronWeights[weightIndex] = Jimmy::Misc::rand().decimal(minWeight - susceptibilityToMutations, maxWeight + susceptibilityToMutations);
    // minWeight = std::min(networkFemale.outputNeurons[neuronIndex].inputNeuronWeights[weightIndex], networkMale.outputNeurons[neuronIndex].inputNeuronWeights[weightIndex]);
    // maxWeight = std::max(networkFemale.outputNeurons[neuronIndex].inputNeuronWeights[weightIndex], networkMale.outputNeurons[neuronIndex].inputNeuronWeights[weightIndex]);
    // outputNeurons[neuronIndex].inputNeuronWeights[weightIndex] = Jimmy::Misc::rand().decimal(minWeight - susceptibilityToMutations, maxWeight + susceptibilityToMutations);
    /* other floating way (same effect) */ 
    // inputNeurons[neuronIndex].inputNeuronWeights[weightIndex] = Jimmy::Misc::rand().decimal(networkFemale.inputNeurons[neuronIndex].inputNeuronWeights[weightIndex], networkMale.inputNeurons[neuronIndex].inputNeuronWeights[weightIndex] ) + Jimmy::Misc::rand().decimal(-susceptibilityToMutations, susceptibilityToMutations);
    // hiddenNeurons[neuronIndex].inputNeuronWeights[weightIndex] = Jimmy::Misc::rand().decimal(networkFemale.hiddenNeurons[neuronIndex].inputNeuronWeights[weightIndex], networkMale.hiddenNeurons[neuronIndex].inputNeuronWeights[weightIndex] ) + Jimmy::Misc::rand().decimal(-susceptibilityToMutations, susceptibilityToMutations);
    // outputNeurons[neuronIndex].inputNeuronWeights[weightIndex] = Jimmy::Misc::rand().decimal(networkFemale.outputNeurons[neuronIndex].inputNeuronWeights[weightIndex], networkMale.outputNeurons[neuronIndex].inputNeuronWeights[weightIndex] ) + Jimmy::Misc::rand().decimal(-susceptibilityToMutations, susceptibilityToMutations);

}

void Jimmy::LoopingNet::evolveFrom(const std::vector<Jimmy::LoopingNet*>& networkOrgyPointers){
    // TODO
}

void Jimmy::LoopingNet::addInputNeurons(size_t n){
    size_t oldSize = inputNeurons.size();
    size_t newSize = oldSize + n;
    inputNeurons.reserve(newSize);
    for(unsigned long int i = 0; i < n; i++){
        inputNeurons.emplace_back(Jimmy::LoopingNeuron(oldSize, hiddenNeurons.size(), outputNeurons.size()));
    }
    for(std::vector<Jimmy::LoopingNeuron>::iterator iter = inputNeurons.begin(); iter != inputNeurons.end(); iter = std::next(iter)){
        Jimmy::LoopingNeuron& neuron = *iter;
        neuron.inputNeuronWeights.reserve(newSize);
        for(unsigned long int i = 0; i < n; i++){
            neuron.inputNeuronWeights.emplace_back(0);
        }
    }
    for(std::vector<Jimmy::LoopingNeuron>::iterator iter = hiddenNeurons.begin(); iter != hiddenNeurons.end(); iter = std::next(iter)){
        Jimmy::LoopingNeuron& neuron = *iter;
        neuron.inputNeuronWeights.reserve(newSize);
        for(unsigned long int i = 0; i < n; i++){
            neuron.inputNeuronWeights.emplace_back(0);
        }
    }
    for(std::vector<Jimmy::LoopingNeuron>::iterator iter = outputNeurons.begin(); iter != outputNeurons.end(); iter = std::next(iter)){
        Jimmy::LoopingNeuron& neuron = *iter;
        neuron.inputNeuronWeights.reserve(newSize);
        for(unsigned long int i = 0; i < n; i++){
            neuron.inputNeuronWeights.emplace_back(0);
        }
    }
}

void Jimmy::LoopingNet::addHiddenNeurons(size_t n){
    size_t oldSize = hiddenNeurons.size();
    size_t newSize = oldSize + n;
    hiddenNeurons.reserve(newSize);
    for(unsigned long int i = 0; i < n; i++){
        hiddenNeurons.emplace_back(Jimmy::LoopingNeuron(inputNeurons.size(), oldSize, outputNeurons.size()));
    }
    for(std::vector<Jimmy::LoopingNeuron>::iterator iter = inputNeurons.begin(); iter != inputNeurons.end(); iter = std::next(iter)){
        Jimmy::LoopingNeuron& neuron = *iter;
        neuron.hiddenNeuronWeights.reserve(newSize);
        for(unsigned long int i = 0; i < n; i++){
            neuron.hiddenNeuronWeights.emplace_back(0);
        }
    }
    for(std::vector<Jimmy::LoopingNeuron>::iterator iter = hiddenNeurons.begin(); iter != hiddenNeurons.end(); iter = std::next(iter)){
        Jimmy::LoopingNeuron& neuron = *iter;
        neuron.hiddenNeuronWeights.reserve(newSize);
        for(unsigned long int i = 0; i < n; i++){
            neuron.hiddenNeuronWeights.emplace_back(0);
        }
    }
    for(std::vector<Jimmy::LoopingNeuron>::iterator iter = outputNeurons.begin(); iter != outputNeurons.end(); iter = std::next(iter)){
        Jimmy::LoopingNeuron& neuron = *iter;
        neuron.hiddenNeuronWeights.reserve(newSize);
        for(unsigned long int i = 0; i < n; i++){
            neuron.hiddenNeuronWeights.emplace_back(0);
        }
    }
}

void Jimmy::LoopingNet::addOutputNeurons(size_t n){
    size_t oldSize = outputNeurons.size();
    size_t newSize = oldSize + n;
    outputNeurons.reserve(newSize);
    outputVector.resize(newSize);
    for(unsigned long int i = 0; i < n; i++){
        outputNeurons.emplace_back(Jimmy::LoopingNeuron(inputNeurons.size(), hiddenNeurons.size(), oldSize));
    }
    for(std::vector<Jimmy::LoopingNeuron>::iterator iter = inputNeurons.begin(); iter != inputNeurons.end(); iter = std::next(iter)){
        Jimmy::LoopingNeuron& neuron = *iter;
        neuron.outputNeuronWeights.reserve(newSize);
        for(unsigned long int i = 0; i < n; i++){
            neuron.outputNeuronWeights.emplace_back(0);
        }
    }
    for(std::vector<Jimmy::LoopingNeuron>::iterator iter = hiddenNeurons.begin(); iter != hiddenNeurons.end(); iter = std::next(iter)){
        Jimmy::LoopingNeuron& neuron = *iter;
        neuron.outputNeuronWeights.reserve(newSize);
        for(unsigned long int i = 0; i < n; i++){
            neuron.outputNeuronWeights.emplace_back(0);
        }
    }
    for(std::vector<Jimmy::LoopingNeuron>::iterator iter = outputNeurons.begin(); iter != outputNeurons.end(); iter = std::next(iter)){
        Jimmy::LoopingNeuron& neuron = *iter;
        neuron.outputNeuronWeights.reserve(newSize);
        for(unsigned long int i = 0; i < n; i++){
            neuron.outputNeuronWeights.emplace_back(0);
        }
    }
}

void Jimmy::LoopingNet::clearValues(){
    for(Jimmy::LoopingNeuron& neuron : inputNeurons){
        neuron.value = 0.0;
        neuron.outputValue = 0.0;
    }
    for(Jimmy::LoopingNeuron& neuron : hiddenNeurons){
        neuron.value = 0.0;
        neuron.outputValue = 0.0;
    }
    for(Jimmy::LoopingNeuron& neuron : outputNeurons){
        neuron.value = 0.0;
        neuron.outputValue = 0.0;
    }
}

void Jimmy::LoopingNet::clearOutputValues(){
    for(Jimmy::LoopingNeuron& neuron : outputNeurons){
        neuron.value = 0.0;
        neuron.outputValue = 0.0;
    }
}


void Jimmy::LoopingNet::setOutputValueAsValue(std::vector<Jimmy::LoopingNeuron>& neurons){
    for(std::vector<Jimmy::LoopingNeuron>::iterator iter = neurons.begin(); iter != neurons.end(); iter = std::next(iter)){
        Jimmy::LoopingNeuron& neuron = *iter;
        neuron.value = neuron.outputValue;
    }
}

void Jimmy::LoopingNet::neuronsProcess(std::vector<Jimmy::LoopingNeuron>& processingNeurons, const bool resetSum, const bool addBias, const bool processInputs, const bool processHidden, const bool processOutput){
    for(std::vector<Jimmy::LoopingNeuron>::iterator iter = processingNeurons.begin(); iter != processingNeurons.end(); iter = std::next(iter)){
        Jimmy::LoopingNeuron& neuron = *iter;
        if(resetSum){
            neuron.weightedSum = 0;
        }
        if(addBias){
            neuron.weightedSum += neuron.bias;
        }
        if(processInputs){ 
            for(unsigned int i = 0; i < inputNeurons.size(); i++){
                // std::cout << "inp : " << inputNeurons[i].value << " * " << neuron.inputNeuronWeights[i] << " = " << inputNeurons[i].value * neuron.inputNeuronWeights[i] << " " << transferFunction.run(INFINITY) << "\n";
                neuron.weightedSum += inputNeurons[i].value * neuron.inputNeuronWeights[i];
            }
        }
        if(processHidden){
            for(unsigned int i = 0; i < hiddenNeurons.size(); i++){
                // std::cout << "hid : " << hiddenNeurons[i].value << " * " << neuron.hiddenNeuronWeights[i] << " = " << hiddenNeurons[i].value * neuron.hiddenNeuronWeights[i] << "\n";

                neuron.weightedSum += hiddenNeurons[i].value * neuron.hiddenNeuronWeights[i];
            } 
        }
        if(processOutput){
            for(unsigned int i = 0; i < outputNeurons.size(); i++){
                // std::cout << "out : " << outputNeurons[i].value << " * " << neuron.outputNeuronWeights[i] << " = " << outputNeurons[i].value * neuron.outputNeuronWeights[i] << "\n";

                neuron.weightedSum += outputNeurons[i].value * neuron.outputNeuronWeights[i];
            }
        }
        // std::cout << neuron.weightedSum << " " << transferFunction.run(neuron.weightedSum) << "\n";
        neuron.outputValue = transferFunction.run(neuron.weightedSum);
    }   
}