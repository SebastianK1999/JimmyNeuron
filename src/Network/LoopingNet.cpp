/*
* MIT License
* 
* Copyright (c) 2023 Sebastian Kwaśniak
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
: inputSize(other.inputSize)
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
: inputSize(std::move(other.inputSize))
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
    inputSize = other.inputSize;
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
        inputSize = std::move(other.inputSize);
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
: inputSize(inputLayerSize)
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
    // neuronsProcess<false, false, false, true, false, false>(hiddenNeurons, inputs);
    // setOutputValueAsValue(hiddenNeurons);
    for (int i = 0; i < 2; i++)
    {
        neuronsProcess<true, true, true, true, true, true>(hiddenNeurons, inputs);
        setOutputValueAsValue(hiddenNeurons);
    }
    neuronsProcess<true, true, true, true, true, true>(outputNeurons, inputs);
    setOutputValueAsValue(outputNeurons);
    neuronsProcess<true, true, true, true, true, true>(memoryNeurons, inputs);
    setOutputValueAsValue(memoryNeurons);
}

void Jimmy::LoopingNet::mutate(){
    #define RANDOM_MUTATION() \ 
        Jimmy::Misc::rand().decimal(-susceptibilityToMutations, susceptibilityToMutations)

    #define FOR_WEIGHT(WEIGHT_TYPE) \
        for(auto& weight : neuron.WEIGHT_TYPE){ \
            weight += RANDOM_MUTATION(); \
        } 

    #define FOR_NEURON_GROUP(NEURON_GROUP) \
        for(Jimmy::LoopingNeuron& neuron : NEURON_GROUP){ \
            neuron.bias += RANDOM_MUTATION(); \
            FOR_WEIGHT(inputNeuronWeights) \
            FOR_WEIGHT(hiddenNeuronWeights) \
            FOR_WEIGHT(outputNeuronWeights) \
            FOR_WEIGHT(memoryNeuronWeights) \
        }

    // FOR_NEURON_GROUP(inputNeurons)
    FOR_NEURON_GROUP(hiddenNeurons)
    FOR_NEURON_GROUP(outputNeurons)
    FOR_NEURON_GROUP(memoryNeurons)

    #undef RANDOM_MUTATION
    #undef FOR_WEIGHT
    #undef FOR_NEURON_GROUP
}

void Jimmy::LoopingNet::randomize(double minWeight, double maxWeight)
{
    
    #define RANDOMIZE() \ 
        Jimmy::Misc::rand().decimal(minWeight, maxWeight)

    #define FOR_WEIGHT(WEIGHT_TYPE) \
        for(auto& weight : neuron.WEIGHT_TYPE){ \
            weight = RANDOMIZE(); \
        } 

    #define FOR_NEURON_GROUP(NEURON_GROUP) \
        for(Jimmy::LoopingNeuron& neuron : NEURON_GROUP){ \
            neuron.bias = RANDOMIZE(); \
            FOR_WEIGHT(inputNeuronWeights) \
            FOR_WEIGHT(hiddenNeuronWeights) \
            FOR_WEIGHT(outputNeuronWeights) \
            FOR_WEIGHT(memoryNeuronWeights) \
        }

    // FOR_NEURON_GROUP(inputNeurons)
    FOR_NEURON_GROUP(hiddenNeurons)
    FOR_NEURON_GROUP(outputNeurons)
    FOR_NEURON_GROUP(memoryNeurons)


    #undef RANDOMIZE
    #undef FOR_WEIGHT
    #undef FOR_NEURON_GROUP
}

void Jimmy::LoopingNet::evolveFrom(const Jimmy::LoopingNet& networkParent){
    (*this) = networkParent;
    mutate();
}

void Jimmy::LoopingNet::evolveFrom(const Jimmy::LoopingNet& networkFemale, const Jimmy::LoopingNet& networkMale){
    #define INHERIT_SIZE(GROUP_TYPE) \
        if((*this).GROUP_TYPE.size() != networkFemale.GROUP_TYPE.size() || (*this).GROUP_TYPE.size() != networkFemale.GROUP_TYPE.size()){ \
            const bool inheritLengthFromMale = Jimmy::Misc::rand().binary(); \
            if(inheritLengthFromMale){ \
                GROUP_TYPE.resize(networkMale.GROUP_TYPE.size()); \
            } \
            else{ \
                GROUP_TYPE.resize(networkFemale.GROUP_TYPE.size()); \
            } \
        }

    INHERIT_SIZE(hiddenNeurons)
    INHERIT_SIZE(outputNeurons)
    INHERIT_SIZE(memoryNeurons)    

    if((*this).outputNeurons.size() != networkFemale.outputNeurons.size() || (*this).outputNeurons.size() != networkFemale.outputNeurons.size()){
        outputVector.resize(outputNeurons.size()); // TODO !
    }

    #define MIX_NEURON_WEIGHT(WEIGHT_TYPE) \
        if(maleNeuron.WEIGHT_TYPE.size() > femaleNeuron.WEIGHT_TYPE.size()){ \
            shortWeightVectorPtr = &(femaleNeuron.WEIGHT_TYPE); \
            longWeightVectorPtr = &(maleNeuron.WEIGHT_TYPE); \
        } \
        else{ \
            shortWeightVectorPtr = &(maleNeuron.WEIGHT_TYPE); \
            longWeightVectorPtr = &(femaleNeuron.WEIGHT_TYPE); \
        } \
        for(unsigned int weightIndex = 0; weightIndex < shortWeightVectorPtr->size(); weightIndex++){ \
            currentNeuron.WEIGHT_TYPE[weightIndex] = Jimmy::Misc::rand().decimal(maleNeuron.WEIGHT_TYPE[weightIndex], femaleNeuron.WEIGHT_TYPE[weightIndex]) + Jimmy::Misc::rand().decimal(-susceptibilityToMutations, susceptibilityToMutations); \
            /*currentNeuron.WEIGHT_TYPE[weightIndex] = (Jimmy::Misc::rand().binary() ? networkFemale.GROUP_TYPE[neuronIndex].WEIGHT_TYPE[weightIndex] : networkMale.GROUP_TYPE[neuronIndex].WEIGHT_TYPE[weightIndex]) + Jimmy::Misc::rand().decimal(-susceptibilityToMutations, susceptibilityToMutations);*/ \
        } \
        for(unsigned int weightIndex = shortWeightVectorPtr->size(); weightIndex < longWeightVectorPtr->size(); weightIndex++){ \
            currentNeuron.WEIGHT_TYPE[weightIndex] = (*longWeightVectorPtr)[weightIndex] + Jimmy::Misc::rand().decimal(-susceptibilityToMutations, susceptibilityToMutations); \
        }\
        
    #define MIX_NEURON_GROUPS(GROUP_TYPE) \
        for(unsigned int neuronIndex = 0; neuronIndex < GROUP_TYPE.size(); neuronIndex++){ \
            Jimmy::LoopingNeuron& currentNeuron = GROUP_TYPE[neuronIndex]; \
            const Jimmy::LoopingNeuron& maleNeuron = networkMale.GROUP_TYPE[neuronIndex]; \
            const Jimmy::LoopingNeuron& femaleNeuron = networkFemale.GROUP_TYPE[neuronIndex]; \
            currentNeuron.inputNeuronWeights.resize(inputSize); \
            currentNeuron.hiddenNeuronWeights.resize(hiddenNeurons.size()); \
            currentNeuron.outputNeuronWeights.resize(outputNeurons.size()); \
            currentNeuron.memoryNeuronWeights.resize(memoryNeurons.size()); \
            currentNeuron.bias = Jimmy::Misc::rand().decimal(networkFemale.GROUP_TYPE[neuronIndex].bias, networkMale.GROUP_TYPE[neuronIndex].bias) + Jimmy::Misc::rand().decimal(-susceptibilityToMutations, susceptibilityToMutations); \
            /*currentNeuron.bias = (Jimmy::Misc::rand().binary() ? networkFemale.GROUP_TYPE[neuronIndex].bias : networkMale.GROUP_TYPE[neuronIndex].bias) + Jimmy::Misc::rand().decimal(-susceptibilityToMutations, susceptibilityToMutations);*/ \
            const std::vector<double>* shortWeightVectorPtr; \
            const std::vector<double>* longWeightVectorPtr; \
            MIX_NEURON_WEIGHT(inputNeuronWeights) \
            MIX_NEURON_WEIGHT(hiddenNeuronWeights) \
            MIX_NEURON_WEIGHT(outputNeuronWeights) \
            MIX_NEURON_WEIGHT(memoryNeuronWeights) \
        }

    MIX_NEURON_GROUPS(hiddenNeurons)
    MIX_NEURON_GROUPS(outputNeurons)
    MIX_NEURON_GROUPS(memoryNeurons)

    #undef INHERIT_SIZE
    #undef MIX_NEURON_WEIGHT
    #undef MIX_NEURON_GROUPS
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

