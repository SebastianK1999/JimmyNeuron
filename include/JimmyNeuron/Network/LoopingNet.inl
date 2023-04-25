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

/*
*   Inline file for LoopingNet.hpp
*/
template<bool RESET_SUM, bool ADD_BIAS, bool PROCESS_INPUT, bool PROCESS_HIDDEN, bool PROCESS_OUTPUT, bool PROCESS_MEMORY>
void Jimmy::LoopingNet::neuronsProcess(std::vector<Jimmy::LoopingNeuron>& processingNeurons, std::vector<double> inputData){
    for(Jimmy::LoopingNeuron& neuron : processingNeurons){
        if constexpr (RESET_SUM){
            neuron.weightedSum = 0;
        }
        if constexpr (ADD_BIAS){
            neuron.weightedSum += neuron.bias;
        }
        if constexpr (PROCESS_INPUT){ 

            neuron.weightedSum += Jimmy::Misc::WeightedSumSIMD(inputData, neuron.inputNeuronWeights); // beacuse of .vale i cant use SIMD everywhere
            // for(unsigned int i = 0; i < inputData.size(); i++){
            //     // std::cout << "inp : " << inputData[i] << " * " << neuron.inputNeuronWeights[i] << " = " << inputData[i] * neuron.inputNeuronWeights[i] << " " << transferFunction.run(INFINITY) << "\n";
            //     neuron.weightedSum += inputData[i] * neuron.inputNeuronWeights[i];
            // }
        }
        if constexpr (PROCESS_HIDDEN){
            for(unsigned int i = 0; i < hiddenNeurons.size(); i++){
                // std::cout << "hid : " << hiddenNeurons[i].value << " * " << neuron.hiddenNeuronWeights[i] << " = " << hiddenNeurons[i].value * neuron.hiddenNeuronWeights[i] << "\n";

                neuron.weightedSum += hiddenNeurons[i].value * neuron.hiddenNeuronWeights[i];
            } 
        }
        if constexpr (PROCESS_OUTPUT){
            // std::cout << "out {\n";
            // std::cout << "  " << neuron.weightedSum << "\n";
            for(unsigned int i = 0; i < outputNeurons.size(); i++){
                // std::cout << "  out : " << outputNeurons[i].value << " * " << neuron.outputNeuronWeights[i] << " = " << outputNeurons[i].value * neuron.outputNeuronWeights[i] << "\n";

                neuron.weightedSum += outputNeurons[i].value * neuron.outputNeuronWeights[i];
            }
            // std::cout << "  " << neuron.weightedSum << "\n";
            // std::cout << "}\n";
        }
        if constexpr (PROCESS_MEMORY){
            // std::cout << "mem {\n";
            // std::cout << "  " << neuron.weightedSum << "\n";
            for(unsigned int i = 0; i < memoryNeurons.size(); i++){
                // std::cout << "  out : " << memoryNeurons[i].value << " * " << neuron.memoryNeuronWeights[i] << " = " << memoryNeurons[i].value * neuron.memoryNeuronWeights[i] << "\n";

                neuron.weightedSum += memoryNeurons[i].value * neuron.memoryNeuronWeights[i];
            }
            // std::cout << "  " << neuron.weightedSum << "\n";
            // std::cout << "}\n";
        }
        // std::cout << neuron.weightedSum << " " << transferFunction.run(neuron.weightedSum) << "\n";
        neuron.outputValue = transferFunction.run(neuron.weightedSum);
    }   
}