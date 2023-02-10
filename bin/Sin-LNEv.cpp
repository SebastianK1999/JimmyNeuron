#include <iostream>
#include <chrono>
#include <climits>
#include <cmath>
#include <vector>
#include <algorithm>
#include "JimmyNeuron/Jimmy.hpp"
#include "JimmyNeuron/Network/LoopingNet.hpp"

using namespace std;

int main(){
    std::vector<Jimmy::LoopingNet> networks;
    networks.reserve(6);
    for(int i = 0; i < networks.capacity(); i++){
        networks.emplace_back(Jimmy::LoopingNet(1, 30, 1, 0, Jimmy::Methods::transFuncs::tanh, 0.1));
        Jimmy::LoopingNet& network = networks.back();
        network.randomize(-2.0/(30 + 2.0), 2.0/(30 + 2.0));
    }
    for(unsigned long int generationNumber = 0; true; generationNumber++){ // Test Program if the net can predict the result of sin(x)
        for(std::vector<Jimmy::LoopingNet>::iterator iter = networks.begin();  iter != networks.end(); iter = std::next(iter)){
            Jimmy::LoopingNet& network = (*iter);
            network.score = 0.0;
        }
        double biggestError = 0.0;
        double precision = 0.0;
        for(int i = 0; i < 100; i++){
            double angle = Jimmy::Misc::rand().decimal(0,6.28);
            double sinAngle = sin(angle);
            double biggestErrorNet = 0.0;
            for(std::vector<Jimmy::LoopingNet>::iterator iter = networks.begin();  iter != networks.end(); iter = std::next(iter)){
                Jimmy::LoopingNet& network = (*iter);
                network.inputNeurons[0].value = angle;
                network.run();
                double error = abs(sinAngle - network.outputNeurons[0].outputValue);
                if(generationNumber < 20000){
                    network.score += error;
                }
                else if(network.score < error){
                    network.score = error;
                }
                if(error > biggestErrorNet){
                    biggestErrorNet = error;
                }
                if(error < 0.1){
                    precision += 1.0/100.0;
                }
                // std::cout << network.outputNeurons[0].outputValue << "\n";
            }
            if(biggestErrorNet > biggestError){
                biggestError = biggestErrorNet;
            }
        }
        std::sort(networks.begin(), networks.end(), [](const Jimmy::LoopingNet& a, const Jimmy::LoopingNet& b){return a.score < b.score;});
        if(generationNumber % 100 == 0){
            std::cout << "generation " << generationNumber << ":\tpopulationError " << std::accumulate(networks.begin(), networks.end(), 0.0, [](double s, const Jimmy::LoopingNet& a){return s + a.score;})/(100*networks.size()) << ",\tbest " << networks[0].score/100 << ",\tworst " << networks.back().score/100 << ",\t biggest error " <<  biggestError << ",\t precision " << precision/(networks.size())*100.0 << "%" << "\n";
            if(generationNumber % 10000 == 0){
                for(std::vector<Jimmy::LoopingNet>::iterator iter = networks.begin();  iter != networks.end(); iter = std::next(iter)){
                    Jimmy::LoopingNet& network = (*iter);
                    network.susceptibilityToMutations *= 0.5;
                }
                std::cout << "change susceptibilityToMutations to " << networks[0].susceptibilityToMutations << "\n";
            }
        }
        networks[5].evolveFrom(networks[0],networks[1]);
        networks[4].evolveFrom(networks[0],networks[2]);
        networks[3].evolveFrom(networks[1],networks[2]);
    }






    // std::vector<int> topology{1,40,40,1}; // defying how many neurons will be created in each layer
    // Jimmy::Net myNet(topology, Jimmy::Methods::transFuncs::tanh, Jimmy::Methods::lossFuncs::rmse, 0.01);
    // vector<double> inp(topology[0]);
    // vector<double> prop(topology.back());
    // Jimmy::Critic critic(myNet);
    // critic.chooseAll();
    // for(int i = 0; true; i++){ // Test Program if the net can predict the result of sin(x)
    //     inp[0] =  Jimmy::Misc::rand().decimal(0,6); // new random inputs (0 or 1) =
    //     prop[0] = sinf(inp[0]); // crating output target
        
    //     myNet.feedForward(inp); // running the  net
    //     myNet.backProp(prop);
    
    //     // if ((double)myNet.getResult(0) < 1)
    //     // std::cout << (double)myNet.getResult(0) << " " <<  << "\n";

    //     // checking if passed
    //     if(abs(prop[0] - myNet.getResult(0))< 0.1){
    //         cout << "\x1b[42m passed \x1b[0m\n";
    //         // critic.reward();
    //     }
    //     else{
    //         cout <<"\x1b[41m failed \x1b[0m\n";
    //         // critic.punish();
    //     }
    //     // cout << inp[0] << "\n";
    //     // // outputting data to the console
    //     cout << "example " << i << "\n"
    //          << "result:" << myNet.getResult(0) << "\ttarget: " << prop[0] << "\tloss: " << myNet.getLoss() <<"\n\n";

    // }
    
    // Jimmy::includeTest(); // test if the JimmyNeuron is correctly included

    return 0;
}