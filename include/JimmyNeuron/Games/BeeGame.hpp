#pragma once
#ifdef OPTIONAL_LIBRARY_SFML

#include <vector>

#include "SFML/Window.hpp"
#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"

#include "JimmyNeuron/Jimmy.hpp"

namespace Games{
    class BeeGame{
        public:
        class Obstacle{
            public:
            sf::Image img;
            sf::Texture texture;
            sf::RectangleShape body;

            double getTarget(int);
            bool move(double);
            Obstacle();
            
        };
        class player{
            public:
            void reset();
            sf::RectangleShape body;
            bool isJumping = true;
            bool isDucking = false;
            double y;
            double vy;
            double height;
            double duckHeight;
            double width;
            int duckTimeout;
            void gravity();
            void jump();
            void duck();
            player();

        };
        sf::RenderWindow Window;
        Obstacle obstacles;
        player p;
        Jimmy::Net neuralNet;
        Jimmy::Critic critic;
        bool canThink = true;
        bool knowsHowToJump = false;
        bool wereTouching;
        int countdown = 45;
        std::vector<double> target{0,0};
        void reset();
        double speed;
        double score;
        void move();
        void logistics();
        void start();
        void gameSim();
        int displayCountdown = 0;
        sf::Font font;
        sf::Text text;

        BeeGame();
    };
}

#endif // OPTIONAL_LIBRARY_SFML