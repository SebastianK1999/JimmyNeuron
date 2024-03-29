#ifdef OPTIONAL_LIBRARY_SFML

#include <numeric>

#include "JimmyNeuron/Games/TicTacToe.hpp"

bool Games::TicTacToe::checkFull(){
    int zerosCount = 0;
    for(int i = 0; i < this->board.size(); i++){
        if(this->board[i] == 0){
            zerosCount++;
        }
    }
    if(zerosCount == 0){
        return 1;
    }
    return 0;
}
int Games::TicTacToe::checkWin(){
    for(int i = 0; i< 9; i+=3){
        if(this->board[i] == this->board[i+1] && this->board[i] == this->board[i+2] && this->board[i] != 0){
            return(board[i]);
        }
    }
    for(int i = 0; i< 3; i++){
        if(this->board[i] == this->board[3+i] && this->board[i] == this->board[i+6] && this->board[i] != 0){
            return(board[i]);
        }
    }
    if(this->board[0] == this->board[4] && this->board[0] == this->board[8] && this->board[4] != 0){
            return(board[4]);
    }
    if(this->board[2] == this->board[4] && this->board[2] == this->board[6] && this->board[4] != 0){
            return(board[4]);
    }

    return 0;
}

void Games::TicTacToe::reset(){
    for(int i = 0; i < this->board.size(); i++){
        this->board[i] = 0;
    }
    this->movesInRound = 1;
}

void Games::TicTacToe::randomMove(int symbol){
    while(true){
        int index = Jimmy::Misc::rand().randInt(9);
        if(this->board[index] == 0){
            this->board[index] = symbol;
            return;
        } 
    }

}

void Games::TicTacToe::gameSim(){
    // cleaning after last round (not loop)
    if(this->checkWin() != 0 || this->checkFull()){
        this->reset();
    }
    // end of  cleaning

    // opponent
    this->randomMove(-1);
    if(this->checkWin() == -1){
        std::cout <<"\x1b[41m failed \x1b[0m\n";
        this->critic.punishRecords();
        this->critic.clearRecords();
        return;
    }
    // end of opponent

    // cleaning after last round (not loop)
    if(this->checkWin() != 0 || this->checkFull()){
        this->reset();
        return;
    }
    // end of  cleaning

    this->movesInRound++;
    ai_turn:
    this->neuralNet.feedForward( this->board );
    this->critic.chooseHighest();
    
    std::vector<int> badIndexes{};
    std::vector<int> unusedIndexes{};
    int index = -1;
    double maxVal;
    int activeNeuronsCount = 0;
    
    for(int i = 0; i < 9; i++){
        if(board[i] == 0){
            index = i;
            maxVal = this->neuralNet.getResult(i);
            break;
        }
    }

    for(int i = 0; i < 9; i++){
        if(this->neuralNet.getResult(i) >= 0.5f){
            activeNeuronsCount++;
            if(i != index || board[i] != 0){
                badIndexes.push_back(i);
            }
        }
        else{
            unusedIndexes.push_back(i);
        }
    }

    // for(int i = 0; i< 3;  i++){
    //     for(int j = 0; j< 3;  j++){
    //         std::cout << "\t" << board[i*+j];
    //     }
    //     std::cout << "\n";
    // }
    // std::cout << index << " " << board[index] << " " << maxVal << "\n";

    // for(int i = index+1; i < this->board.size(); i++){
    //     if(this->neuralNet.getResult(i) > maxVal){
    //         if(maxVal > 0.5){
    //             badIndexes.push_back(index);
    //         }
    //         unusedIndexes.push_back(index);
    //         index = i;
    //         maxVal = this->neuralNet.getResult(i);
    //         activeNeuronsCount++;
    //     }
    //     else if(this->neuralNet.getResult(i)  > 0.5){
    //         unusedIndexes.push_back(index);
    //         activeNeuronsCount++;
    //         badIndexes.push_back(index);
    //     }
    //     else 
    //         unusedIndexes.push_back(index);
    // }

    if(activeNeuronsCount > 1){
        this->critic.chooseSelected(badIndexes);
        std::cout <<"\x1b[45m Multiple moves \x1b[0m\n";
        this->critic.punish();
        badIndexes.clear();
        unusedIndexes.clear();
        // this->reset();
        goto ai_turn;
        // return;
    }
    else if(maxVal < 0.5){
       std::cout <<"\x1b[45mNo move\x1b[0m\n";
       this->critic.chooseAll();
       this->critic.punish();
        badIndexes.clear();
        unusedIndexes.clear();
        goto ai_turn;
    }

    if(this->board[index] != 0){
        std::cout <<"\x1b[45m Invalid move \x1b[0m\n";
        this->critic.chooseHighest();
        this->critic.punish();
        //this->critic.chooseSelected(unusedIndexes);
        //this->critic.reward(1.0/(this->movesInRound*this->movesInRound));
        // this->reset();
        badIndexes.clear();
        unusedIndexes.clear();
        goto ai_turn;

        // return;
    }
    else{
        this->critic.reward(this->movesInRound);
        this->critic.recordMove();
        this->board[index] = 1;
    }


    if(this->checkWin() == 1){
        std::cout <<"\x1b[42m passed \x1b[0m\n";
        this->critic.rewardRecords();
        return;
    }
    // end of Jimmy Move



}

