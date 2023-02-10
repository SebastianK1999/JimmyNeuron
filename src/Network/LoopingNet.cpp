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
#include <iostream>
#include <filesystem>

#include "JimmyNeuron/Misc/Misc.hpp"

#include "JimmyNeuron/Network/LoopingNet.hpp"


Jimmy::LoopingNet::LoopingNet(const LoopingNet& other ) noexcept 
: inputNeurons(other.inputNeurons)
, hiddenNeurons(other.hiddenNeurons)
, outputNeurons(other.outputNeurons)
, memoryNeurons(other.memoryNeurons)
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
, memoryNeurons(std::move(other.memoryNeurons))
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
    memoryNeurons = other.memoryNeurons;
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
        memoryNeurons = std::move(other.memoryNeurons);
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

Jimmy::LoopingNet::LoopingNet(size_t inputLayerSize, size_t hiddenLayerSize, size_t outputLayerSize, size_t memoryLayerSize, Jimmy::TransferFunction _transferFunction, double _susceptibilityToMutations)
: inputNeurons(inputLayerSize, Jimmy::LoopingNeuron(inputLayerSize,hiddenLayerSize,outputLayerSize,memoryLayerSize))
, hiddenNeurons(hiddenLayerSize, Jimmy::LoopingNeuron(inputLayerSize,hiddenLayerSize,outputLayerSize,memoryLayerSize))
, outputNeurons(outputLayerSize, Jimmy::LoopingNeuron(inputLayerSize,hiddenLayerSize,outputLayerSize,memoryLayerSize))
, memoryNeurons(memoryLayerSize, Jimmy::LoopingNeuron(inputLayerSize,hiddenLayerSize,outputLayerSize,memoryLayerSize))
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
}

