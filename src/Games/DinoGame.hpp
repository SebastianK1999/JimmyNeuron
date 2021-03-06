#ifndef GAMES_DINOGAME_INCLUDED
#define GAMES_DINOGAME_INCLUDED

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
        obsticle obsts;
        player p;
        Jimmy::Net NeuralNet;
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

        DinoGame();
    };
}

#endif