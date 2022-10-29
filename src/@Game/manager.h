#ifndef MANAGER_H_INCLUDED
#define MANAGER_H_INCLUDED
#include "../TerrarianNew.h"
#include "player.h"

using namespace std;
using namespace sf;

namespace game{
    struct GameManager{
        map<unsigned int,vec<Chunk*>> loaded;
        vec<HChunkDesc> cCDes;//C Des
        vec<CDataDes> cCDDes;//Data Des
        void LoadPerm();
        void UnloadCDataDes(CDataDes&);
        CDataDes LoadCDataDes(HChunkDesc&);
        Chunk * LoadChunk(unsigned int dimension,Pt2Di id);
        void UnloadChunk(Chunk*);
        vec<Chunk*> QuickBuildChunks(vec<Pt2Di> ids,unsigned int);
    };
}

#endif // MANAGER_H_INCLUDED
