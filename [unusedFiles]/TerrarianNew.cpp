#include "TerrarianNew.h"
#include <thread>

using namespace std;
using namespace trnd;

#define SAMPLES 200
#define MULTI(X) ((4)*(X))
#define SMOOTH_TIME MULTI(8)

template<class T> T & operator>>(vector<T> & v,int index){
    return getVectorValueEx(v,index);
}

Dimension::Dimension(unsigned int widthChunks,unsigned int heightChunks,unsigned int seed,int * status){
    wc = widthChunks;
    hc = heightChunks;
    _SCP(status,1);
    this->rnd.srand(seed);
    ///Generating ys
    ys.resize(widthChunks * CHUNK_SIZE);
    int stepSamp = (widthChunks * CHUNK_SIZE) / SAMPLES;
    for(unsigned int i = 0;i < widthChunks * CHUNK_SIZE;i += stepSamp){
        ys>>i = (rnd.getIntRangedEq(-128,256));
    }
    ///Smooth
    for(unsigned int smn = 0;smn < SMOOTH_TIME/4 * 3;++smn){
        for(unsigned int i = 0;i < widthChunks * CHUNK_SIZE;++i){
            ys>>i = ((ys >> (i+1)) + (ys >> (i - 1)))/ 2;
        }
    }
    ///Spawn point is flat
    for(int i = -16;i <= 16;++i){
        ys >> (grin(i,widthChunks)) = rnd.getIntRangedEq(-4,4);
    }
    ///Smooth
    for(unsigned int smn = 0;smn < SMOOTH_TIME/4;++smn){
        for(unsigned int i = 0;i < widthChunks * CHUNK_SIZE;++i){
            ys>>i = ((ys >> (i+1)) + (ys >> (i - 1)))/ 2;
        }
    }
    _SCP(status,100);

    {
        int ta = 0;
        string binded = "";

        for_each(ys.begin(),ys.end(),[&](int i)->void{
            binded += _s("Point ") + to_string(ta) + _s(":") + to_string(i) + _s("\n");
            ++ta;
        });

        cout << binded << endl;
    }
}

GameUniverse::GameUniverse(unsigned int seed){
    //Using multi-threaded to control
    thread t([&](GameUniverse * ptr)->void{
        int status = 0;
        #define DimensionCount 1
        Dimension mainWorld(32,16,seed,&status);
        ptr->loadingStatus = _f(status)/_f(DimensionCount);
        ptr->dimensions.push_back(mainWorld);
        ptr->inited = true;
        #undef DimensionCount
    },this);
    t.detach();
}

void Dimension::loadBase(float x,float y,unsigned int loadChunkW,unsigned int loadChunkH){

}
