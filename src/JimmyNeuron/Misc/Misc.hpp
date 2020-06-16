#ifndef MISC_INCLUDE
#define MISC_INCLUDE

#include <random>
#include <iostream>
#include <climits>

namespace Jimmy{
    namespace Misc{

        class rand{
        private:
        rand(const rand&) = delete; // Delete copy constructor
        rand(); // private constructor
        static rand staticRand;

        std::random_device rd; // random device
        std::mt19937 generator; // generator
    
        // int distributors 
        std::uniform_int_distribution<int> disBinary; 
        std::uniform_int_distribution<int> disMaxInt;
        std::uniform_int_distribution<int> disAllInt;
        // float_t distributors
        std::uniform_real_distribution<float_t> disZeroOnefloat_t;
        
        public:
        static rand& get();

        int binary(); // returns one or zero
        int allInt(); // returns positive and nagative numbers
        int maxInt(); // returns poitive number
        int randInt(int n, int N); // returns number in range
        int randInt(int N); // returns number in range from 0 to N

        float_t Decim(); // returns number from 0.0 to 1.0
        float_t Decim(float_t,float_t); // returns number in range
        float_t Decim(float_t); // returns number in range from 0 to N

        };
        
        float_t calculateTheAnswerToTheUltimateQuestionOfLifeTheUniverseAndEverything();
    } // end Misc
} // end Jimmy

#endif