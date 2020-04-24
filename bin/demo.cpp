#include <iostream>
#include <JimmyNeuron/Jimmy.hpp>

using namespace std;

int main(){
    std::vector<unsigned int> topology{2,3,1}; // defying how many neurons will be craerted in each layer
    Jimmy::Net mynet(topology, Jimmy::Methods::transFuncs::sigmoid, Jimmy::Methods::lossFuncs::bce); // passing topopgy to initalize network
    vector<double> inp = {0.7, 0.04};
    vector<double> prop = {1};
    mynet.feedForward(inp);
    mynet.backProp(prop);
    
    Jimmy::includeTest(); // test if the JimmyNeuron is corectly included

    return 0;
}