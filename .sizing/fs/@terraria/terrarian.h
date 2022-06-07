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

// deprecated #define TERRARIAN_GEN_LEV

struct Map{
    unsigned int seed;
    //vector<SingleSinFunction> mapFunctions;
    vector<Vector2f> mapPoints;
    Map(unsigned int seed,Vector2i mpsize);
    Map();
    void setSeedAndGen(unsigned int sd,int mapMinWidth,int mapMaxWidth,int mapMinHeight,int mapMaxHeight);
};


#endif // TERRARIAN_H_INCLUDED
