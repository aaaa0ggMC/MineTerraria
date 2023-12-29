#include "random.h"

using namespace trnd;

Random::Random(std::string seed,bool tryParseInt){
    this->srand(seed,tryParseInt);
    distr = std::uniform_int_distribution<unsigned long>(0,UINT_MAX);
}

Random::Random(unsigned int seed){
    this->srand(seed);
    distr = std::uniform_int_distribution<unsigned long>(0,UINT_MAX);
}

void Random::srand(std::string seed,bool tryParseInt){
    eng = std::mt19937(HashStringToUInt(seed,tryParseInt));
    this->seed = HashStringToUInt(seed,tryParseInt);
}

void Random::srand(unsigned int seed){
    eng = std::mt19937(seed);
    this->seed = seed;
}


unsigned int Random::genUint(){
    return (unsigned int)distr(eng);
}

unsigned int Random::genUint(unsigned int max){
    std::uniform_int_distribution<unsigned int> ds(0,max);
    return ds(eng);
}

unsigned long Random::genUlong(){
    return distr(eng);
}

float Random::genFloat01(){
    unsigned long a,b;
    a = distr(eng);
    b = distr(eng);
    if(b == 0){
        return 1;
    }
    return (float)((double)a/(double)b);
}
