#include <iostream>

#ifdef OPTIONAL_LIBRARY_SFML
#include "JimmyNeuron/Games/BeeGame.hpp"
#endif // OPTIONAL_LIBRARY_SFML

int main(){
    #ifdef OPTIONAL_LIBRARY_SFML
        Games::BeeGame game;
        game.start();
    #else
        std::cout << "SFML NOT FOUND" << std::endl;
    #endif // OPTIONAL_LIBRARY_SFML
}
