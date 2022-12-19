#ifndef TILES_H_INCLUDED
#define TILES_H_INCLUDED
#include "Registry.h"

#define TILE_BASE "res/imgs/tiles/"
#define TL(id,pth) {id,TILE_BASE pth}
vector<game::Register::RTile> tiles = {
    TL(0,"tile_test.png"),
    TL(1,"test2.png")
};


#endif // TILES_H_INCLUDED
