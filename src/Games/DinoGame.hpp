#ifndef GAMES_INCLUDED
#define GAMES_INCLUDED

#include <JimmyNeuron/Jimmy.hpp>
#include <vector>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

namespace Games{
    class DinoGame{
        public:
        class obsticle{
            public:
            sf::Image img;
            sf::Texture texture;
            sf::RectangleShape body;

            double getTarget(int);
            bool move(double);
            obsticle();
            
        };
        class player{
            public:
            sf::RectangleShape body;
            bool isJumping = true;
            bool isDucking = false;
            double y;
            double vy;
            double height;
            double duckHeight;
            double width;
            void gravity();
            void jump();
            void duck();
            player();

        };
        sf::RenderWindow Window;
        obsticle obsts;
        player p;
        Jimmy::Net NeuralNet;
        bool canThink = true;
        bool knowsHowToJump = false;
        bool wereTouching;
        int countdown = 45;
        std::vector<double> target{0,0};
        double speed;
        double score;
        void move();
        void logistics();
        void start();

        DinoGame();
    };
}

#endif