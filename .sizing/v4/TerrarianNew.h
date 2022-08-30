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
    Block blocks[CHUNK_SIZE][CHUNK_SIZE];
};

struct LoadChunkStatus{
    vector<string> missingItems;
};

struct Dimension{
    vector<Chunk> chunks;
    int dimension_id;
    trnd::Random rnd;

    Dimension(unsigned int,unsigned int seed,int * status);

    LoadChunkStatus LoadChunk(int,int);
};

struct GameUniverse{
    bool inited {false};
    float loadingStatus;
    map<int,Dimension> dimensions;
    GameUniverse(unsigned int seed = 114514);
};

#define grin getReversedIndexN

#endif // TERRARIANNEW_H_INCLUDED
