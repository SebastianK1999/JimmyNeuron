#define DEV_GAME // DEVELOPMENT MODE ( GAME / NET)


#include <iostream>
#include <JimmyNeuron/Jimmy.hpp>
#include <chrono>
#include <climits>
#include <Games/DinoGame.hpp>
#include <Games/TicTacToe.hpp>

using namespace std;

int main(){
#ifdef DEV_GAME  
    //Games::DinoGame game;
    //game.start();
    Games::TicTacToe game2;
    game2.start();

#else
#ifdef DEV_NET
    std::vector<int> topology{2,5,1}; // defying how many neurons will be craerted in each layer
    Jimmy::Net mynet(topology, Jimmy::Methods::transFuncs::tanh, Jimmy::Methods::lossFuncs::rmse, 0.01);
    vector<float_t> inp = {0, 0};
    vector<float_t> prop = {0};
    Jimmy::Critic critic(mynet);
    critic.chooseAll();
    for(int i = 0; true; i++){ // Test Program if the net cac se the result of "f(n1,n2,n3,n4) = (n1 && n2) ^ (n3 && n4)""
        inp[0] =  Jimmy::Misc::rand::get().binary(); // new random inputs (0 or 1) 
        inp[1] =  Jimmy::Misc::rand::get().binary();
        prop[0] = (inp[0]>0.5 ^ inp[1]>0.5); // crating output target
        
        mynet.feedForward(inp); // runing the  net
        //mynet.backProp(prop);
    
        // checking if passed
        if((mynet.getResult(0) >= 0.5 && prop[0] == 1) || (mynet.getResult(0) < 0.5 && prop[0] == 0)){
            cout << "\x1b[42mpassed\x1b[0m\n";
            critic.reward();
        }
        else{
            cout <<"\x1b[41mfailed\x1b[0m\n";
            critic.punish();
        }

        cout << inp[0] << " " << inp[1] << "\n";
        // outputing data to the console
        cout << "example " << i << "\n"
             << "result:" << mynet.getResult(0) << "\ttarget: " << prop[0] << "\tloss: " << mynet.getLoss() <<"\n\n";
    }
    
    Jimmy::includeTest(); // test if the JimmyNeuron is corectly included

#endif
#endif

    return 0;
}