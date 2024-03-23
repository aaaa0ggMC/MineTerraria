#ifndef CHUNK_H_INCLUDED
#define CHUNK_H_INCLUDED
#include "properties.h"
#include <vector>
#include "../-gameobjects/Tile.h"

namespace gm{

    using namespace std;

    using tile_y = vector<Tile*> *;
    using tile_y_root = vector<Tile*>;
    using tiles_x = vector<tile_y>;

    struct ChunkMap{
        ChunkMap(unsigned int width,unsigned int height);

        Tile* getTile(unsigned int re_x,unsigned int re_y);
        void genEmpty();

        tiles_x tiles;
        bool available;
        unsigned int w;
        unsigned int h;
    };

    struct Chunk{
        ///data
        ChunkMap data;
        ///properties
        unsigned int refer;
        bool permanent;

        Chunk(bool genChunk = false);

        void addRef();
        unsigned int delRef();
        bool needDestroy();
    };

}

#endif // CHUNK_H_INCLUDED
