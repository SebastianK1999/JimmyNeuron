#include "Misc.hpp"




Jimmy::Misc::rand Jimmy::Misc::rand::staticRand;

Jimmy::Misc::rand::rand():
    generator(rd()),
    disAllInt(INT_MIN,INT_MAX),
    disMaxInt(0,INT_MAX),
    disBinary(0,1),
    disZeroOneDouble(0.0,1.0)
{}

Jimmy::Misc::rand& Jimmy::Misc::rand::get(){ return Jimmy::Misc::rand::staticRand; }

int Jimmy::Misc::rand::binary(){
    return this->disBinary(this->generator);
}
int Jimmy::Misc::rand::maxInt(){
    return this->disMaxInt(this->generator);
}
int Jimmy::Misc::rand::allInt(){
    return this->disAllInt(this->generator);
}
int Jimmy::Misc::rand::randInt(int N){
    return this->disMaxInt(this->generator)%N;
}
int Jimmy::Misc::rand::randInt(int n,int N){
    return this->disAllInt(this->generator)%(N-n) + n;
}
double Jimmy::Misc::calculateTheAnswerToTheUltimateQuestionOfLifeTheUniverseAndEverything(){
    std::cout << "The answer to the ultimate question, of life, the universe, and everything. Is \x1b[1m" << 42 << "\x1b[0m!!!" << std::endl;
    return 42;
}

// rand Double
double Jimmy::Misc::rand::Double(){
    return this->disZeroOneDouble(this->generator);
}
double Jimmy::Misc::rand::Double(double n, double N){
    return this->disZeroOneDouble(this->generator) * (N-n) + n;
}
double Jimmy::Misc::rand::Double(double N){
    return this->Double(0,N);
}