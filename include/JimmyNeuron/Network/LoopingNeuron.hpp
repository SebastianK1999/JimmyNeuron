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


#pragma once

#include <vector>

namespace Jimmy
{
    class LoopingNeuron{
    public:
        std::vector<double> inputNeuronWeights;
        std::vector<double> hiddenNeuronWeights;
        std::vector<double> outputNeuronWeights;
        std::vector<double> memoryNeuronWeights;
        double bias;
        double value;
        double outputValue;
        double weightedSum;

        LoopingNeuron(const LoopingNeuron& other ) noexcept;
        LoopingNeuron(LoopingNeuron&& other) noexcept;
        LoopingNeuron& operator=(const LoopingNeuron& other) noexcept;
        LoopingNeuron& operator=(LoopingNeuron&& other) noexcept;
        virtual ~LoopingNeuron();
        LoopingNeuron(size_t inputLayerSize, size_t hiddenLayerSize, size_t outputLayerSize, size_t memoryLayerSize);
        LoopingNeuron();
    };
} // namespace jimmy
