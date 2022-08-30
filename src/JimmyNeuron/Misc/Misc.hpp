#ifndef MISC_INCLUDE
#define MISC_INCLUDE

#include <random>
#include <iostream>
#include <climits>

namespace Jimmy{
    namespace Misc{

        class rand {
        private:
            rand(const rand&) = delete; // Delete copy constructor
            rand(); // private constructor
            static rand staticRand;

            size_t seed;

            std::random_device rd; // random device
            std::mt19937_64 generator; // generator

            // int distributors 
            std::uniform_int_distribution<int> disBinary;
            std::uniform_int_distribution<int> disMaxInt;
            std::uniform_int_distribution<int> disAllInt;
            // double distributors
            std::uniform_real_distribution<double> disZeroOneDouble;

        public:
            static rand& get();

            static void setSeed(size_t);
            static void rmSeed();


            int binary(); // returns one or zero
            int allInt(); // returns positive and negative numbers
            int maxInt(); // returns positive number
            int randInt(int n, int N); // returns number in range
            int randInt(int N); // returns number in range from 0 to N

            double Decim(); // returns number from 0.0 to 1.0
            double Decim(double, double); // returns number in range
            double Decim(double); // returns number in range from 0 to N

        };
        
        double calculateTheAnswerToTheUltimateQuestionOfLifeTheUniverseAndEverything();
    } // end Misc
} // end Jimmy

#endif