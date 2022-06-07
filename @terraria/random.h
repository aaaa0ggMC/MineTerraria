#ifndef RANDOM_H_INCLUDED
#define RANDOM_H_INCLUDED

#include <string>
#include <stdlib.h>
#include "../@enc/pubhash.h"

using namespace std;

namespace trnd{
    class Random{
    private:
        unsigned int seed;
    public:
        Random(std::string seed,bool tryParseInt = true);
        Random(unsigned int seed = 114514);
        unsigned int getUInt(unsigned int maxNum = UINT_MAX);
        void srand(std::string seed,bool tryParseInt = true);
        void srand(unsigned int seed);
        /** \brief Return random number,not contains edge
         *
         * \param
         * \param
         * \return
         *
         */

        int getIntRanged(int minNum,int maxNum);

        int getIntRangedEq(int minNum,int maxNum);

        float getFloat(float minNum,float maxNum);

        float getFloatRegular();//Bet 0 and 1
    };
}


#endif // RANDOM_H_INCLUDED
