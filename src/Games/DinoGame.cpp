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
        this->vy += 1.35;

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
        this->vy = -14;
    }
}
void Games::DinoGame::player::duck(){
    this->body.setSize(sf::Vector2f(this->width,this->height));
    this->body.setPosition(sf::Vector2f(this->body.getPosition().x,this->y));
    this->isDucking = false;

    if(this->duckTimeout > 0){
        this->body.setSize(sf::Vector2f(this->width,this->duckHeight));
        this->body.setPosition(sf::Vector2f(this->body.getPosition().x,this->y + 15));
        this->duckTimeout--;
        this->isDucking = true;
    }
}

void Games::DinoGame::player::reset(){
    this->isJumping = 0;
    this->body.setSize(sf::Vector2f(this->width,this->height));
    this->body.setPosition(sf::Vector2f(this->body.getPosition().x,this->y));
    this->isDucking = false;
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
        this->body.setPosition(sf::Vector2f(500,this->body.getPosition().y));
        return 1;
    }
    return 0;
}

void Games::DinoGame::reset(){
    this->obsts.body.setPosition(sf::Vector2f(500,175 - 40 * Jimmy::Misc::rand::get().binary()));
    this->score = 0;
    this->wereTouching = false;
    this->canThink = true;
}

void Games::DinoGame::gameSim(){
    this->NeuralNet.feedForward( std::vector<double>{(double) (this->p.isJumping || this->p.isDucking),(double) this->obsts.body.getPosition().x/250, (double) (this->obsts.body.getPosition().y == 175)} );
    critic.chooseLowest();
    critic.recordMove();

    if(this->NeuralNet.getResult(0) > 0.5 && this->NeuralNet.getResult(1) > 0.5){
        std::cout <<"\x1b[43mInvalid move\x1b[0m\n";
        this->critic.chooseAll();
        this->critic.punish();
        this->reset();
        this->p.reset();
    }
    else if((this->p.isJumping || this->p.isDucking) && (this->NeuralNet.getResult(0) > 0.5 || this->NeuralNet.getResult(1) > 0.5)){
        std::cout <<"\x1b[43mInvalid move\x1b[0m\n";
        this->critic.chooseHighest();
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

    if(this->NeuralNet.getResult(0) > 0.5 && this->NeuralNet.getResult(1) < this->NeuralNet.getResult(0)){
        this->p.jump(); 
        canThink = false;
    }
    if(this->NeuralNet.getResult(1) > 0.5 && this->NeuralNet.getResult(1) > this->NeuralNet.getResult(0)){
        this->p.duckTimeout = 30; 
    }

    this->p.duck();
    this->p.gravity();

    if( !this->wereTouching && this->p.body.getGlobalBounds().intersects(this->obsts.body.getGlobalBounds())){
        this->wereTouching = true;
    }



    if(this->obsts.move(this->speed)){
        if(!this->wereTouching){
            std::cout <<"\x1b[42mpassed\x1b[0m\n";
            critic.rewardRecords();
        }
        this->reset();
    }
}

void Games::DinoGame::start(){
    this->reset();
    std::cout << "stariting DinoGame\n";
    sf::Event event;
    int n = 1000000;
    for(int i = 0; i < 0; i++){
        this->gameSim();
    }
    
    while(this->Window.isOpen()){
        this->gameSim();
        while (this->Window.pollEvent(event)){
            if (event.type == sf::Event::Closed){
                this->Window.close();
            }
  
        }  
        if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || ! (this->displayCountdown)){
            
            if (this->displayCountdown == 0){
                displayCountdown = 120;
            }
            this->displayCountdown--;
            this->Window.clear(sf::Color(255,255,255,255));

            this->Window.draw(this->obsts.body);
            this->Window.draw(this->p.body);
            this->Window.draw(this->text);
            this->Window.display();
        }

    }
}

Games::DinoGame::DinoGame():
NeuralNet(std::vector<int>{3,12,6,2} ,Jimmy::Methods::transFuncs::tanh, Jimmy::Methods::lossFuncs::rmse, 0.1),
critic(NeuralNet),
speed(5),
score(0),
obsts()
{   
    this->font.loadFromFile("../res/arial.ttf");
    this->text.setFont(this->font);
    this->text.setString("Hold SPACE to speedup learning prosess");
    this->text.setPosition(sf::Vector2f(5,5));
    this->text.setCharacterSize(14);
    this->text.setStyle(sf::Text::Bold);
    this->text.setFillColor(sf::Color::Red);
    this->text.setOutlineColor(sf::Color::White);
    this->text.setOutlineThickness(1.25);

    this->Window.create(sf::VideoMode(500,200), "DinoGame");
    this->Window.setFramerateLimit(60);

}
