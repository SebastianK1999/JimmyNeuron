#include <iostream>
#include <JimmyNeuron/Jimmy.hpp>
#include <chrono>
#include <climits>

using namespace std;

int main(){
    std::vector<unsigned int> topology{4,10,1}; // defying how many neurons will be craerted in each layer
    Jimmy::Net mynet(topology, Jimmy::Methods::transFuncs::tanh, Jimmy::Methods::lossFuncs::rmse);
    vector<double> inp = {0, 0, 0, 0};
    vector<double> prop = {0};
    //mynet.feedForward(inp);
    //mynet.backProp(prop);
    
    for(int i = 0; true; i++){ // Test Program if the net cac se the result of "f(n1,n2,n3,n4) = (n1 && n2) ^ (n3 && n4)""
        std::srand(std::chrono::steady_clock::now().time_since_epoch().count()); // new seed
        inp[0] =  (std::rand() % 2); // new random inputs (0 or 1) 
        inp[1] =  (std::rand() % 2);
        inp[2] =  (std::rand() % 2);
        inp[3] =  (std::rand() % 2);
        prop[0] = (inp[0] && inp[1]) ^  (inp[2] && inp[3]); // crating output target
        
        mynet.feedForward(inp); // runing the  net
        mynet.backProp(prop);

        // outputing data to the console
        cout << "example " << i << "\n"
             << "result:" << mynet.getResult(0) << "\ttarget: " << prop[0] << "\tloss: " << mynet.getLoss() <<'\n';

        // checking if passed
        if((mynet.getResult(0) >= 0.5 && prop[0] == 1) || (mynet.getResult(0) < 0.5 && prop[0] == 0)){
            cout << "\x1b[42mpassed\x1b[0m\n\n";
        }
        else{
            cout <<"\x1b[41mfailed\x1b[0m\n\n";
        }
    }
    
    Jimmy::includeTest(); // test if the JimmyNeuron is corectly included

    return 0;
}