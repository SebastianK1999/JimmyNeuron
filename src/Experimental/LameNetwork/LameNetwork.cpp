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


#include "JimmyNeuron/Experimental/LameNetwork/LameNetwork.hpp"

Jimmy::E::LameNetwork::LameNetwork(size_t aInputSize, size_t aHiddenSize, size_t aOutputSize, size_t aMemorySize, Jimmy::TransferFunction aTransferFunction, double aSusceptibilityToMutations)
    : score(0)
    , susceptibilityToMutations(aSusceptibilityToMutations)
    , inputSize(aInputSize)
    , hiddenValues(aHiddenSize)
    , outputValues(aOutputSize)
    , memoryValues(aMemorySize)
    , weights(aInputSize, aHiddenSize, aOutputSize, aMemorySize)
{}

void Jimmy::E::LameNetwork::randomize(const double weightMin, const double weightMax)
{
    #define RANDOM() \
        Jimmy::Misc::rand().decimal(weightMin, weightMax)

    #define FOR_WEIGHT(WEIGHT_TYPE) \
        for(unsigned int weightIndex = 0; weightIndex < currentVector.WEIGHT_TYPE.size(); weightIndex++) \
        { \
            currentVector.WEIGHT_TYPE[weightIndex] = RANDOM(); \
        }

    #define FOR_VECTOR(VECTOR_TYPE) \
        for (unsigned int vectorIndex = 0; vectorIndex < weights.VECTOR_TYPE.size(); vectorIndex++) \
        { \
            auto& currentVector = weights.VECTOR_TYPE[vectorIndex]; \
            currentVector.bias = RANDOM(); \
            FOR_WEIGHT(input); \
            FOR_WEIGHT(hidden); \
            FOR_WEIGHT(output); \
            FOR_WEIGHT(memory); \
        }

    FOR_VECTOR(hidden);
    FOR_VECTOR(output);
    FOR_VECTOR(memory);

    #undef RANDOM
    #undef FOR_WEIGHT
    #undef FOR_VECTOR
}

void Jimmy::E::LameNetwork::mutate()
{
    #define MUTATION() \
        Jimmy::Misc::rand().decimal(-susceptibilityToMutations, susceptibilityToMutations)

    #define FOR_WEIGHT(WEIGHT_TYPE) \
        for(unsigned int weightIndex = 0; weightIndex < currentVector.WEIGHT_TYPE.size(); weightIndex++) \
        { \
            currentVector.WEIGHT_TYPE[weightIndex] += MUTATION(); \
        }

    #define FOR_VECTOR(VECTOR_TYPE) \
        for (unsigned int vectorIndex = 0; vectorIndex < weights.VECTOR_TYPE.size(); vectorIndex++) \
        { \
            auto& currentVector = weights.VECTOR_TYPE[vectorIndex]; \
            currentVector.bias += MUTATION(); \
            FOR_WEIGHT(input); \
            FOR_WEIGHT(hidden); \
            FOR_WEIGHT(output); \
            FOR_WEIGHT(memory); \
        }

    FOR_VECTOR(hidden);
    FOR_VECTOR(output);
    FOR_VECTOR(memory);

    #undef MUTATION
    #undef FOR_WEIGHT
    #undef FOR_VECTOR
}

void Jimmy::E::LameNetwork::evolveFrom(const Jimmy::E::LameNetwork& parent0, const Jimmy::E::LameNetwork& parent1)
{
    
    #if 1
    #define INHERIT(MEMBER) \
        Jimmy::Misc::rand().decimal(parent0Vector.MEMBER, parent1Vector.MEMBER) + Jimmy::Misc::rand().decimal(-susceptibilityToMutations, susceptibilityToMutations)
    
    #else
    #define INHERIT(MEMBER) \
        (Jimmy::Misc::rand().binary() ? parent0Vector.MEMBER : parent1Vector.MEMBER) + Jimmy::Misc::rand().decimal(-susceptibilityToMutations, susceptibilityToMutations)

    #endif

    #define FOR_WEIGHT(WEIGHT_TYPE) \
        for(unsigned int weightIndex = 0; weightIndex < currentVector.WEIGHT_TYPE.size(); weightIndex++) \
        { \
            currentVector.WEIGHT_TYPE[weightIndex] = INHERIT(WEIGHT_TYPE[weightIndex]); \
        }

    #define FOR_VECTOR(VECTOR_TYPE) \
        for (unsigned int vectorIndex = 0; vectorIndex < weights.VECTOR_TYPE.size(); vectorIndex++) \
        { \
            auto& currentVector = weights.VECTOR_TYPE[vectorIndex]; \
            auto& parent0Vector = parent0.weights.VECTOR_TYPE[vectorIndex]; \
            auto& parent1Vector = parent1.weights.VECTOR_TYPE[vectorIndex]; \
            currentVector.bias = INHERIT(bias); \
            FOR_WEIGHT(input); \
            FOR_WEIGHT(hidden); \
            FOR_WEIGHT(output); \
            FOR_WEIGHT(memory); \
        }

    FOR_VECTOR(hidden);
    FOR_VECTOR(output);
    FOR_VECTOR(memory);

    #undef INHERIT
    #undef FOR_WEIGHT
    #undef FOR_VECTOR
}

