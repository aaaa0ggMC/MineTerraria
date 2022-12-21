#ifndef TILES_H_INCLUDED
#define TILES_H_INCLUDED
#include "Registry.h"

#define TILE_BASE "res/imgs/tiles/"
#define BLOCK_BASE "res/imgs/blocks/"
#define TL(id,pth) {id,TILE_BASE pth}
#define BL(id,pth) {id,BLOCK_BASE pth}
vector<game::Register::RTile> tiles = {
    TL(0,"tile_test.png"),
    TL(1,"test2.png")
};

vector<game::Register::RTile> blocks = {
    BL(0,"wood.png")
};


#endif // TILES_H_INCLUDED
