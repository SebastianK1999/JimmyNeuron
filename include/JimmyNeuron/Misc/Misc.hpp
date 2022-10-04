#pragma once

#include <random>
#include <iostream>
#include <climits>

namespace Jimmy{
    namespace Misc{
        class MISC_RAND {
        private:
            MISC_RAND(const MISC_RAND&) = delete; // Delete copy constructor
            MISC_RAND(); // private constructor
            static MISC_RAND staticRand;

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
            static MISC_RAND& get();

            static void setSeed(size_t);
            static void rmSeed();


            int binary(); // returns one or zero
            int allInt(); // returns positive and negative numbers
            int maxInt(); // returns positive number
            int randInt(int n, int N); // returns number in range
            int randInt(int N); // returns number in range from 0 to N

            double decimal(); // returns number from 0.0 to 1.0
            double decimal(double, double); // returns number in range
            double decimal(double); // returns number in range from 0 to N

        };
        double calculateTheAnswerToTheUltimateQuestionOfLifeTheUniverseAndEverything();
        MISC_RAND& rand();
    } // end Misc
} // end Jimmy