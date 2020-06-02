#ifndef CRITIC_INCLUDE
#define CRITIC_INCLUDE

#include "Layer.hpp"
#include "TransferFunction.hpp"
#include "LossFunction.hpp"
#include "RecordsIO.hpp"

#include <JimmyNeuron/Jimmy.hpp>
#include <iostream>
#include <vector>
#include <functional>
#include <cassert>

namespace Jimmy{
    class Net;
    class Critic{
        private:
        Jimmy::Net* networkPointer; // poiner to the critic's network
        std::vector<std::reference_wrapper<Jimmy::NEURON>> watchListNeurons; // vector of neurons that critic tries to lean 
        void backProp(int); // modified backProp algorythym. supports wotchlist neurons
        
        double trigger = 0.5; // constant that sets the poit in whitch the output logic value changes
        int logicValue(const double&); // convets output to posible desired output

        Jimmy::RECORDS_IO records;

        public:
        void record(); // saves input and output od the net
        void rewardRecords(); // rewards based on good preformance with no errors
        void punishRecords(); // punishes based on bad preformance with no errors

        void chooseHighest(); // puts neuron with the highest output on the watch list
        void chooseLowest(); // puts neuron with the lowes output on the watchlist
        void chooseAll(); // puts alll neuron on the watch list
        void chooseSelected(const std::vector<unsigned int>& outputNeuronIndex); // puts selected by the index nuron on the watchlist

        void reward(); // reward watchlist neurons titsh bacprop
        void punish(); // punish watchlist neurons titsh bacprop
        Critic(std::reference_wrapper<Jimmy::Net>); // constructor
    };
}

#endif