void Jimmy::E::LameNetwork::addNeurons(size_t aInputSize, size_t aHiddenSize, size_t aOutputSize, size_t aMemorySize)
{
    #define FOR_VECTOR(VECTOR_TYPE) \
        for (unsigned int vectorIndex = 0; vectorIndex < weights.VECTOR_TYPE.size(); vectorIndex++) \
        { \
            auto& currentVector = weights.VECTOR_TYPE[vectorIndex]; \
            currentVector.input.resize(inputSize, 0); \
            currentVector.hidden.resize(hiddenSize, 0); \
            currentVector.output.resize(outputSize, 0); \
            currentVector.memory.resize(memorySize, 0); \
        }

    inputSize += aInputSize;
    size_t hiddenSize = hiddenValues.size() + aHiddenSize;
    size_t outputSize = outputValues.size() + aOutputSize;
    size_t memorySize = memoryValues.size() + aMemorySize;
    hiddenValues.resize(hiddenSize);
    weights.hidden.resize(hiddenSize, Jimmy::E::LameWeightVector(inputSize, hiddenSize, outputSize, memorySize));
    FOR_VECTOR(hidden);
    outputValues.resize(outputSize);
    weights.output.resize(outputSize, Jimmy::E::LameWeightVector(inputSize, hiddenSize, outputSize, memorySize));
    FOR_VECTOR(output);
    outputValues.resize(outputSize);
    weights.memory.resize(memorySize, Jimmy::E::LameWeightVector(inputSize, hiddenSize, outputSize, memorySize));
    FOR_VECTOR(memory);

    #undef FOR_VECTOR
}

void Jimmy::E::LameNetwork::clearValues()
{
    for (unsigned int valueIndex = 0; valueIndex < hiddenValues.size(); valueIndex++)
    {
        hiddenValues[valueIndex] = 0;
    }
    for (unsigned int valueIndex = 0; valueIndex < outputValues.size(); valueIndex++)
    {
        outputValues[valueIndex] = 0;
    }
    for (unsigned int valueIndex = 0; valueIndex < memoryValues.size(); valueIndex++)
    {
        memoryValues[valueIndex] = 0;
    }
}

void Jimmy::E::LameNetwork::run(const std::vector<double>& inputValues)
{
    for (unsigned int valueIndex = 0; valueIndex < hiddenValues.size(); valueIndex++)
    {
        hiddenValues[valueIndex] = std::tanh
        (
            weights.hidden[valueIndex].bias
            + Jimmy::Misc::WeightedSumSIMD(inputValues,  weights.hidden[valueIndex].input)
            + Jimmy::Misc::WeightedSumSIMD(hiddenValues, weights.hidden[valueIndex].hidden)
            + Jimmy::Misc::WeightedSumSIMD(outputValues, weights.hidden[valueIndex].output)
            + Jimmy::Misc::WeightedSumSIMD(memoryValues, weights.hidden[valueIndex].memory)
        );
    }
    for (unsigned int valueIndex = 0; valueIndex < outputValues.size(); valueIndex++)
    {
        outputValues[valueIndex] = std::tanh
        (
            weights.output[valueIndex].bias
            + Jimmy::Misc::WeightedSumSIMD(inputValues,  weights.output[valueIndex].input)
            + Jimmy::Misc::WeightedSumSIMD(hiddenValues, weights.output[valueIndex].hidden)
            + Jimmy::Misc::WeightedSumSIMD(outputValues, weights.output[valueIndex].output)
            + Jimmy::Misc::WeightedSumSIMD(memoryValues, weights.output[valueIndex].memory)
        );
    }
    for (unsigned int valueIndex = 0; valueIndex < memoryValues.size(); valueIndex++)
    {
        memoryValues[valueIndex] = std::tanh
        (
            weights.memory[valueIndex].bias
            + Jimmy::Misc::WeightedSumSIMD(inputValues,  weights.memory[valueIndex].input)
            + Jimmy::Misc::WeightedSumSIMD(hiddenValues, weights.memory[valueIndex].hidden)
            + Jimmy::Misc::WeightedSumSIMD(outputValues, weights.memory[valueIndex].output)
            + Jimmy::Misc::WeightedSumSIMD(memoryValues, weights.memory[valueIndex].memory)
        );
    }
}

