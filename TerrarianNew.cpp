#include "TerrarianNew.h"
#include <stdio.h>
#include <thread>

using namespace std;
using namespace trnd;

#define SAMPLES 200
#define MULTI(X) ((4)*(X))
#define SMOOTH_TIME MULTI(8)

template<class T> T & operator>>(vector<T> & v,int index){
    return getVectorValueEx(v,index);
}

Dimension::Dimension(unsigned int diId,unsigned int seed,int * status){
    this->rnd.srand(seed);
    dimension_id = diId;
    (*status)++;
    cout << "Initialized Dimension Over-world" << endl;
}

GameUniverse::GameUniverse(unsigned int seed){
    //this->inited = false;
    this->loadingStatus = 0;
    int default_id = 0;
    //Using multi-threaded to control
    thread t([&](GameUniverse * ptr)->void{
        int status = 0;
        #define DimensionCount 1
        Dimension mainWorld(default_id++,seed,&status);
        mainWorld.LoadChunk(0,0);
        ptr->loadingStatus = _f(status)/_f(DimensionCount);
        ptr->dimensions.insert(make_pair(mainWorld.dimension_id,mainWorld));
        ptr->inited = true;
        #undef DimensionCount
    },this);
    t.detach();
}

#define DEFAULT_SEED_ID 1145141919810

LoadChunkStatus Dimension::LoadChunk(int x,int y){
    unsigned int seed = rnd.Seed();
    seed = ((seed | x) & y)^ DEFAULT_SEED_ID;
    printf("%X",seed);
    return {{}};
}
