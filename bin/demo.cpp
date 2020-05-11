#define DEV_NET // DEVELOPMENT MODE ( GAME / NET)


#include <iostream>
#include <JimmyNeuron/Jimmy.hpp>
#include <chrono>
#include <climits>
#include <Games/DinoGame.hpp>

using namespace std;

int main(){
#ifdef DEV_GAME  
    Games::DinoGame game;
    game.start();

#endif
#ifdef DEV_NET

    std::vector<unsigned int> topology{2,10,1}; // defying how many neurons will be craerted in each layer
    Jimmy::Net mynet(topology, Jimmy::Methods::transFuncs::linear, Jimmy::Methods::lossFuncs::rmse);
    vector<double> inp = {0, 0};
    vector<double> prop = {0};
    mynet.feedForward(inp);
    mynet.backProp(prop);
    for(int i = 0; true; i++){ // Test Program if the net cac se the result of "f(n1,n2,n3,n4) = (n1 && n2) ^ (n3 && n4)""
        std::srand(std::chrono::steady_clock::now().time_since_epoch().count()); // new seed
        inp[0] =  ((double)std::rand() / INT_MAX); // new random inputs (0 or 1) 
        inp[1] =  ((double)std::rand() / INT_MAX);
        //inp[2] =  (std::rand() % 2);
        //inp[3] =  (std::rand() % 2);
        prop[0] = (inp[0] > 0.75); // crating output target
        
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

    #endif

    return 0;
}