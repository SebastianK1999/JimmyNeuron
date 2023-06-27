#include <immintrin.h>
#include <sstream>
#include <iostream>
#include <iomanip>


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

double Jimmy::Misc::WeightedSumSIMD(const std::vector<double>& value, const std::vector<double>& weights)
{
    double weightedSum = 0.0;
    __m128d sum = _mm_setzero_pd();
    for (int i = 0; i < value.size(); i += 2)
    {
        __m128d v = _mm_loadu_pd(&value[i]);
        __m128d w = _mm_loadu_pd(&weights[i]);
        sum = _mm_add_pd(sum, _mm_mul_pd(v, w));
    }
    weightedSum = _mm_cvtsd_f64(_mm_hadd_pd(sum, sum));
    
    return weightedSum;
}

std::string Jimmy::Misc::GetDate()
{
    time_t now = time(0);
    tm *ltm = localtime(&now);
    int year = 1900 + ltm->tm_year;
    int month = 1 + ltm->tm_mon;
    int day = ltm->tm_mday;
    std::stringstream ss;
    ss << year << "-" << std::setfill('0') << std::setw(2) << month << "-" << std::setfill('0') << std::setw(2) << day;
    return ss.str();
}

std::string Jimmy::Misc::GetDateTime()
{
    time_t now = time(0);
    tm *ltm = localtime(&now);
    std::stringstream ss;
    ss << 1900 + ltm->tm_year 
        << "-" << std::setfill('0') << std::setw(2) << 1 + ltm->tm_mon 
        << "-" << std::setfill('0') << std::setw(2) << ltm->tm_mday 
        << "_" << std::setfill('0') << std::setw(2) << ltm->tm_hour 
        << ":" << std::setfill('0') << std::setw(2) << ltm->tm_min 
        << ":" << std::setfill('0') << std::setw(2) << ltm->tm_sec;
    return ss.str();
}