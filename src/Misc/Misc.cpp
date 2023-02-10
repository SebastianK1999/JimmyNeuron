#include "JimmyNeuron/Misc/Misc.hpp"


Jimmy::Misc::MISC_RAND Jimmy::Misc::MISC_RAND::staticRand;
//Jimmy::Misc::MISC_RAND Jimmy::Misc::MISC_RAND::staticRandFromSeed(mn);

Jimmy::Misc::MISC_RAND::MISC_RAND() :
    generator(rd()),
    disAllInt(INT_MIN, INT_MAX),
    disMaxInt(0, INT_MAX),
    disBinary(0, 1),
    disZeroOneDouble(0.0, 1.0)
{}

Jimmy::Misc::MISC_RAND& Jimmy::Misc::MISC_RAND::get() { return Jimmy::Misc::MISC_RAND::staticRand; }

void Jimmy::Misc::MISC_RAND::setSeed(size_t arg_seed) {
    Jimmy::Misc::MISC_RAND::get().seed = arg_seed;
    std::mt19937_64 gen(arg_seed);
    Jimmy::Misc::MISC_RAND::get().generator = gen;
}

void Jimmy::Misc::MISC_RAND::rmSeed() {
    std::mt19937_64 gen(Jimmy::Misc::MISC_RAND::get().rd());
    Jimmy::Misc::MISC_RAND::get().generator = gen;
}

int Jimmy::Misc::MISC_RAND::binary() {
    return this->disBinary(this->generator);
}
int Jimmy::Misc::MISC_RAND::maxInt() {
    return this->disMaxInt(this->generator);
}
int Jimmy::Misc::MISC_RAND::allInt() {
    return this->disAllInt(this->generator);
}
int Jimmy::Misc::MISC_RAND::randInt(int N) {
    return this->disMaxInt(this->generator) % N;
}
int Jimmy::Misc::MISC_RAND::randInt(int n, int N) {
    return n + (this->disMaxInt(this->generator) % abs(N - n));
}

// rand double
double Jimmy::Misc::MISC_RAND::decimal() {
    return this->disZeroOneDouble(this->generator);
}
double Jimmy::Misc::MISC_RAND::decimal(double n, double N) {
    return this->disZeroOneDouble(this->generator) * (N - n) + n;
}
double Jimmy::Misc::MISC_RAND::decimal(double N) {
    return this->decimal(0, N);
}
Jimmy::Misc::MISC_RAND& Jimmy::Misc::rand(){
    return Jimmy::Misc::MISC_RAND::get();
}