#pragma once
#ifdef OPTIONAL_LIBRARY_SFML

#include <vector>

#include "SFML/Window.hpp"
#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"

#include "JimmyNeuron/Jimmy.hpp"

namespace Games{
    class TicTacToe{

        sf::RenderWindow Window;
        sf::Font font;
        sf::Text textL1;
        sf::Text textL2;
        Jimmy::Net neuralNet;
        Jimmy::Critic critic;
        std::vector<double> board{0,0,0,  0,0,0,  0,0,0};
        sf::RectangleShape red;
        sf::RectangleShape blue;
        int checkWin();
        bool checkFull();
        void reset();
        void gameSim();
        void randomMove(int);

        int displayCountdown = 0;
        int movesInRound = 0;
        
        public:
        void start();
        TicTacToe();
    };
}

#endif // OPTIONAL_LIBRARY_SFML