#define ADD_NEURON_WEIGHTS(NEURON_WEIGHT_NAME, NEURON_GROUP_VEC) \
    for(Jimmy::LoopingNeuron& neuron : NEURON_GROUP_VEC) \
    { \
        neuron.NEURON_WEIGHT_NAME.reserve(newSize); \
        for(unsigned long int i = 0; i < n; i++){ \
            neuron.NEURON_WEIGHT_NAME.emplace_back(0); \
        } \
    }

void Jimmy::LoopingNet::addInputNeurons(size_t n){
    size_t newSize = inputSize + n;
    inputSize = newSize;
    ADD_NEURON_WEIGHTS(inputNeuronWeights, hiddenNeurons);
    ADD_NEURON_WEIGHTS(inputNeuronWeights, outputNeurons);
    ADD_NEURON_WEIGHTS(inputNeuronWeights, memoryNeurons);
}

#define ADD_NEURONS_FUNCTION_IMPLEMENTATION(F_NAME, NEURON_GROUP_VEC, NEURON_WEIGHT_NAME, IS_OUTPUT) \
    void Jimmy::LoopingNet::F_NAME(size_t n){ \
        size_t newSize = NEURON_GROUP_VEC.size() + n; \
        NEURON_GROUP_VEC.reserve(newSize); \
        if constexpr (IS_OUTPUT) outputVector.resize(newSize); \
        size_t hiddenSize = hiddenNeurons.size(); \
        size_t outputSize = outputNeurons.size(); \
        size_t memorySize = memoryNeurons.size(); \
        for(unsigned long int i = 0; i < n; i++){ \
            NEURON_GROUP_VEC.emplace_back(Jimmy::LoopingNeuron(inputSize, hiddenSize, outputSize, memorySize)); \
        } \
        ADD_NEURON_WEIGHTS(NEURON_WEIGHT_NAME, hiddenNeurons); \
        ADD_NEURON_WEIGHTS(NEURON_WEIGHT_NAME, outputNeurons); \
        ADD_NEURON_WEIGHTS(NEURON_WEIGHT_NAME, memoryNeurons); \
    }

