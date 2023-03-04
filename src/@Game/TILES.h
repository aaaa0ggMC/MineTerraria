#ifndef TILES_H_INCLUDED
#define TILES_H_INCLUDED
#include "Registry.h"
#include "IDs.h"

#define TILE_BASE "res/imgs/tiles/"
#define BLOCK_BASE "res/imgs/blocks/"
#define TL(id,pth) {id,TILE_BASE pth}
#define BL(id,pth) {id,BLOCK_BASE pth}
vector<game::Register::RTile> tiles = {
    TL(TILE_GRASS,"grass.png"),
    TL(TILE_SAND,"sand.png")
};

vector<game::Register::RTile> blocks = {
    BL(BLOCK_WOOD,"wood.png")
};


#endif // TILES_H_INCLUDED
