#ifndef RANDOM_XX_H_INCLUDED
#define RANDOM_XX_H_INCLUDED

#include <string>
#include <stdlib.h>
#include <random>
#include "../@enc/pubhash.h"

using namespace std;

namespace trnd{
    class Random{
    private:
        unsigned int seed;
    public:
        std::mt19937 eng;


        std::uniform_int_distribution<unsigned long> distr;
        Random(std::string seed,bool tryParseInt = true);
        Random(unsigned int seed = 114514);
        void srand(std::string seed,bool tryParseInt = true);
        unsigned int genUint(unsigned int max);
        void srand(unsigned int seed);
        unsigned int genUint();
        unsigned long genUlong();
        float genFloat01();
    };
}


#endif // RANDOM_XX_H_INCLUDED
