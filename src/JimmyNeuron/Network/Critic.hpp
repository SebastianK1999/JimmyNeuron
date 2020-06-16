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
    class RECORDS_IO;
    class Critic{
        private:
        Jimmy::Net* networkPointer; // poiner to the critic's network
        Jimmy::RECORDS_IO* recPointer; // heap alcated pointer

        std::vector<std::reference_wrapper<Jimmy::NEURON>> watchListNeurons; // vector of neurons that critic tries to lean 
        void backProp(float_t,int); // modified backProp algorythym. supports wotchlist neurons
        void backPropRecords(float_t,int,int); // modified backProp algorythym. supports wotchlist neurons
        float_t trigger = 0.5; // constant that sets the poit in whitch the output logic value changes
        int logicValue(const float_t&); // convets output to posible desired output
        void replay(int recordIndex); // feed forward but works on recored data 

        public:
        void recordMove(); // saves input and output od the net
        void record(); // saves input and output od the net
        void clearRecords(); // Clears records in case of net winingg or losing;
        void rewardRecords(float_t = 1); // rewards based on good preformance with no errors
        void punishRecords(float_t = 1); // punishes based on bad preformance with no errors
        void reward(float_t = 1); // reward watchlist neurons titsh bacprop
        void punish(float_t = 1); // punish watchlist neurons titsh bacprop

        void chooseHighest(); // puts neuron with the highest output on the watch list
        void chooseActive(); // puts neurons thar are activated on a watchlist;
        void chooseLowest(); // puts neuron with the lowes output on the watchlist
        void chooseAll(); // puts alll neuron on the watch list
        void chooseSelected(const std::vector<int>& outputNeuronIndex); // puts selected by the index nuron on the watchlist

        Critic(std::reference_wrapper<Jimmy::Net>); // constructor
        ~Critic();
    };
}

#endif

