#include "manager.h"

using namespace std;
using namespace sf;
using namespace game;

void GameManager::LoadPerm(){
    vec<CDataDes> tmp_c;
    for(HChunkDesc & cd : this->cCDes){
        CDataDes * c = CH::QuickFindDes(cd,cCDDes);
        ////这里引入了异常
        //if(!c)throw "GameCrashed";
        if(c)UnloadCDataDes(*c);
        tmp_c.push_back(LoadCDataDes(cd));
    }
    cCDDes = tmp_c;
}

Chunk * GameManager::LoadChunk(unsigned int dimension,Pt2Di id){
    Chunk * c = CH::FindChunk(loaded,dimension,id);
    if(!c){
        c = new Chunk(id,dimension);
        loaded[dimension].push_back(c);
    }
    c->addRef();
    return c;
}

void GameManager::UnloadChunk(Chunk* c){
    c->delRef();
    if(c->needDestory()){
        auto cc = find(loaded[c->dimension].begin(),loaded[c->dimension].end(),c);
        if(cc != loaded[c->dimension].end()){//Means the chunk is maybe a ghost chunk
            loaded[c->dimension].erase(cc);
        }
        delete c;
    }
}


void GameManager::UnloadCDataDes(CDataDes& w){
    for(Chunk* c : w.chunks){
        UnloadChunk(c);
    }
}

CDataDes GameManager::LoadCDataDes(HChunkDesc& w){
    CDataDes ret;
    ret.chunks = QuickBuildChunks(CH::QuickBuildSurrId(w.center,w.size),w.dimension);
    ret.desc = w;
    return ret;
}

vec<Chunk*> GameManager::QuickBuildChunks(vec<Pt2Di> ids,unsigned int b_dimension){
    vec<Chunk*> c;
    for(Pt2Di & p : ids){
        c.push_back(LoadChunk(b_dimension,p));
    }
    return c;
}