void Games::TicTacToe::start(){
    this->reset();
    std::cout << "starting TicTacToe\n";
    sf::Event event;

    // int n = 1000000;
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
            
            for(int i = 0; i < this->board.size(); i++){
                if(this->board[i] == 1){
                    this->red.setPosition(sf::Vector2f(i%3 * 100, i/3 * 100));
                    this->Window.draw(this->red);
                }
                else if(this->board[i] == -1){
                    this->blue.setPosition(sf::Vector2f(i%3 * 100, i/3 * 100));
                    this->Window.draw(this->blue);
                }
            }

            this->Window.draw(this->textL1);
            this->Window.draw(this->textL2);
            this->Window.display();
        }
    }
}

Games::TicTacToe::TicTacToe():
neuralNet(std::vector<int>{9,90,40,9} ,Jimmy::Methods::transFuncs::tanh, Jimmy::Methods::lossFuncs::rmse, 0.1),
critic(neuralNet)
{   
    bool ok = true;
    ok = ok && this->font.loadFromFile("../res/arial.ttf");
    this->textL1.setFont(this->font);
    this->textL1.setString("Hold SPACE to speedup");
    this->textL1.setPosition(sf::Vector2f(5,5));
    this->textL1.setCharacterSize(14);
    this->textL1.setStyle(sf::Text::Bold);
    this->textL1.setFillColor(sf::Color::Red);
    this->textL1.setOutlineColor(sf::Color::White);
    this->textL1.setOutlineThickness(1.25);

    this->textL2.setFont(this->font);
    this->textL2.setString("learning process");
    this->textL2.setPosition(sf::Vector2f(5,5+16));
    this->textL2.setCharacterSize(14);
    this->textL2.setStyle(sf::Text::Bold);
    this->textL2.setFillColor(sf::Color::Red);
    this->textL2.setOutlineColor(sf::Color::White);
    this->textL2.setOutlineThickness(1.25);

    this->Window.create(sf::VideoMode({300,300}), "TicTacToe");
    this->Window.setFramerateLimit(4);

    this->red.setSize(sf::Vector2f(100,100));
    this->red.setFillColor(sf::Color(255,0,0));

    this->blue.setSize(sf::Vector2f(100,100));
    this->blue.setFillColor(sf::Color(0,0,255));

    if(!ok){
        std::cout << "Error during creation of TicTacToe game\n";
    }

}

#endif // OPTIONAL_LIBRARY_SFML
