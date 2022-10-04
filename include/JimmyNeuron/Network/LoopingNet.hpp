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

#include "JimmyNeuron/Network/LoopingNeuron.hpp"
#include "JimmyNeuron/Network/TransferFunction.hpp"

namespace Jimmy
{
    class LoopingNet{
    public:
        std::vector<Jimmy::LoopingNeuron> inputNeurons;
        std::vector<Jimmy::LoopingNeuron> hiddenNeurons;
        std::vector<Jimmy::LoopingNeuron> outputNeurons;
        std::vector<double*> outputDoublePtrs;
        Jimmy::TransferFunction transferFunction;
        double susceptibilityToMutations;
        double score;

        LoopingNet(const LoopingNet& other ) noexcept;
        LoopingNet(LoopingNet&& other) noexcept;
        LoopingNet& operator=(const LoopingNet& other) noexcept;
        LoopingNet& operator=(LoopingNet&& other) noexcept;
        virtual ~LoopingNet();
        LoopingNet(size_t inputLayerSize, size_t hiddenLayerSize, size_t outputLayerSize, Jimmy::TransferFunction transferFunction, double susceptibilityToMutations);
        void setInput(const std::vector<double>& inputs);
        const std::vector<double>& getOutput();
        void run();
        void run(const std::vector<double>& inputs);
        void mutate();
        void randomize(double minWeight, double maxWeight);
        void evolveFrom(const Jimmy::LoopingNet& networkParent);
        void evolveFrom(const Jimmy::LoopingNet& networkFemale, const Jimmy::LoopingNet& networkMale);
        void evolveFrom(const std::vector<Jimmy::LoopingNet*>& networkOrgyPointers);
        void addInputNeurons(size_t n);
        void addHiddenNeurons(size_t n);
        void addOutputNeurons(size_t n);
        void clearValues();
        void clearOutputValues();
        
    private:
        std::vector<double> outputVector;

        void neuronsProcess(std::vector<Jimmy::LoopingNeuron>& processingNeurons, const bool resetSum = false, const bool addBias = false, const bool processInputs = false, const bool processHidden = false, const bool processOutput = false);
        static void setOutputValueAsValue(std::vector<Jimmy::LoopingNeuron>& neurons);

    };
} // namespace Jimmy
