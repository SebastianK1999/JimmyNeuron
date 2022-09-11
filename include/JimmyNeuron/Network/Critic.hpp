#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include <cassert>

#include "JimmyNeuron/Network/Net.hpp"
#include "JimmyNeuron/Network/Layer.hpp"
#include "JimmyNeuron/Network/TransferFunction.hpp"
#include "JimmyNeuron/Network/LossFunction.hpp"
#include "JimmyNeuron/Network/RecordsIO.hpp"

namespace Jimmy{
    class Network;
    class RECORDS_IO;
    class Critic{
        private:
        Jimmy::Net* networkPointer; // pointer to the critic's network
        Jimmy::RECORDS_IO* recPointer; // heap allocated pointer

        std::vector<std::reference_wrapper<Jimmy::Neuron>> watchListNeurons; // vector of neurons that critic tries to lean 
        void backProp(double,int); // modified backProp algorithm. supports watchlist neurons
        void backPropRecords(double,int,int); // modified backProp algorithm. supports watchlist neurons
        double trigger = 0.5; // constant that sets the point in which the output logic value changes
        int logicValue(const double&); // converts output to possible desired output
        void replay(int recordIndex); // feed forward but works on recorded data 

        public:
        void recordMove(); // saves input and output od the net
        void record(); // saves input and output od the net
        void clearRecords(); // Clears records in case of net winning or losing;
        void rewardRecords(double = 1); // rewards based on good performance with no errors

        void punishRecords(double = 1); // punishes based on bad performance with no errors
        void punish(double = 1); // punish watchlist neurons using backprop
        void reward(double = 1); // reward watchlist neurons using backprop

        void chooseHighest(); // puts neuron with the highest output on the watch list
        void chooseActive(); // puts neurons thar are activated on a watchlist;
        void chooseLowest(); // puts neuron with the lowest output on the watchlist
        void chooseAll(); // puts all neuron on the watch list
        void chooseSelected(const std::vector<int>& outputNeuronIndex); // puts selected by the index neuron on the watchlist

        Critic(std::reference_wrapper<Jimmy::Net>); // constructor
        ~Critic();
    };
}

