#ifndef RECORDSIO_INCLUDED 
#define RECORDSIO_INCLUDED

#include "Network.hpp"
#include <vector>

namespace Jimmy{
    class Net;

    class NEURON_WEIGHTS{
        public:
            std::vector<double> inputWeightsRecords{};
            double selfWeightRecord;
        };
    class RECORDS_IO{
        friend class Critic;

    public:
        void clear();

    private:
        void saveInputWeights(Jimmy::Net*);

        std::vector<std::vector<Jimmy::NEURON_WEIGHTS>> inputWeights;
        std::vector<std::vector<double>> inputs;
        std::vector<std::vector<int>> outputIndexes;

    }; // end of Records_IO
} // end of Jimmy

#endif