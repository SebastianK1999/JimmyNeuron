#include <iostream>

#ifdef OPTIONAL_LIBRARY_SFML
#include "JimmyNeuron/Games/TicTacToe.hpp"
#endif // OPTIONAL_LIBRARY_SFML

int main(){
    #ifdef OPTIONAL_LIBRARY_SFML
        Games::TicTacToe game;
        game.start();
    #else
        std::cout << "SFML NOT FOUND" << std::endl;
    #endif // OPTIONAL_LIBRARY_SFML
}
