#include <iostream>
#include <chrono>
#include <climits>
#include <cmath>
#include "JimmyNeuron/Jimmy.hpp"

using namespace std;

int main(){
    std::vector<int> topology{1,40,40,1}; // defying how many neurons will be created in each layer
    Jimmy::Net myNet(topology, Jimmy::Methods::transFuncs::tanh, Jimmy::Methods::lossFuncs::rmse, 0.01);
    vector<double> inp(topology[0]);
    vector<double> prop(topology.back());
    Jimmy::Critic critic(myNet);
    critic.chooseAll();
    for(int i = 0; true; i++){ // Test Program if the net can predict the result of sin(x)
        inp[0] =  Jimmy::Misc::rand::get().Decimal(0,6); // new random inputs (0 or 1) =
        prop[0] = sinf(inp[0]); // crating output target
        
        myNet.feedForward(inp); // running the  net
        myNet.backProp(prop);
    
        // if ((double)myNet.getResult(0) < 1)
        // std::cout << (double)myNet.getResult(0) << " " <<  << "\n";

        // checking if passed
        if(abs(prop[0] - myNet.getResult(0))< 0.1){
            cout << "\x1b[42m passed \x1b[0m\n";
            // critic.reward();
        }
        else{
            cout <<"\x1b[41m failed \x1b[0m\n";
            // critic.punish();
        }
        // cout << inp[0] << "\n";
        // // outputting data to the console
        cout << "example " << i << "\n"
             << "result:" << myNet.getResult(0) << "\ttarget: " << prop[0] << "\tloss: " << myNet.getLoss() <<"\n\n";

    }
    
    Jimmy::includeTest(); // test if the JimmyNeuron is correctly included

    return 0;
}