ADD_NEURONS_FUNCTION_IMPLEMENTATION(addHiddenNeurons, hiddenNeurons, hiddenNeuronWeights, false)
ADD_NEURONS_FUNCTION_IMPLEMENTATION(addOutputNeurons, outputNeurons, outputNeuronWeights, true)
ADD_NEURONS_FUNCTION_IMPLEMENTATION(addMemoryNeurons, memoryNeurons, memoryNeuronWeights, false)

#undef ADD_NEURON_WEIGHTS
#undef ADD_NEURONS_FUNCTION_IMPLEMENTATION

void Jimmy::LoopingNet::clearValues(){
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

void Jimmy::LoopingNet::clearMemoryValues(){
    for(Jimmy::LoopingNeuron& neuron : memoryNeurons){
        neuron.value = 0.0;
        neuron.outputValue = 0.0;
    }
}

void Jimmy::LoopingNet::saveToFile(const std::filesystem::path& path) const {
    std::ofstream ofs(path, std::ios::binary);
    size_t inputLayerSize;
    size_t hiddenLayerSize;
    size_t outputLayerSize;
    size_t memoryLayerSize;
    inputLayerSize = inputSize;
    hiddenLayerSize = hiddenNeurons.size();
    outputLayerSize = outputNeurons.size();
    memoryLayerSize = memoryNeurons.size();
    ofs.write(reinterpret_cast<const char*>(& inputLayerSize), sizeof(size_t));
    ofs.write(reinterpret_cast<const char*>(& hiddenLayerSize), sizeof(size_t));
    ofs.write(reinterpret_cast<const char*>(& outputLayerSize), sizeof(size_t));
    ofs.write(reinterpret_cast<const char*>(& memoryLayerSize), sizeof(size_t));
    // for(const Jimmy::LoopingNeuron& neuron: inputNeurons){
    //     saveNeuron(neuron, ofs);
    // }
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

void Jimmy::LoopingNet::loadFromFile(const std::filesystem::path& path){
    std::ifstream ifs(path, std::ios::binary);
    size_t inputLayerSize;
    size_t hiddenLayerSize;
    size_t outputLayerSize;
    size_t memoryLayerSize;
    ifs.read(reinterpret_cast<char *>(&inputLayerSize), sizeof(size_t));
    ifs.read(reinterpret_cast<char *>(&hiddenLayerSize), sizeof(size_t));
    ifs.read(reinterpret_cast<char *>(&outputLayerSize), sizeof(size_t));
    ifs.read(reinterpret_cast<char *>(&memoryLayerSize), sizeof(size_t));
    inputSize = inputLayerSize;
    hiddenNeurons.resize(hiddenLayerSize);
    outputNeurons.resize(outputLayerSize);
    memoryNeurons.resize(memoryLayerSize);
    // for(Jimmy::LoopingNeuron& neuron: inputNeurons){
    //     loadNeuron(neuron, ifs);
    // }
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

void Jimmy::LoopingNet::saveNeuron(const Jimmy::LoopingNeuron &neuron, std::ofstream &ofs) const
{
    ofs.write(reinterpret_cast<const char*>(&(neuron.bias)), sizeof(double));
    ofs.write(reinterpret_cast<const char *>(neuron.inputNeuronWeights.data() ),inputSize  * sizeof(double));
    ofs.write(reinterpret_cast<const char *>(neuron.hiddenNeuronWeights.data()),hiddenNeurons.size() * sizeof(double));
    ofs.write(reinterpret_cast<const char *>(neuron.outputNeuronWeights.data()),outputNeurons.size() * sizeof(double));
    ofs.write(reinterpret_cast<const char *>(neuron.memoryNeuronWeights.data()),memoryNeurons.size() * sizeof(double));
}

void Jimmy::LoopingNet::loadNeuron(Jimmy::LoopingNeuron& neuron, std::ifstream& ifs){
    neuron.inputNeuronWeights.resize(inputSize);
    neuron.hiddenNeuronWeights.resize(hiddenNeurons.size());
    neuron.outputNeuronWeights.resize(outputNeurons.size());
    neuron.memoryNeuronWeights.resize(memoryNeurons.size());
    ifs.read(reinterpret_cast<char *>(&(neuron.bias)), sizeof(double));
    ifs.read(reinterpret_cast<char *>(neuron.inputNeuronWeights.data() ),inputSize  * sizeof(double));
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