void Jimmy::E::LameNetwork::saveToFile(const std::filesystem::path& path) const
{
    std::ofstream ofs(path, std::ios::binary);
    size_t hiddenSize = hiddenValues.size();
    size_t outputSize = outputValues.size();
    size_t memorySize = memoryValues.size();
    ofs.write(reinterpret_cast<const char*>(& inputSize), sizeof(size_t));
    ofs.write(reinterpret_cast<const char*>(& hiddenSize), sizeof(size_t));
    ofs.write(reinterpret_cast<const char*>(& outputSize), sizeof(size_t));
    ofs.write(reinterpret_cast<const char*>(& memorySize), sizeof(size_t));

    #define FOR_VECTOR(VECTOR_TYPE) \
        for (unsigned int vectorIndex = 0; vectorIndex < weights.VECTOR_TYPE.size(); vectorIndex++) \
        { \
            auto& currentVector = weights.VECTOR_TYPE[vectorIndex]; \
            ofs.write(reinterpret_cast<const char *>(&(currentVector.bias)), sizeof(double)); \
            ofs.write(reinterpret_cast<const char *>(currentVector.input.data() ),inputSize  * sizeof(double)); \
            ofs.write(reinterpret_cast<const char *>(currentVector.hidden.data()),hiddenSize * sizeof(double)); \
            ofs.write(reinterpret_cast<const char *>(currentVector.output.data()),outputSize * sizeof(double)); \
            ofs.write(reinterpret_cast<const char *>(currentVector.memory.data()),memorySize * sizeof(double)); \
        }

    FOR_VECTOR(hidden);
    FOR_VECTOR(output);
    FOR_VECTOR(memory);

    #undef FOR_VECTOR

    ofs.close();
}

void Jimmy::E::LameNetwork::loadFromFile(const std::filesystem::path& path)
{
    std::ifstream ifs(path, std::ios::binary);
    size_t hiddenSize;
    size_t outputSize;
    size_t memorySize;
    ifs.read(reinterpret_cast<char *>(&inputSize), sizeof(size_t));
    ifs.read(reinterpret_cast<char *>(&hiddenSize), sizeof(size_t));
    ifs.read(reinterpret_cast<char *>(&outputSize), sizeof(size_t));
    ifs.read(reinterpret_cast<char *>(&memorySize), sizeof(size_t));
    weights.hidden.resize(hiddenSize, Jimmy::E::LameWeightVector(inputSize, hiddenSize, outputSize, memorySize));
    weights.output.resize(outputSize, Jimmy::E::LameWeightVector(inputSize, hiddenSize, outputSize, memorySize));
    weights.memory.resize(memorySize, Jimmy::E::LameWeightVector(inputSize, hiddenSize, outputSize, memorySize));

    #define FOR_VECTOR(VECTOR_TYPE) \
        for (unsigned int vectorIndex = 0; vectorIndex < weights.VECTOR_TYPE.size(); vectorIndex++) \
        { \
            auto& currentVector = weights.VECTOR_TYPE[vectorIndex]; \
            currentVector.input.resize(inputSize); \
            currentVector.hidden.resize(hiddenSize); \
            currentVector.output.resize(outputSize); \
            currentVector.memory.resize(memorySize); \
            ifs.read(reinterpret_cast<char *>(&(currentVector.bias)), sizeof(double)); \
            ifs.read(reinterpret_cast<char *>(currentVector.input.data() ),inputSize  * sizeof(double)); \
            ifs.read(reinterpret_cast<char *>(currentVector.hidden.data()),hiddenSize * sizeof(double)); \
            ifs.read(reinterpret_cast<char *>(currentVector.output.data()),outputSize * sizeof(double)); \
            ifs.read(reinterpret_cast<char *>(currentVector.memory.data()),memorySize * sizeof(double));\
        }

    FOR_VECTOR(hidden);
    FOR_VECTOR(output);
    FOR_VECTOR(memory);

    #undef FOR_VECTOR

    ifs.close();



}

const std::vector<double>& Jimmy::E::LameNetwork::getOutput()
{
    return outputValues;
}
