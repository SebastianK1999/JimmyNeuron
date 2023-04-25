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

#include "JimmyNeuron/Network/LoopingNeuron.hpp"


Jimmy::LoopingNeuron::LoopingNeuron(const LoopingNeuron& other ) noexcept
: inputNeuronWeights(other.inputNeuronWeights)
, hiddenNeuronWeights(other.hiddenNeuronWeights)
, outputNeuronWeights(other.outputNeuronWeights)
, memoryNeuronWeights(other.memoryNeuronWeights)
, bias(other.bias)
, value(other.value)
, outputValue(other.outputValue)
, weightedSum(other.weightedSum)
{
    
}

Jimmy::LoopingNeuron::LoopingNeuron(LoopingNeuron&& other) noexcept
: inputNeuronWeights(std::move(other.inputNeuronWeights))
, hiddenNeuronWeights(std::move(other.hiddenNeuronWeights))
, outputNeuronWeights(std::move(other.outputNeuronWeights))
, memoryNeuronWeights(std::move(other.memoryNeuronWeights))
, bias(std::move(other.bias))
, value(std::move(other.value))
, outputValue(std::move(other.outputValue))
, weightedSum(std::move(other.weightedSum))
{
    
}

Jimmy::LoopingNeuron& Jimmy::LoopingNeuron::operator=(const LoopingNeuron& other) noexcept {
    inputNeuronWeights = other.inputNeuronWeights;
    hiddenNeuronWeights = other.hiddenNeuronWeights;
    outputNeuronWeights = other.outputNeuronWeights;
    memoryNeuronWeights = other.memoryNeuronWeights;
    bias = other.bias;
    value = other.value;
    outputValue = other.outputValue;
    weightedSum = other.weightedSum;

    return *this;
}

Jimmy::LoopingNeuron& Jimmy::LoopingNeuron::operator=(LoopingNeuron&& other) noexcept {
    if(this != &other){
        inputNeuronWeights = std::move(other.inputNeuronWeights);
        hiddenNeuronWeights = std::move(other.hiddenNeuronWeights);
        outputNeuronWeights = std::move(other.outputNeuronWeights);
        memoryNeuronWeights = std::move(other.memoryNeuronWeights);
        bias = std::move(other.bias);
        value = std::move(other.value);
        outputValue = std::move(other.outputValue);
        weightedSum = std::move(other.weightedSum);
    }
    return *this;
}

Jimmy::LoopingNeuron::~LoopingNeuron(){

}

Jimmy::LoopingNeuron::LoopingNeuron(size_t inputLayerSize, size_t hiddenLayerSize, size_t outputLayerSize, size_t memoryLayerSize)
: inputNeuronWeights(inputLayerSize)
, hiddenNeuronWeights(hiddenLayerSize)
, outputNeuronWeights(outputLayerSize)
, memoryNeuronWeights(memoryLayerSize)
, bias(0)
, value(0)
, outputValue(0)
, weightedSum(0)
{
    
}

Jimmy::LoopingNeuron::LoopingNeuron()
: LoopingNeuron(0,0,0,0)
{
    
}