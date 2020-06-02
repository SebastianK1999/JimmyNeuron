#ifndef RECORDSIO_INCLUDED 
#define RECORDSIO_INCLUDED

#include "Network.hpp"
#include <vector>

namespace Jimmy{
    class RECORDS_IO{
        friend class Critic;
    private:
        std::vector<std::vector<double>> inputs;
        std::vector<std::vector<double>> outputs;
        std::vector<std::vector<unsigned int>> outputIndexes;

    }; // end of Records_IO
} // end of Jimmy

#endif