void Jimmy::LoopingNet::run(const std::vector<double>& inputs){
    neuronsProcess<true, true, true, false, true, true>(hiddenNeurons, inputs);
    setOutputValueAsValue(hiddenNeurons);
    neuronsProcess<false, false, false, true, false, false>(hiddenNeurons, inputs);
    setOutputValueAsValue(hiddenNeurons);
    neuronsProcess<true, true, true, true, true, true>(outputNeurons, inputs);
    setOutputValueAsValue(outputNeurons);
    neuronsProcess<true, true, true, true, true, true>(memoryNeurons, inputs);
    setOutputValueAsValue(memoryNeurons);
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
        for(std::vector<double>::iterator iterVal = neuron.memoryNeuronWeights.begin(); iterVal != neuron.memoryNeuronWeights.end(); iterVal = std::next(iterVal)){
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
        for(std::vector<double>::iterator iterVal = neuron.memoryNeuronWeights.begin(); iterVal != neuron.memoryNeuronWeights.end(); iterVal = std::next(iterVal)){
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
        for(std::vector<double>::iterator iterVal = neuron.memoryNeuronWeights.begin(); iterVal != neuron.memoryNeuronWeights.end(); iterVal = std::next(iterVal)){
            double& value = *iterVal;
            value += Jimmy::Misc::rand().decimal(-susceptibilityToMutations, susceptibilityToMutations);
        }
    }
    for(std::vector<Jimmy::LoopingNeuron>::iterator iter = memoryNeurons.begin(); iter != memoryNeurons.end(); iter = std::next(iter)){
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
        for(std::vector<double>::iterator iterVal = neuron.memoryNeuronWeights.begin(); iterVal != neuron.memoryNeuronWeights.end(); iterVal = std::next(iterVal)){
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
        for(std::vector<double>::iterator iterVal = neuron.memoryNeuronWeights.begin(); iterVal != neuron.memoryNeuronWeights.end(); iterVal = std::next(iterVal)){
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
        for(std::vector<double>::iterator iterVal = neuron.memoryNeuronWeights.begin(); iterVal != neuron.memoryNeuronWeights.end(); iterVal = std::next(iterVal)){
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
        for(std::vector<double>::iterator iterVal = neuron.memoryNeuronWeights.begin(); iterVal != neuron.memoryNeuronWeights.end(); iterVal = std::next(iterVal)){
            double& value = *iterVal;
            value = Jimmy::Misc::rand().decimal(minWeight, maxWeight);
        }
    }
    for(std::vector<Jimmy::LoopingNeuron>::iterator iter = memoryNeurons.begin(); iter != memoryNeurons.end(); iter = std::next(iter)){
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
        for(std::vector<double>::iterator iterVal = neuron.memoryNeuronWeights.begin(); iterVal != neuron.memoryNeuronWeights.end(); iterVal = std::next(iterVal)){
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
    if((*this).memoryNeurons.size() != networkFemale.memoryNeurons.size() || (*this).memoryNeurons.size() != networkFemale.memoryNeurons.size()){
        const bool inheritLengthFromMale = Jimmy::Misc::rand().binary();
        if(inheritLengthFromMale){
            memoryNeurons.resize(networkMale.memoryNeurons.size());
        }
        else{
            memoryNeurons.resize(networkFemale.memoryNeurons.size());
        }
    }

    // // inherit each weight (input)
    for(unsigned int neuronIndex = 0; neuronIndex < inputNeurons.size(); neuronIndex++){
        Jimmy::LoopingNeuron& currentNeuron = inputNeurons[neuronIndex];
        const Jimmy::LoopingNeuron& maleNeuron = networkMale.inputNeurons[neuronIndex];
        const Jimmy::LoopingNeuron& femaleNeuron = networkFemale.inputNeurons[neuronIndex];
        currentNeuron.inputNeuronWeights.resize(inputNeurons.size());
        currentNeuron.hiddenNeuronWeights.resize(hiddenNeurons.size());
        currentNeuron.outputNeuronWeights.resize(outputNeurons.size());
        currentNeuron.memoryNeuronWeights.resize(memoryNeurons.size());
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

        if(maleNeuron.memoryNeuronWeights.size() > femaleNeuron.memoryNeuronWeights.size()){
            shortWeightVectorPtr = &(femaleNeuron.memoryNeuronWeights);
            longWeightVectorPtr = &(maleNeuron.memoryNeuronWeights);
        }
        else{
            shortWeightVectorPtr = &(maleNeuron.memoryNeuronWeights);
            longWeightVectorPtr = &(femaleNeuron.memoryNeuronWeights);
        }
        for(unsigned int weightIndex = 0; weightIndex < shortWeightVectorPtr->size(); weightIndex++){
            currentNeuron.memoryNeuronWeights[weightIndex] = (Jimmy::Misc::rand().binary() ? maleNeuron.memoryNeuronWeights[weightIndex] : femaleNeuron.memoryNeuronWeights[weightIndex] ) + Jimmy::Misc::rand().decimal(-susceptibilityToMutations, susceptibilityToMutations);
        }
        for(unsigned int weightIndex = shortWeightVectorPtr->size(); weightIndex < longWeightVectorPtr->size(); weightIndex++){
            currentNeuron.memoryNeuronWeights[weightIndex] = (*longWeightVectorPtr)[weightIndex] + Jimmy::Misc::rand().decimal(-susceptibilityToMutations, susceptibilityToMutations);
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
        currentNeuron.memoryNeuronWeights.resize(memoryNeurons.size());
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

        if(maleNeuron.memoryNeuronWeights.size() > femaleNeuron.memoryNeuronWeights.size()){
            shortWeightVectorPtr = &(femaleNeuron.memoryNeuronWeights);
            longWeightVectorPtr = &(maleNeuron.memoryNeuronWeights);
        }
        else{
            shortWeightVectorPtr = &(maleNeuron.memoryNeuronWeights);
            longWeightVectorPtr = &(femaleNeuron.memoryNeuronWeights);
        }
        for(unsigned int weightIndex = 0; weightIndex < shortWeightVectorPtr->size(); weightIndex++){
            currentNeuron.memoryNeuronWeights[weightIndex] = (Jimmy::Misc::rand().binary() ? maleNeuron.memoryNeuronWeights[weightIndex] : femaleNeuron.memoryNeuronWeights[weightIndex] ) + Jimmy::Misc::rand().decimal(-susceptibilityToMutations, susceptibilityToMutations);
        }
        for(unsigned int weightIndex = shortWeightVectorPtr->size(); weightIndex < longWeightVectorPtr->size(); weightIndex++){
            currentNeuron.memoryNeuronWeights[weightIndex] = (*longWeightVectorPtr)[weightIndex] + Jimmy::Misc::rand().decimal(-susceptibilityToMutations, susceptibilityToMutations);
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
        currentNeuron.memoryNeuronWeights.resize(memoryNeurons.size());
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

        if(maleNeuron.memoryNeuronWeights.size() > femaleNeuron.memoryNeuronWeights.size()){
            shortWeightVectorPtr = &(femaleNeuron.memoryNeuronWeights);
            longWeightVectorPtr = &(maleNeuron.memoryNeuronWeights);
        }
        else{
            shortWeightVectorPtr = &(maleNeuron.memoryNeuronWeights);
            longWeightVectorPtr = &(femaleNeuron.memoryNeuronWeights);
        }
        for(unsigned int weightIndex = 0; weightIndex < shortWeightVectorPtr->size(); weightIndex++){
            currentNeuron.memoryNeuronWeights[weightIndex] = (Jimmy::Misc::rand().binary() ? maleNeuron.memoryNeuronWeights[weightIndex] : femaleNeuron.memoryNeuronWeights[weightIndex] ) + Jimmy::Misc::rand().decimal(-susceptibilityToMutations, susceptibilityToMutations);
        }
        for(unsigned int weightIndex = shortWeightVectorPtr->size(); weightIndex < longWeightVectorPtr->size(); weightIndex++){
            currentNeuron.memoryNeuronWeights[weightIndex] = (*longWeightVectorPtr)[weightIndex] + Jimmy::Misc::rand().decimal(-susceptibilityToMutations, susceptibilityToMutations);
        }
           
    }

    // // inherit each weight (memory)
    for(unsigned int neuronIndex = 0; neuronIndex < memoryNeurons.size(); neuronIndex++){
        Jimmy::LoopingNeuron& currentNeuron = memoryNeurons[neuronIndex];
        const Jimmy::LoopingNeuron& maleNeuron = networkMale.memoryNeurons[neuronIndex];
        const Jimmy::LoopingNeuron& femaleNeuron = networkFemale.memoryNeurons[neuronIndex];
        currentNeuron.inputNeuronWeights.resize(inputNeurons.size());
        currentNeuron.hiddenNeuronWeights.resize(hiddenNeurons.size());
        currentNeuron.outputNeuronWeights.resize(outputNeurons.size());
        currentNeuron.memoryNeuronWeights.resize(memoryNeurons.size());
        currentNeuron.bias = (Jimmy::Misc::rand().binary() ? networkFemale.memoryNeurons[neuronIndex].bias : networkMale.memoryNeurons[neuronIndex].bias) + Jimmy::Misc::rand().decimal(-susceptibilityToMutations, susceptibilityToMutations);

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

        if(maleNeuron.memoryNeuronWeights.size() > femaleNeuron.memoryNeuronWeights.size()){
            shortWeightVectorPtr = &(femaleNeuron.memoryNeuronWeights);
            longWeightVectorPtr = &(maleNeuron.memoryNeuronWeights);
        }
        else{
            shortWeightVectorPtr = &(maleNeuron.memoryNeuronWeights);
            longWeightVectorPtr = &(femaleNeuron.memoryNeuronWeights);
        }
        for(unsigned int weightIndex = 0; weightIndex < shortWeightVectorPtr->size(); weightIndex++){
            currentNeuron.memoryNeuronWeights[weightIndex] = (Jimmy::Misc::rand().binary() ? maleNeuron.memoryNeuronWeights[weightIndex] : femaleNeuron.memoryNeuronWeights[weightIndex] ) + Jimmy::Misc::rand().decimal(-susceptibilityToMutations, susceptibilityToMutations);
        }
        for(unsigned int weightIndex = shortWeightVectorPtr->size(); weightIndex < longWeightVectorPtr->size(); weightIndex++){
            currentNeuron.memoryNeuronWeights[weightIndex] = (*longWeightVectorPtr)[weightIndex] + Jimmy::Misc::rand().decimal(-susceptibilityToMutations, susceptibilityToMutations);
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

void Jimmy::LoopingNet::addNeurons(size_t nI, size_t nH, size_t nO, size_t nM){
    addInputNeurons(nI);
    addHiddenNeurons(nH);
    addOutputNeurons(nO);
    addMemoryNeurons(nM);
}

void Jimmy::LoopingNet::addInputNeurons(size_t n){
    size_t oldSize = inputNeurons.size();
    size_t newSize = oldSize + n;
    inputNeurons.reserve(newSize);
    for(unsigned long int i = 0; i < n; i++){
        inputNeurons.emplace_back(Jimmy::LoopingNeuron(oldSize, hiddenNeurons.size(), outputNeurons.size(), memoryNeurons.size()));
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
    for(std::vector<Jimmy::LoopingNeuron>::iterator iter = memoryNeurons.begin(); iter != memoryNeurons.end(); iter = std::next(iter)){
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
        hiddenNeurons.emplace_back(Jimmy::LoopingNeuron(inputNeurons.size(), oldSize, outputNeurons.size(), memoryNeurons.size()));
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
    for(std::vector<Jimmy::LoopingNeuron>::iterator iter = memoryNeurons.begin(); iter != memoryNeurons.end(); iter = std::next(iter)){
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
        outputNeurons.emplace_back(Jimmy::LoopingNeuron(inputNeurons.size(), hiddenNeurons.size(), oldSize, memoryNeurons.size()));
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
    for(std::vector<Jimmy::LoopingNeuron>::iterator iter = memoryNeurons.begin(); iter != memoryNeurons.end(); iter = std::next(iter)){
        Jimmy::LoopingNeuron& neuron = *iter;
        neuron.outputNeuronWeights.reserve(newSize);
        for(unsigned long int i = 0; i < n; i++){
            neuron.outputNeuronWeights.emplace_back(0);
        }
    }
}

void Jimmy::LoopingNet::addMemoryNeurons(size_t n){
    size_t oldSize = memoryNeurons.size();
    size_t newSize = oldSize + n;
    memoryNeurons.reserve(newSize);
    for(unsigned long int i = 0; i < n; i++){
        memoryNeurons.emplace_back(Jimmy::LoopingNeuron(inputNeurons.size(), hiddenNeurons.size(), outputNeurons.size(), oldSize));
    }
    for(std::vector<Jimmy::LoopingNeuron>::iterator iter = inputNeurons.begin(); iter != inputNeurons.end(); iter = std::next(iter)){
        Jimmy::LoopingNeuron& neuron = *iter;
        neuron.memoryNeuronWeights.reserve(newSize);
        for(unsigned long int i = 0; i < n; i++){
            neuron.memoryNeuronWeights.emplace_back(0);
        }
    }
    for(std::vector<Jimmy::LoopingNeuron>::iterator iter = hiddenNeurons.begin(); iter != hiddenNeurons.end(); iter = std::next(iter)){
        Jimmy::LoopingNeuron& neuron = *iter;
        neuron.memoryNeuronWeights.reserve(newSize);
        for(unsigned long int i = 0; i < n; i++){
            neuron.memoryNeuronWeights.emplace_back(0);
        }
    }
    for(std::vector<Jimmy::LoopingNeuron>::iterator iter = outputNeurons.begin(); iter != outputNeurons.end(); iter = std::next(iter)){
        Jimmy::LoopingNeuron& neuron = *iter;
        neuron.memoryNeuronWeights.reserve(newSize);
        for(unsigned long int i = 0; i < n; i++){
            neuron.memoryNeuronWeights.emplace_back(0);
        }
    }
    for(std::vector<Jimmy::LoopingNeuron>::iterator iter = memoryNeurons.begin(); iter != memoryNeurons.end(); iter = std::next(iter)){
        Jimmy::LoopingNeuron& neuron = *iter;
        neuron.memoryNeuronWeights.reserve(newSize);
        for(unsigned long int i = 0; i < n; i++){
            neuron.memoryNeuronWeights.emplace_back(0);
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
    for(Jimmy::LoopingNeuron& neuron : memoryNeurons){
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

void Jimmy::LoopingNet::saveToFile(std::string path) const {
    std::ofstream ofs(path, std::ios::binary);
    size_t inputLayerSize;
    size_t hiddenLayerSize;
    size_t outputLayerSize;
    size_t memoryLayerSize;
    inputLayerSize = inputNeurons.size();
    hiddenLayerSize = hiddenNeurons.size();
    outputLayerSize = outputNeurons.size();
    memoryLayerSize = memoryNeurons.size();
    ofs.write(reinterpret_cast<const char*>(& inputLayerSize), sizeof(size_t));
    ofs.write(reinterpret_cast<const char*>(& hiddenLayerSize), sizeof(size_t));
    ofs.write(reinterpret_cast<const char*>(& outputLayerSize), sizeof(size_t));
    ofs.write(reinterpret_cast<const char*>(& memoryLayerSize), sizeof(size_t));
    for(const Jimmy::LoopingNeuron& neuron: inputNeurons){
        saveNeuron(neuron, ofs);
    }
    for(const Jimmy::LoopingNeuron& neuron: hiddenNeurons){
        saveNeuron(neuron, ofs);
    }
    for(const Jimmy::LoopingNeuron& neuron: outputNeurons){
        saveNeuron(neuron, ofs);
    }
    for(const Jimmy::LoopingNeuron& neuron: memoryNeurons){
        saveNeuron(neuron, ofs);
    }
    ofs.close();
}

void Jimmy::LoopingNet::loadFromFile(std::string path){
    std::ifstream ifs(path, std::ios::binary);
    size_t inputLayerSize;
    size_t hiddenLayerSize;
    size_t outputLayerSize;
    size_t memoryLayerSize;
    ifs.read(reinterpret_cast<char *>(&inputLayerSize), sizeof(size_t));
    ifs.read(reinterpret_cast<char *>(&hiddenLayerSize), sizeof(size_t));
    ifs.read(reinterpret_cast<char *>(&outputLayerSize), sizeof(size_t));
    ifs.read(reinterpret_cast<char *>(&memoryLayerSize), sizeof(size_t));
    inputNeurons.resize(inputLayerSize);
    hiddenNeurons.resize(hiddenLayerSize);
    outputNeurons.resize(outputLayerSize);
    memoryNeurons.resize(memoryLayerSize);
    for(Jimmy::LoopingNeuron& neuron: inputNeurons){
        loadNeuron(neuron, ifs);
    }
    for(Jimmy::LoopingNeuron& neuron: hiddenNeurons){
        loadNeuron(neuron, ifs);
    }
    for(Jimmy::LoopingNeuron& neuron: outputNeurons){
        loadNeuron(neuron, ifs);
    }
    for(Jimmy::LoopingNeuron& neuron: memoryNeurons){
        loadNeuron(neuron, ifs);
    }
    ifs.close();
}

void Jimmy::LoopingNet::saveNeuron(const Jimmy::LoopingNeuron& neuron, std::ofstream& ofs) const {
    ofs.write(reinterpret_cast<const char*>(&(neuron.bias)), sizeof(double));
    ofs.write(reinterpret_cast<const char *>(neuron.inputNeuronWeights.data() ),inputNeurons.size()  * sizeof(double));
    ofs.write(reinterpret_cast<const char *>(neuron.hiddenNeuronWeights.data()),hiddenNeurons.size() * sizeof(double));
    ofs.write(reinterpret_cast<const char *>(neuron.outputNeuronWeights.data()),outputNeurons.size() * sizeof(double));
    ofs.write(reinterpret_cast<const char *>(neuron.memoryNeuronWeights.data()),memoryNeurons.size() * sizeof(double));
}

void Jimmy::LoopingNet::loadNeuron(Jimmy::LoopingNeuron& neuron, std::ifstream& ifs){
    neuron.inputNeuronWeights.resize(inputNeurons.size());
    neuron.hiddenNeuronWeights.resize(hiddenNeurons.size());
    neuron.outputNeuronWeights.resize(outputNeurons.size());
    neuron.memoryNeuronWeights.resize(memoryNeurons.size());
    ifs.read(reinterpret_cast<char *>(&(neuron.bias)), sizeof(double));
    ifs.read(reinterpret_cast<char *>(neuron.inputNeuronWeights.data() ),inputNeurons.size()  * sizeof(double));
    ifs.read(reinterpret_cast<char *>(neuron.hiddenNeuronWeights.data()),hiddenNeurons.size() * sizeof(double));
    ifs.read(reinterpret_cast<char *>(neuron.outputNeuronWeights.data()),outputNeurons.size() * sizeof(double));
    ifs.read(reinterpret_cast<char *>(neuron.memoryNeuronWeights.data()),memoryNeurons.size() * sizeof(double));
}


void Jimmy::LoopingNet::setOutputValueAsValue(std::vector<Jimmy::LoopingNeuron>& neurons){
    for(std::vector<Jimmy::LoopingNeuron>::iterator iter = neurons.begin(); iter != neurons.end(); iter = std::next(iter)){
        Jimmy::LoopingNeuron& neuron = *iter;
        neuron.value = neuron.outputValue;
    }
}

