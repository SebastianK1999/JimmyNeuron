#include <iostream>
#include <JimmyNeuron/Jimmy.hpp>


using namespace std;

int main(){
    std::vector<unsigned int> topology{2,3,1}; // defying how many neurons will be craerted in each layer

    Jimmy::Net mynet(topology); // passing topopgy to initalize network
    Jimmy::includeTest(); // test if the JimmyNeuron is corectly included 
    return 0;
}