#include <iostream>
#include <chrono>
#include <climits>
#include "JimmyNeuron/Jimmy.hpp"

using namespace std;

int main(){
    std::vector<int> topology{2,5,1}; // defying how many neurons will be created in each layer
    Jimmy::Net myNet(topology, Jimmy::Methods::transFuncs::tanh, Jimmy::Methods::lossFuncs::rmse, 0.1);
    vector<double> inp = {0, 0};
    vector<double> prop = {0};
    Jimmy::Critic critic(myNet);
    critic.chooseAll();
    for(int i = 0; true; i++){ // Test Program if the net cac se the result of "f(n1,n2,n3,n4) = (n1 && n2) ^ (n3 && n4)""
        inp[0] =  Jimmy::Misc::rand().binary(); // new random inputs (0 or 1) 
        inp[1] =  Jimmy::Misc::rand().binary();
        prop[0] = ((inp[0]>0.5) ^ (inp[1]>0.5)); // crating output target
        
        myNet.feedForward(inp); // running the  net
        // myNet.backProp(prop);
    
        // checking if passed
        if((myNet.getResult(0) >= 0.5 && prop[0] == 1) || (myNet.getResult(0) < 0.5 && prop[0] == 0)){
            cout << "\x1b[42m passed \x1b[0m\n";
            critic.reward();
        }
        else{
            cout <<"\x1b[41m failed \x1b[0m\n";
            critic.punish();
        }

        cout << inp[0] << " " << inp[1] << "\n";
        // outputting data to the console
        cout << "example " << i << "\n"
             << "result:" << myNet.getResult(0) << "\ttarget: " << prop[0] << "\tloss: " << myNet.getLoss() <<"\n\n";
    }
    
    Jimmy::includeTest(); // test if the JimmyNeuron is correctly included

    return 0;
}