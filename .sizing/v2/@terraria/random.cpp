#include "random.h"

using namespace trnd;

Random::Random(std::string seed,bool tryParseInt){
    this->srand(seed,tryParseInt);
}

Random::Random(unsigned int seed){
    this->srand(seed);
}

unsigned int Random::getUInt(unsigned int maxNum){
    long re = (seed * 114514 + 1919 - 2174169)*7 - 76126;
    seed = re < 0 ? -re : re;
    return seed % maxNum;
}

void Random::srand(std::string seed,bool tryParseInt){
    this->seed = HashStringToUInt(seed,tryParseInt);
}

void Random::srand(unsigned int seed){
    this->seed = seed;
}

/** \brief Return random number,not contains edge
 *
 * \param
 * \param
 * \return
 *
 */

int Random::getIntRanged(int minNum,int maxNum){
    --minNum;
    unsigned int len = getUInt() % (maxNum - minNum);
    return minNum + len;
}

int Random::getIntRangedEq(int minNum,int maxNum){
    unsigned int len = getUInt() % (maxNum - minNum + 1);
    return minNum + len;
}

float Random::getFloat(float minNum,float maxNum){
    return getFloatRegular() * (maxNum - minNum) + minNum;
}

float Random::getFloatRegular(){//Bet 0 and 1
    #define reg_max 100000
    int num = getIntRangedEq(0,reg_max);
    return (float)(num) / (float)reg_max;
}
