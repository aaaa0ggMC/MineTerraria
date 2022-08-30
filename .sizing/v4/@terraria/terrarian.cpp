#include "terrarian.h"

using namespace sf;
using namespace std;

static unsigned int rndSeed = 0;

int getReversedIndex(int realIdx,unsigned int allOfAll){//allOfAll必须可以被2整除
    unsigned int mapHalf = allOfAll/2;
    return realIdx + mapHalf;
}

Map::Map(unsigned int seed,Vector2i mpsize){
    sizeOfMap = mpsize;
    setSeedAndGen(seed,-(int)(mpsize.x / 2),mpsize.x - (int)(mpsize.x / 2),-300,600);//-(int)(mpsize.y / 2),mpsize.y - (int)(mpsize.x / 2)
}

Map::Map(){}

void srandm(unsigned int seed){
    rndSeed = seed;
}

int randRangeFn(int minV,int maxVEq){
    long re = (rndSeed * 114514 + 1919 - 2174169)*7 - 76126;
    rndSeed = re < 0 ? -re : re;
    cout << re << " " << rndSeed << endl;
    return rndSeed % (maxVEq - minV + 1) + minV;
}

#define SAMPLES 100
#define SMT_TM 32

void Map::setSeedAndGen(unsigned int sd,int mapMinWidth,int mapMaxWidth,int mapMinHeight,int mapMaxHeight){
    seed = sd;
    ///Generation

    srandm(sd);

    int mapW = mapMaxWidth - mapMinWidth;
    int mapH = mapMaxHeight - mapMinHeight;

    ///Generating random points
    vector<float> ys;
    ys.resize(mapMaxWidth - mapMinWidth);
    for(int i = 0;i < SAMPLES;i++){
        ys[randRangeFn(mapMinWidth,mapMaxWidth) + mapMaxWidth] = randRangeFn(mapMinHeight,mapMaxHeight);
    }

    ///Spawn Point flat
    for(int i = (int)(gri(0,mapW) - 16);i <= (int)(gri(0,mapW) + 16);++i){
        ys[i] = 0;
    }
    ///Smooth
    for(int t = 0;t < SMT_TM;++t){
        for(int idx = 0;idx < mapMaxWidth - mapMinWidth;++idx){
            float ty = 0;
            if(idx == 0){
                ty = (ys[1] + ys[mapMaxWidth - mapMinWidth - 1])/2;
            }else if(idx == mapMaxWidth - mapMinWidth -1){
                ty = (ys[0] + ys[mapMaxWidth - mapMinWidth - 2])/2;
            }else{
                ty = (ys[idx - 1] + ys[idx + 1])/2;
            }
            ys[idx] = ty;
        }
    }
    ///Adding
    for(int v = 0;v < mapW;++v){
        mapPoints.push_back(Vector2f(v + mapMinWidth,ys[v]));
    }
}
