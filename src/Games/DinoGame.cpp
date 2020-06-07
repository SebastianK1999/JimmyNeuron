#include "DinoGame.hpp"

Games::DinoGame::player::player(){
    this->width = 30;
    this->height = 45;
    this->duckHeight = 30;
    this->y = 5;
    this->body.setSize(sf::Vector2f(this->width,this->height));
    this->body.setOrigin(sf::Vector2f(this->width / 2 , this->height));
    this->body.setFillColor(sf::Color(200, 20, 75, 255));
    this->body.setPosition(sf::Vector2f(100,this->y));
}

void Games::DinoGame::player::gravity(){
    if(this->isJumping){
        this->y += this->vy;
        this->vy += 2;

        if(this->y >= 175){
            this->y = 175;
            this->vy = 0;
            this->isJumping = false;
        }
        this->body.setPosition(sf::Vector2f(this->body.getPosition().x,this->y));

    }
}

void Games::DinoGame::player::jump(){
    if(!this->isJumping){
        this->isJumping = true;
        this->vy = -23;
    }
}
void Games::DinoGame::player::duck(){
    this->body.setSize(sf::Vector2f(this->width,this->height));
    if(this->isDucking){
        this->body.setSize(sf::Vector2f(this->width,this->duckHeight));
        this->isDucking = false;
    }
}

void Games::DinoGame::player::reset(){
    this->isJumping = 0;
    this->vy = 0;
    this->y = 175;
    this->body.setPosition(sf::Vector2f(this->body.getPosition().x,this->y));
}

Games::DinoGame::obsticle::obsticle(){
    this->img.loadFromFile("../res/bee_img.png");
    this->texture.loadFromImage(this->img);
    this->body.setTexture(&this->texture);
    this->body.setSize(sf::Vector2f(20,30));
    //this->body.setFillColor(sf::Color(0,255,255));
    this->body.setOrigin(10,30);
    this->body.setPosition(sf::Vector2f(500,175));

}
bool Games::DinoGame::obsticle::move(double speed){
    this->body.setPosition(sf::Vector2f(this->body.getPosition().x - speed, this->body.getPosition().y));
    if(this->body.getPosition().x < 0){
        return 1;
    }
    return 0;
}
double Games::DinoGame::obsticle::getTarget(int i){
    if(i == 0){
                if(this->body.getPosition().y < 170){
            return 1;
        }
        return 0;
    }
    else if(i == 1){
        if(this->body.getPosition().y == 175){
            return 1;
        }
        return 0;
    }
    return 0;
}

void Games::DinoGame::reset(){
    this->obsts.body.setPosition(sf::Vector2f(500,175 - 0 * Jimmy::Misc::rand::get().binary()));
    this->score = 0;
    this->wereTouching = false;
    this->canThink = true;
}

void Games::DinoGame::gameSim(){
    this->NeuralNet.feedForward( std::vector<double>{(double) this->p.isJumping,(double) this->obsts.body.getPosition().x/250} );
    critic.chooseHighest();
    critic.recordMove();

    if(this->p.isJumping && this->NeuralNet.getResult(0) > 0.5){
        std::cout <<"\x1b[43mInvalid move\x1b[0m\n";
        this->critic.punish();
        this->reset();
        this->p.reset();
    }
    if(this->wereTouching){
        std::cout <<"\x1b[41mfailed\x1b[0m\n";
        critic.punishRecords();
        this->reset();
        this->p.reset();
    }

    if(this->NeuralNet.getResult(0) > 0.5){
        this->p.jump(); 
        canThink = false;
    }


    this->p.gravity();

    if( !this->wereTouching && this->p.body.getGlobalBounds().intersects(this->obsts.body.getGlobalBounds())){
        this->wereTouching = true;
    }



    if(this->obsts.move(this->speed)){
        if(!this->wereTouching){
            std::cout <<"\x1b[42mpassed\x1b[0m\n";
            critic.rewardRecords(1);
        }


        this->reset();
    }

}

void Games::DinoGame::start(){
    this->reset();
    std::cout << "stariting DinoGame\n";
    sf::Event event;

    for(int i = 0; i < 1000000; i++){
        this->gameSim();
    }
    
    while(this->Window.isOpen()){
        this->gameSim();

        this->Window.clear(sf::Color(255,255,255,255));

        this->Window.draw(this->obsts.body);
        this->Window.draw(this->p.body);
        this->Window.display();

    }
}

Games::DinoGame::DinoGame():
NeuralNet(std::vector<unsigned int>{2,6,1} ,Jimmy::Methods::transFuncs::tanh, Jimmy::Methods::lossFuncs::rmse, 0.1),
speed(5),
score(0),
obsts(),
critic(NeuralNet)

{   
    this->Window.create(sf::VideoMode(500,200), "DinoGame");
    this->Window.setFramerateLimit(60);
    //for(int i  = 0; i < 2; i++){
    //    this->obsts.push_back(Games::DinoGame::obsticle());
    //}
}