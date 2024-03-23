#include "Chunk.h"

using namespace gm;
using namespace std;

Chunk::Chunk(bool gen):data(chunk_size$,chunk_size$){
    permanent = false;
    refer = 0;
    if(gen){
        data.genEmpty();
    }
}

void Chunk::addRef(){
    refer++;
}

unsigned int Chunk::delRef(){
    if(refer == 0)return 0;
    return --refer;
}

bool Chunk::needDestroy(){
    return !refer;
}

ChunkMap::ChunkMap(unsigned int w,unsigned int h){
    this->w = w;
    this->h = h;
    available = false;
}

Tile* ChunkMap::getTile(unsigned int rx,unsigned int ry){
    if(!available)return NULL;
    return (*tiles[rx])[ry];
}

void ChunkMap::genEmpty(){
    if(available)return;
    tiles.reserve(sizeof(tile_y) * w);
    for(unsigned int i = 0;i < w;i++){
        for(unsigned int j = 0;j < h;j++){
            tile_y n = new tile_y_root;
            n->reserve(sizeof(Tile) * h);
            ///Éú³É¿ÕµÄ
            Tile* emp = new Tile;
        }
    }
}
