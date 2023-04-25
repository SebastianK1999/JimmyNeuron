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


#pragma once

#include <vector>
#include <filesystem>

#include "JimmyNeuron/Jimmy.hpp"
#include "JimmyNeuron/Experimental/LameNetwork/LameWeightMatrix.hpp"

namespace Jimmy
{
    namespace E
    {
        class LameNetwork
        {
        public:
            double score;
            double susceptibilityToMutations;

            LameNetwork(size_t inputSize, size_t hiddenSize, size_t outputSize, size_t memorySize, Jimmy::TransferFunction transferFunction, double susceptibilityToMutations);
            void randomize(const double weightMin, const double weightMax);
            void mutate();
            void evolveFrom(const LameNetwork& parent_0, const LameNetwork& parent_1);
            void addNeurons(size_t inputSize, size_t hiddenSize, size_t outputSize, size_t memorySize);
            void clearValues();
            void run(const std::vector<double>& inputValues);
            void saveToFile(const std::filesystem::path& path) const;
            void loadFromFile(const std::filesystem::path& path);
            const std::vector<double>& getOutput();

        private:
            size_t inputSize;
            std::vector<double> hiddenValues;
            std::vector<double> outputValues;
            std::vector<double> memoryValues;
            Jimmy::E::LameWeightMatrix weights;

        };
    }
}