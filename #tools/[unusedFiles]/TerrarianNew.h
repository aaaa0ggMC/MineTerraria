#ifndef TERRARIANNEW_H_INCLUDED
#define TERRARIANNEW_H_INCLUDED
#include "kernel.h"
#include "./@terraria/random.h"
#include "helping.h"

using namespace std;
using namespace sf;

#define CHUNK_SIZE 16

//+1 or -1 means the spawn index
#define SPAWN_POINT_INDEX 1

struct Block{
    int block_id;
    Vector2f position;
};

struct Chunk{//16*16
    vector<Block> blocks;
};

struct Dimension{
    vector<Chunk> chunks;
    vector<float> ys;
    int dimension_id;
    unsigned int wc,hc;
    trnd::Random rnd;
    Dimension(unsigned int widthChunks,unsigned int heightChunks,unsigned int seed,int * status);

    void loadBase(float x,float y,unsigned int loadChunkW,unsigned int loadChunkH);
};

struct GameUniverse{
    bool inited {false};//C++ 17Ьиад
    float loadingStatus {0};
    vector<Dimension> dimensions;
    GameUniverse(unsigned int seed = 114514);
};

template<class T> T & getVectorValueEx(vector<T> & v,int index){
    if(index < 0){
        return v[index + v.size()];
    }else if(index >= (int)v.size()){
        return v[index - v.size()];
    }else{
        return v[index];
    }
}

template<class T> T & operator>>(vector<T> & v,int index);

inline int getReversedIndexN(int rindex,unsigned int chunks){
    unsigned int allSz = chunks * CHUNK_SIZE;
    unsigned int center = allSz / 2 +SPAWN_POINT_INDEX;
    return center + rindex;
}

#define grin getReversedIndexN

#endif // TERRARIANNEW_H_INCLUDED
