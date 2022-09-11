#include "JimmyNeuron/Misc/Misc.hpp"


Jimmy::Misc::rand Jimmy::Misc::rand::staticRand;
//Jimmy::Misc::rand Jimmy::Misc::rand::staticRandFromSeed(mn);

Jimmy::Misc::rand::rand() :
    generator(rd()),
    disAllInt(INT_MIN, INT_MAX),
    disMaxInt(0, INT_MAX),
    disBinary(0, 1),
    disZeroOneDouble(0.0, 1.0)
{}

Jimmy::Misc::rand& Jimmy::Misc::rand::get() { return Jimmy::Misc::rand::staticRand; }

void Jimmy::Misc::rand::setSeed(size_t arg_seed) {
    Jimmy::Misc::rand::get().seed = arg_seed;
    std::mt19937_64 gen(arg_seed);
    Jimmy::Misc::rand::get().generator = gen;
}

void Jimmy::Misc::rand::rmSeed() {
    std::mt19937_64 gen(Jimmy::Misc::rand::get().rd());
    Jimmy::Misc::rand::get().generator = gen;
}

int Jimmy::Misc::rand::binary() {
    return this->disBinary(this->generator);
}
int Jimmy::Misc::rand::maxInt() {
    return this->disMaxInt(this->generator);
}
int Jimmy::Misc::rand::allInt() {
    return this->disAllInt(this->generator);
}
int Jimmy::Misc::rand::randInt(int N) {
    return this->disMaxInt(this->generator) % N;
}
int Jimmy::Misc::rand::randInt(int n, int N) {
    return this->disAllInt(this->generator) % (N - n) + n;
}

// rand double
<<<<<<< HEAD:src/Misc/Misc.cpp
double Jimmy::Misc::rand::Decimal() {
    return this->disZeroOneDouble(this->generator);
}
double Jimmy::Misc::rand::Decimal(double n, double N) {
=======
double Jimmy::Misc::rand::Decim() {
    return this->disZeroOneDouble(this->generator);
}
double Jimmy::Misc::rand::Decim(double n, double N) {
>>>>>>> 014d68cdc4addcc9817bd9ac8fa40f09385d35d0:src/JimmyNeuron/Misc/Misc.cpp
    return this->disZeroOneDouble(this->generator) * (N - n) + n;
}
double Jimmy::Misc::rand::Decim(double n, double N) {
    return this->disZeroOnedouble(this->generator) * (N - n) + n;
}
double Jimmy::Misc::rand::Decimal(double N) {
    return this->Decimal(0, N);
}