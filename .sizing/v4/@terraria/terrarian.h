#ifndef TERRARIAN_H_INCLUDED
#define TERRARIAN_H_INCLUDED
#include "../kernel.h"

using namespace sf;

/*
@deprecated
struct SingleSinFunction{
    ///[scale]sin([frequency]coordinate.x + [bias])
    float scale;
    float frequency;
    float bias;

    SingleSinFunction(float s,float f,float b){
        scale = s;
        frequency = f;
        bias = b;
    }
};
*/

///Created this to solve some problems easily
int getReversedIndex(int realIdx,unsigned int allOfAll);

#define gri getReversedIndex

// deprecated #define TERRARIAN_GEN_LEV

struct Map{
    unsigned int seed;
    //vector<SingleSinFunction> mapFunctions;
    vector<Vector2f> mapPoints;
    Vector2i  sizeOfMap;
    Map(unsigned int seed,Vector2i mpsize);
    Map();
    void setSeedAndGen(unsigned int sd,int mapMinWidth,int mapMaxWidth,int mapMinHeight,int mapMaxHeight);
};


#endif // TERRARIAN_H_INCLUDED
