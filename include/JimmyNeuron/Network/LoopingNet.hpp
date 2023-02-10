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
#include <fstream>

#include "JimmyNeuron/Network/LoopingNeuron.hpp"
#include "JimmyNeuron/Network/TransferFunction.hpp"

namespace Jimmy
{
    class LoopingNet{
    public:
        std::vector<Jimmy::LoopingNeuron> inputNeurons;
        std::vector<Jimmy::LoopingNeuron> hiddenNeurons;
        std::vector<Jimmy::LoopingNeuron> outputNeurons;
        std::vector<Jimmy::LoopingNeuron> memoryNeurons;
        std::vector<double*> outputDoublePtrs;
        Jimmy::TransferFunction transferFunction;
        double susceptibilityToMutations;
        double score;

        LoopingNet(const LoopingNet& other ) noexcept;
        LoopingNet(LoopingNet&& other) noexcept;
        LoopingNet& operator=(const LoopingNet& other) noexcept;
        LoopingNet& operator=(LoopingNet&& other) noexcept;
        virtual ~LoopingNet();
        LoopingNet(size_t inputLayerSize, size_t hiddenLayerSize, size_t outputLayerSize, size_t memoryLayerSize, Jimmy::TransferFunction transferFunction, double susceptibilityToMutations);
        void setInput(const std::vector<double>& inputs);
        const std::vector<double>& getOutput();
        void run();
        void run(const std::vector<double>& inputs);
        void mutate();
        void randomize(double minWeight, double maxWeight);
        void evolveFrom(const Jimmy::LoopingNet& networkParent);
        void evolveFrom(const Jimmy::LoopingNet& networkFemale, const Jimmy::LoopingNet& networkMale);
        void evolveFrom(const std::vector<Jimmy::LoopingNet*>& networkOrgyPointers);
        void addNeurons(size_t nI, size_t nH ,size_t nO, size_t nM);
        void addInputNeurons(size_t n);
        void addHiddenNeurons(size_t n);
        void addOutputNeurons(size_t n);
        void addMemoryNeurons(size_t n);
        void clearValues();
        void clearOutputValues();
        void saveToFile(std::string path) const;
        void loadFromFile(std::string path);
        
    private:
        void saveNeuron(const Jimmy::LoopingNeuron& neuron, std::ofstream& ofs) const;
        void loadNeuron(Jimmy::LoopingNeuron& neuron, std::ifstream& ifs);
        std::vector<double> outputVector;

        template<bool RESET_SUM, bool ADD_BIAS, bool PROCESS_INPUT, bool PROCESS_HIDDEN, bool PROCESS_OUTPUT, bool PROCESS_MEMORY>
        void neuronsProcess(std::vector<Jimmy::LoopingNeuron>& processingNeurons, std::vector<double> inputData);
        static void setOutputValueAsValue(std::vector<Jimmy::LoopingNeuron>& neurons);

    };
} // namespace Jimmy

#include "JimmyNeuron/Network/LoopingNet.inl"